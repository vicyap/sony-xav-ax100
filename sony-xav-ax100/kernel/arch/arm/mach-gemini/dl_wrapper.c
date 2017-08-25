#include <linux/types.h>	// for size_t
#include <asm/hardware/types.h>	// for SINT32, etc
#include <mach/dlmalloc.h>	// for mem_info_t
#include <mach/gp_chunkmem.h>	// for save_data_proc
#include <linux/slab.h>		// for kmalloc/kfree
#include <linux/spinlock.h>	// for spinlock

/* chunkmem to dlmalloc interface */
// necessary for dlmalloc
typedef void *mspace;
mspace g_ms = 0;

#define MALLINFO_FIELD_TYPE size_t
struct mallinfo {
	MALLINFO_FIELD_TYPE arena;    /* non-mmapped space allocated from system */
	MALLINFO_FIELD_TYPE ordblks;  /* number of free chunks */
	MALLINFO_FIELD_TYPE smblks;   /* always 0 */
	MALLINFO_FIELD_TYPE hblks;    /* always 0 */
	MALLINFO_FIELD_TYPE hblkhd;   /* space in mmapped regions */
	MALLINFO_FIELD_TYPE usmblks;  /* maximum total allocated space */
	MALLINFO_FIELD_TYPE fsmblks;  /* always 0 */
	MALLINFO_FIELD_TYPE uordblks; /* total allocated space */
	MALLINFO_FIELD_TYPE fordblks; /* total free space */
	MALLINFO_FIELD_TYPE keepcost; /* releasable (via malloc_trim) space */
};

extern mspace create_mspace_with_base(void *base, size_t capacity, int locked);
extern size_t destroy_mspace(mspace msp);
extern void *mspace_malloc(mspace msp, size_t bytes);
extern void *mspace_memalign(mspace msp, size_t alignment, size_t bytes);
extern void mspace_free(mspace msp, void *mem);
extern void mspace_malloc_stats(mspace msp);
extern size_t mspace_usable_size(const void *mem);
extern struct mallinfo mspace_mallinfo(mspace msp);
extern size_t mspace_footprint(mspace msp);

// used in wrapper
#define ALIGNMENT_4K 0x1000
#define NEED_DL_RESTORE 0

// chunkmem_node struct
struct chunkmem_node {
	struct list_head list;
	unsigned int va;
	unsigned int real_size;
	unsigned int aligned_size;
	int id;
	int refcnt;
};

// chunkmem_node list head
LIST_HEAD(chunkmem_node_list);
static DEFINE_MUTEX(chunkmem_node_mutex);

// support multiple bank
struct bank_info {
	mspace ms;
	void *vbase;
	UINT32 space_size;
};

#define NUM_OF_BANK 1
struct bank_info bank[NUM_OF_BANK] = {{0}};
int bank_idx = 0;

// dl restore
#if NEED_DL_RESTORE
struct dl_restore {
	struct list_head list;
	unsigned int start;
	unsigned int end;
};

LIST_HEAD(dl_restore_list);
static DEFINE_MUTEX(dl_restore_mutex);

// extern from gp_chunkmem
extern unsigned int gp_chunk_pa(void *ka);

// debug log for dl restore
#define DEBUG_DL_RESTORE 0

#endif // NEED_DL_RESTORE

// debug log
#define DEBUG_LOG 0

/*
 * check if @va is in current bank, which is selected by
 * dlRebase().
 * return 1 means @va is in current bank, otherwise not
 * in current bank.
 */
int va_in_current_bank(void *va)
{
	int i;

	for (i = 0; i < NUM_OF_BANK; i++) {
#if DEBUG_LOG
		printk(KERN_DEBUG "bank=%d ms=%08x %08x-%08x va=%08x\n",
		       i, (unsigned int)bank[i].ms, (unsigned int)bank[i].vbase,
		       (unsigned int)bank[i].vbase + bank[i].space_size, (unsigned int)va);
#endif
		if (bank[i].ms == g_ms) {
			if (va >= bank[i].vbase && va < (bank[i].vbase + bank[i].space_size))
				return 1;
		}
	}
	return 0;
}

/*
 * @tbase is vbase of whole memory. no return.
 *
 * rebase and handlers should be protected by
 * &chunkmem->sem. If any new feature, take care
 * of this.
 */
void dlRebase(void *tbase)
{
	int i;
	for (i = 0; i < NUM_OF_BANK; i++) {
		if (bank[i].vbase == tbase) {
			g_ms = bank[i].ms;
#if DEBUG_LOG
			printk(KERN_INFO "[dlwrap] g_ms switch to %d vbase=%08x\n",
                               i, (unsigned int)bank[i].vbase);
#endif
			return;
		}
	}
	printk(KERN_WARNING "%s: warning! can not found %08x\n", __FUNCTION__, (unsigned int)tbase);
}

/*
 * @vbase and @space_size are from ioremap, and @page_size is not used.
 * return 0 for successful.
 */
SINT32 dlMallocInit(void *vbase, UINT32 space_size, UINT32 page_size)
{
	if (bank_idx >= NUM_OF_BANK) {
		printk(KERN_ERR "too many calls to %s\n", __FUNCTION__);
		return -1;
	}
	if (bank[bank_idx].ms)
		return -1;
	bank[bank_idx].vbase = vbase;
	bank[bank_idx].space_size = space_size;
	bank[bank_idx].ms = create_mspace_with_base(vbase, space_size, 0);
	if (!bank[bank_idx].ms)
		return -1;
#if DEBUG_LOG
	printk(KERN_DEBUG "[dlwrap] created mspace[%d] vbase=%08x size=%08x\n",
	       bank_idx, (unsigned int)bank[bank_idx].vbase, bank[bank_idx].space_size);
#endif
	g_ms = bank[bank_idx].ms;
	bank_idx++;

	return 0;
}

/*
 * @info is a output parameter, containing capability infomation.
 * no return.
 */
void dlMalloc_Status(mem_info_t *info)
{
	if (!info) {
		mspace_malloc_stats(g_ms);
	} else {
		struct mallinfo m_info_p;
		m_info_p = mspace_mallinfo(g_ms);

		info->total_bytes = mspace_footprint(g_ms);
		info->used_bytes = m_info_p.uordblks;
		info->max_free_block_bytes = m_info_p.fordblks;
	}
}

/* @mem is start va of a mchunk. return usable size of this mchunk. */
UINT32 dlMalloc_Usable_Size(void *mem)
{
	return mspace_usable_size(mem);
}

/* test handler */
void *dlMalloc_Check(void *tbase, void *addr, UINT32 *pSize)
{
	// dummy, return 0 means failed
	return 0;
}

/*
 * When process(pid=@id) is closed, free all of used chunk.
 * return number of free memory.
 */
SINT32 dlFreeAll(UINT32 id)
{
	struct chunkmem_node *tmp, *tmp2;
	struct chunkmem_node *free_ptr;
	int count = 0;

	mutex_lock(&chunkmem_node_mutex);
	list_for_each_entry_safe(tmp, tmp2, &chunkmem_node_list, list) {
		// only check current bank, or g_ms will be incorrect.
		if (tmp->id == id && va_in_current_bank((void *)tmp->va)) {
			tmp->refcnt--;
			if (tmp->refcnt >= 1) {
				// still shared
			} else {
				free_ptr = tmp;
				list_del(&tmp->list);
				kfree(free_ptr);
#if DEBUG_LOG
				printk(KERN_DEBUG "[dlwrap] freeing addr=%08x size=%08x count=%d\n",
				       tmp->va, tmp->aligned_size, count);
#endif
				mspace_free(g_ms, (void *)tmp->va);
				count++;
			}
		}
	}
	mutex_unlock(&chunkmem_node_mutex);

	return count;
}

/* free chunk of @addr. alwasy return 0. */
SINT32 dlFree(void *addr)
{
	int hit = 0;
	int shared = 0;
	struct chunkmem_node *tmp, *tmp2;
	struct chunkmem_node *free_ptr;
#if DEBUG_LOG
	unsigned int log_addr = 0;
	unsigned int log_size = 0;
#endif
	mutex_lock(&chunkmem_node_mutex);
	list_for_each_entry_safe(tmp, tmp2, &chunkmem_node_list, list) {
		if (tmp->va == (unsigned int)addr) {
			if (hit != 0)
				printk(KERN_WARNING "%s: warning! the same addr %08x in list\n",
				       __FUNCTION__, (unsigned int)addr);
			hit = 1;
			tmp->refcnt--;
			if (tmp->refcnt >= 1) {
				shared = 1;
			} else {
				free_ptr = tmp;
				list_del(&tmp->list);
#if DEBUG_LOG
				log_addr = free_ptr->va;
				log_size = free_ptr->aligned_size;
#endif
				kfree(free_ptr);
			}
		}
	}
	mutex_unlock(&chunkmem_node_mutex);

	// if addr is not an allocated addr, do not free it.
	// if addr is shared with others, do not free it.
	if (hit == 1) {
		if (shared == 0) {
			mspace_free(g_ms, addr);
#if DEBUG_LOG
			printk(KERN_DEBUG "[dlwrap] freed addr=%08x size=%08x\n", log_addr, log_size);
#endif
		}
	} else {
		printk(KERN_WARNING "%s: warning! got a wrong addr %08x\n", __FUNCTION__, (unsigned int)addr);
	}
	return 0; // no return from mspace_free. If error, abort.
}

/*
 * alloc @size chunk for @id.
 * return addr of chunk. if no memory, return NULL.
 */
void *dlMalloc(UINT32 id, UINT32 size)
{
	void *mem = NULL;
	struct chunkmem_node *node = NULL;

	node = kmalloc(sizeof(struct chunkmem_node), GFP_KERNEL);
	if (!node) {
		return NULL;
	}
	memset(node, 0, sizeof(struct chunkmem_node));
	// use mspace_memalign(), we can make start address aligned.
	// but end address still do not match 4K-aligned.
	// so we need to handle size, extend size as aligned range.
	node->real_size = size;
	node->aligned_size = (size + (ALIGNMENT_4K - 1)) & ~(ALIGNMENT_4K - 1);
	node->id = id;
	node->refcnt = 1;

	mem = mspace_memalign(g_ms, ALIGNMENT_4K, node->aligned_size);

	if (!mem) {
		// if dlmalloc have no memory,
		// free node and return mem = NULL.
		kfree(node);
	} else {
		node->va = (unsigned int)mem;
#if DEBUG_LOG
		printk(KERN_DEBUG "[dlwrap] allocated addr=%08x rsize=%08x asize=%08x\n",
		       node->va, node->real_size, node->aligned_size);
#endif
		mutex_lock(&chunkmem_node_mutex);
		list_add_tail(&node->list, &chunkmem_node_list);
		mutex_unlock(&chunkmem_node_mutex);
	}
	return mem;
}

#if NEED_DL_RESTORE
/*
 * assign node and add to list .
 * return 0 means success, otherwise error.
 */
int dl_restore_new_node(void *start, unsigned int size)
{
	struct dl_restore *node = NULL;

	node = kmalloc(sizeof(struct dl_restore), GFP_KERNEL);
	if (!node) {
		return -1;
	}

	node->start = (unsigned int)start;
	node->end = (unsigned int)start + size;

	mutex_lock(&dl_restore_mutex);
#if DEBUG_DL_RESTORE
	printk(KERN_DEBUG "new %08x-%08x to list\n", node->start, node->end);
#endif
	list_add_tail(&node->list, &dl_restore_list);
	mutex_unlock(&dl_restore_mutex);
	return 0;
}

/* for showing dl restore list */
void show_dl_restore_list(void)
{
	struct dl_restore *node = NULL;

	mutex_lock(&dl_restore_mutex);
	list_for_each_entry(node, &dl_restore_list, list) {
		printk(KERN_DEBUG "%s: %08x-%08x\n", __FUNCTION__, node->start, node->end);
	}
	mutex_unlock(&dl_restore_mutex);
}

/*
 * @start and @size are from allocated nodes, compare them
 * with restored nodes. If overlapped, merge range. If they
 * are independent nodes, create new node and add it to list.
 * return 0 means success otherwise error.
 */
int dl_restore_merge_or_new(unsigned int start, unsigned int size)
{
	struct dl_restore *tmp, *tmp2;
	unsigned int end = start + size;
	struct dl_restore *node = NULL;
	int merge = 0;

#if DEBUG_DL_RESTORE
	printk(KERN_DEBUG "checking %08x-%08x\n", start, end);
#endif
	// extend range as start-0x1000 to end+0x1000
	// for dlmalloc data struct
	if (start <= ALIGNMENT_4K)
		start = 0x0;
	else
		start -= ALIGNMENT_4K;
	end += ALIGNMENT_4K;

	mutex_lock(&dl_restore_mutex);
	list_for_each_entry_safe(tmp, tmp2, &dl_restore_list, list) {
		if ((start >= tmp->start && start <= tmp->end)
		    || (end >= tmp->start && end <= tmp->end)) {
#if DEBUG_DL_RESTORE
			printk(KERN_DEBUG "node %08x-%08x ", tmp->start, tmp->end);
#endif
			if (start < tmp->start)
				tmp->start = start;
			else if (end > tmp->end)
				tmp->end = end;
#if DEBUG_DL_RESTORE
			printk(KERN_DEBUG "updated as %08x-%08x\n", tmp->start, tmp->end);
#endif
			merge = 1;
		} else if (unlikely(start < tmp->start && end > tmp->end)) {
#if DEBUG_DL_RESTORE
			printk(KERN_DEBUG "node %08x-%08x ", tmp->start, tmp->end);
#endif
			tmp->start = start;
			tmp->end = end;
#if DEBUG_DL_RESTORE
			printk(KERN_DEBUG "updated as %08x-%08x\n", tmp->start, tmp->end);
#endif
			merge = 1;
		}
	} // for list dl_restore_list

	if (!merge) {
		node = kmalloc(sizeof(struct dl_restore), GFP_KERNEL);
		if (!node) {
			mutex_unlock(&dl_restore_mutex);
			return -1;
		}
		node->start = start;
		node->end = end;
#if DEBUG_DL_RESTORE
		printk(KERN_DEBUG "adding %08x-%08x to list\n", node->start, node->end);
#endif
		list_add_tail(&node->list, &dl_restore_list);
	}
	mutex_unlock(&dl_restore_mutex);

	return 0;
}

/*
 * @bank_base and @bank_size are output parameter
 * filled by global bank[current] array.
 */
void get_bank_range(unsigned int *bank_base, unsigned int *bank_size)
{
	int i;
	// get current bank range
	for (i = 0; i < NUM_OF_BANK; i++) {
#if DEBUG_DL_RESTORE
		printk(KERN_DEBUG "bank=%d ms=%08x %08x-%08x\n", i, (unsigned int)bank[i].ms,
		       (unsigned int)bank[i].vbase, (unsigned int)bank[i].vbase + bank[i].space_size);
#endif
		if (bank[i].ms == g_ms) {
			*bank_base = (unsigned int)bank[i].vbase;
			*bank_size = bank[i].space_size;
		}
	}
}

/*
 * dlChunkSuspend() will be called multiple times for
 * multiple bank so this handler will be too.
 * Handle one bank by one calling. Before calling,
 * there should be rebase.
 * return 0 means success, otherwise eror.
 */
int create_dl_restore_list(void)
{
	unsigned int bank_base = 0, bank_size = 0;
	struct chunkmem_node *node = NULL;
	int ret = 0;

	// get current bank range
	get_bank_range(&bank_base, &bank_size);
#if DEBUG_DL_RESTORE
	printk(KERN_DEBUG "current bank range %08x-%08x\n", bank_base, (bank_base + bank_size));
#endif

	// restore first alignment and last 2 alignment
	if (mspace_footprint(g_ms) <= ALIGNMENT_4K * 3) {
		ret = dl_restore_new_node((void *)bank_base, bank_size);
		if (ret) {
			printk(KERN_WARNING "warning dl_restore_new_node return %d\n", ret);
			return -1;
		}
		return 0;
	} else {
		ret = dl_restore_new_node((void *)bank_base, ALIGNMENT_4K);
		if (ret) {
			printk(KERN_WARNING "warning dl_restore_new_node return %d\n", ret);
			return -1;
		}

		ret = dl_restore_new_node((void *)(bank_base + mspace_footprint(g_ms) - (ALIGNMENT_4K * 2)), ALIGNMENT_4K * 2);
		if (ret) {
			printk(KERN_WARNING "warning dl_restore_new_node return %d\n", ret);
			return -1;
		}
	}

	// restore each dl slot, merge if necessary
	mutex_lock(&chunkmem_node_mutex);
	list_for_each_entry(node, &chunkmem_node_list, list) {
		if(va_in_current_bank((void *)node->va)) {
			ret = dl_restore_merge_or_new(node->va, node->aligned_size);
			if(ret) {
				printk(KERN_WARNING "warning dl_restore_merge_or_new return %d\n", ret);
				mutex_unlock(&chunkmem_node_mutex);
				return -1;
			}
		}
	}
	mutex_unlock(&chunkmem_node_mutex);

	return 0;
}

/* return pa for @va */
unsigned int get_pa_by_va(void *va)
{
	return gp_chunk_pa(va);
}

/*
 * @save_proc is function pointer which is for low
 * level restoring. Give it physical start address
 * and size.
 */
int dl_restore_save_proc(save_data_proc save_proc)
{
	struct dl_restore *node = NULL;
	unsigned int pa = 0, size = 0;
	unsigned int bank_base = 0, bank_size = 0;

	// get current bank range
	get_bank_range(&bank_base, &bank_size);

	mutex_lock(&dl_restore_mutex);
	list_for_each_entry(node, &dl_restore_list, list) {
		if (va_in_current_bank((void*)node->start)) {
			pa = get_pa_by_va((void*)node->start);
			size = node->end - node->start + 0x10; // 0x10 is header
#if DEBUG_DL_RESTORE
			printk(KERN_DEBUG "%s: %08x-%08x param=%08x-%08x\n",
			       __FUNCTION__, node->start, node->end, pa, pa+size);
#endif
			save_proc(pa, size);
		}
	}
	mutex_unlock(&dl_restore_mutex);

	return 0;
}

/* used by kernel/power/warp.c to store chunk data struct. */
int dlChunkSuspend(save_data_proc save_proc)
{
	int ret;
	ret = create_dl_restore_list();
	if (ret) {
		printk(KERN_WARNING "warning ret %d from create_dl_restore_list\n", ret);
		return ret;
	}

	dl_restore_save_proc(save_proc);

	return 0;
}
#else
int dlChunkSuspend(save_data_proc save_proc)
{
	// dummy, return 0 means success
	return 0;
}
#endif // NEED_DL_RESTORE

/* @addr is start vaddr of a chunk. tag is string pattern. always return 0. */
SINT32 dlSetTag(void *addr, char *tag)
{
	// dummy, return 0 means success
	return 0;
}

/*
 * @tbase is vbase of whole memory. @addr is va of a chunk.
 * return reference count.
 */
UINT32 dlShare(void *tbase, void *addr)
{
	struct chunkmem_node *node;
	int refcnt = -1;

	mutex_lock(&chunkmem_node_mutex);
	list_for_each_entry(node, &chunkmem_node_list, list) {
		if (node->va == (unsigned int)addr) {
			node->refcnt++;
			refcnt = node->refcnt;
		}
	}
	mutex_unlock(&chunkmem_node_mutex);

	return refcnt;
}

/* chunkmem to dlmalloc interface end */
