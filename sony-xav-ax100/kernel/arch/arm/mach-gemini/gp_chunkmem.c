/**
 * @file    gp_chunkmem.c
 * @brief   Implement of GP chunk memory driver.
 * @author  qinjian
 * @since   2010-09-01
 * @date    2010-09-01
 */
/* #define DIAG_LEVEL DIAG_LVL_VERB */
#include <mach/kernel.h>
#include <mach/module.h>
#include <mach/cdev.h>
#include <mach/diag.h>
#include <mach/dlmalloc.h>
#include <mach/gp_chunkmem.h>
#include <mach/hal/regmap/reg_system.h>	// sp_get_bank_num
#include <mach/sp_reservemem.h>		// for res_ variables

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

#define MAX_OPEN_PROCESS	50

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/

#define USE_DLMALLOC_EX		0
#define CHUNK_SUSPEND_TEST	0
#define DEBUG_ALLOC_FREE	0
//#define DOWN_INTERRUPTIBLE	down_interruptible
#define DOWN_INTERRUPTIBLE	down_killable

#define CHUNKMEM_REBASE(pool)	dlRebase((pool)->vbase)

#define PROT_PTE_DEVICE		L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY | L_PTE_XN
#define L_PTE_SHARED		(_AT(pteval_t, 1) << 10)

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

typedef struct open_process_s {
	unsigned int pid;		/*!< @brief process id */
	unsigned int count;		/*!< @brief open count in this process */
} open_process_t;


typedef struct chunkmem_info_s {
	struct miscdevice dev;		/*!< @brief chunkmem device */

	unsigned int mmap_enable;	/*!< @brief mmap flag, used to disable calling mmap from user AP */
	unsigned int mmap_nocache;	/*!< @brief mmap nocache flag, used to disable cache mmap from user AP */
	struct semaphore sem;		/*!< @brief mutex semaphore for mem ops */

	open_process_t opens[MAX_OPEN_PROCESS]; /*!< @brief all process open the device */
	unsigned int opens_count;	/*!< @brief open process count */
	struct semaphore opens_sem;	/*!< @brief mutex semaphore for opens */
} chunkmem_info_t;

typedef struct chunkmem_pool_s {
	unsigned int pbase;		/*!< @brief start address of the chunkmem region, phy_addr */
	unsigned int size;		/*!< @brief size of the chunkmem region */
	void __iomem *vbase;		/*!< @brief start address of the remaped chunkmem region, kernel_addr */
	struct resource *mem;		/*!< @brief memory resource to hold chunkmem region */
} chunkmem_pool_t;

/**************************************************************************
 *                 E X T E R N A L    R E F E R E N C E S                 *
 **************************************************************************/

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/

static chunkmem_info_t *chunkmem = NULL;

static chunkmem_pool_t *chunkmem_pools;
static unsigned int chunkmem_pool_count = 0;

/* These five global variables are all from kernel parameter, for exmaple :
 * memnr    : gp_chunkmem.memnr=2
 * membase1 : gp_chunkmem.membase1=0x32000000
 * memsize1 : gp_chunkmem.memsize1=0x0df00000
 * membase2 : gp_chunkmem.membase2=0x1d200000
 * memsize2 : gp_chunkmem.memsize2=0x02e00000
 * still keep kernel parameter for backward compatibility */
unsigned int memnr = -1;
unsigned int membase1 = -1;
unsigned int memsize1 = -1;
unsigned int membase2 = -1;
unsigned int memsize2 = -1;
module_param(memnr, uint, S_IRUGO);
module_param(membase1, uint, S_IRUGO);
module_param(memsize1, uint, S_IRUGO);
module_param(membase2, uint, S_IRUGO);
module_param(memsize2, uint, S_IRUGO);

/* for backward compatibility */
static unsigned int membase = 0;
static unsigned int memsize = 0;
module_param(membase, uint, S_IRUGO);
module_param(memsize, uint, S_IRUGO);

#ifdef GP_SYNC_OPTION
/**
 * @brief   Sync cache
 * @return  None
 * @see
 */
void gp_sync_cache(void)
{
	unsigned long oldIrq;

	local_irq_save(oldIrq);
	flush_cache_all();
	local_irq_restore(oldIrq);
}
EXPORT_SYMBOL(gp_sync_cache);
#else
/**
 * @brief   Sync cache
 * @return  None
 * @see
 */
void gp_sync_cache_all(void)
{
	unsigned long oldIrq;

	local_irq_save(oldIrq);
	__cpuc_flush_kern_all();
	local_irq_restore(oldIrq);
}
EXPORT_SYMBOL(gp_sync_cache_all);
#endif

/**
 * @brief   Get chunkmem number
 */
unsigned int gp_chunk_num(void)
{
	return chunkmem_pool_count;
}
EXPORT_SYMBOL(gp_chunk_num);

/**
 * @brief   Get chunkmem base address & size
 */
void gp_chunk_info(unsigned int pool_idx, unsigned int *base, unsigned int *size)
{
	if (pool_idx < chunkmem_pool_count) {
		*base = (unsigned int)chunkmem_pools[pool_idx].vbase;
		*size = chunkmem_pools[pool_idx].size;
	}
}
EXPORT_SYMBOL(gp_chunk_info);

static int set_page_attr(const void *va, pte_t attr)
{
	unsigned long addr = (unsigned long)va;
	pgd_t *pgd = pgd_offset_k(addr);

	if (!pgd_none(*pgd)) {
		pud_t *pud = pud_offset(pgd, addr);
		if (!pud_none(*pud)) {
			pmd_t *pmd = pmd_offset(pud, addr);
			if (!pmd_none(*pmd)) {
				pte_t *ptep;

				ptep = pte_offset_map(pmd, addr);
				if (pte_present(*ptep)) {
					set_pte_at(&init_mm, addr, ptep, pfn_pte(*ptep >> PAGE_SHIFT, attr));
					pte_unmap(ptep);
					return 0;
				}
				pte_unmap(ptep);
			}
		}
	}

	return 1;
}

extern int dlChunkSuspend(save_data_proc save_proc);

int gp_chunk_suspend(save_data_proc save_proc)
{
	int i = 0;

	down(&chunkmem->sem);
	while (i < chunkmem_pool_count) {
		CHUNKMEM_REBASE(&chunkmem_pools[i]);
		dlChunkSuspend(save_proc);
		i++;
	}
	up(&chunkmem->sem);

	return 0;
}
EXPORT_SYMBOL(gp_chunk_suspend);

void *gp_chunk_malloc_cache(unsigned int pool_idx, unsigned int id, unsigned int size)
{
	void *va = NULL;
	pte_t pte = 0;
	int i;
	int ret;

	va = gp_chunk_malloc_nocache(pool_idx, id, size);
	if (va == NULL)
		return NULL;

	pte |= L_PTE_MT_DEV_SHARED;
	pte |= PROT_PTE_DEVICE;
	pte |= L_PTE_SHARED;

	for (i = 0; i < size; i += PAGE_SIZE) {
		ret = set_page_attr(va + i, pte);
		if (ret)
			return NULL;
	}

	return va;
}
EXPORT_SYMBOL(gp_chunk_malloc_cache);

/**
 * @brief   Allocate memory block from chunkmem.
 * @param   id [in] ownerID of the chunkmem block.
 * @param   size [in] size of the chunkmem block.
 * @return	success: start address of allocated chunkmem block,
 *  		fail: NULL
 * @see 	dlMalloc
 */
void* gp_chunk_malloc_nocache(unsigned int pool_idx, unsigned int id, unsigned int size)
{
	void *ret = NULL;

	if (pool_idx >= chunkmem_pool_count) {
		printk(KERN_WARNING "pool_idx(%d) out of range, forced to pool 0!\n", pool_idx);
		pool_idx = 0;
	}

	if ((chunkmem != NULL) && (size != 0) && (pool_idx < chunkmem_pool_count)) {
#ifdef DOWN_INTERRUPTIBLE
		if (DOWN_INTERRUPTIBLE(&chunkmem->sem) != 0) {
			DIAG_ERROR("%s: acquire sem interrupted!\n", __FUNCTION__);
		}
		else {
#else
		{
			down(&chunkmem->sem);
#endif
			CHUNKMEM_REBASE(&chunkmem_pools[pool_idx]);

#if DEBUG_ALLOC_FREE
			DIAG_DEBUG("++++++++++++++++++++++++ ALLOC ++++++++++++++++++++++++++\n");
			DIAG_DEBUG("!!!!size = %08X\n", size);
			dlMalloc_Status(NULL);
#endif
#if USE_DLMALLOC_EX
			ret = dlMallocEx(id, size);
#else
			ret = dlMalloc(id, size);
#endif
#if DEBUG_ALLOC_FREE
			dlMalloc_Status(NULL);
			DIAG_DEBUG("------------------------ ALLOC --------------------------\n");
#else
			if (ret == NULL) {
				DIAG_ERROR("[gp_chunk_malloc_nocache] out of memory! (%s:%08X)\n", current->comm, size);
				dlMalloc_Status(NULL);
			}
#endif
			up(&chunkmem->sem);
		}
	}

	return ret;
}
EXPORT_SYMBOL(gp_chunk_malloc_nocache);

/**
 * @brief 	Find the addr belong which chunkmem pool.
 * @param 	ka [in] kernel_addr.
 * @return  chunkmem pool, fail: NULL
 */
chunkmem_pool_t *chunk_pool(void *ka)
{
	chunkmem_pool_t *pool = chunkmem_pools;

	while (pool->pbase != 0) {
		unsigned int i = ka - pool->vbase;
		if (i < pool->size) {
			return pool;
		}
		pool++;
	}

	return NULL;
}

/**
 * @brief 	Find the addr belong which chunkmem pool.
 * @param 	ka [in] kernel_addr.
 * @return  chunkmem pool index, fail: -1
 */
int chunk_pool_idx(void *ka)
{
	chunkmem_pool_t *pool = chunkmem_pools;
	int idx = 0;

	while (pool->pbase != 0) {
		unsigned int i = ka - pool->vbase;
		if (i < pool->size) {
			return idx;
		}
		pool++;
		idx++;
	}

	return -1;
}
EXPORT_SYMBOL(chunk_pool_idx);

/**
 * @brief 	Free chunkmem block.
 * @param 	addr [in] start address of chunkmem block to free,
 *  			kernel_addr.
 * @return  None
 * @see 	dlFree
 */
void gp_chunk_free(void *addr)
{
	if (addr == NULL) {
		DIAG_WARN("%s: WARNING!!! free NULL pointer!\n", __FUNCTION__);
		return;
	}

	if (chunkmem != NULL) {
#ifdef DOWN_INTERRUPTIBLE
		if (DOWN_INTERRUPTIBLE(&chunkmem->sem) != 0) {
			DIAG_ERROR("%s: acquire sem interrupted!\n", __FUNCTION__);
		}
		else {
#else
		{
			down(&chunkmem->sem);
#endif
			chunkmem_pool_t *pool = chunk_pool(addr);
			if (pool == NULL) return;
			CHUNKMEM_REBASE(pool);

#if DEBUG_ALLOC_FREE
			DIAG_DEBUG("++++++++++++++++++++++++ FREE ++++++++++++++++++++++++++\n");
			DIAG_DEBUG("!!!!addr = %08X\n", (unsigned long)addr);
			dlMalloc_Status(NULL);
#endif
#if (DIAG_LEVEL >= DIAG_LVL_VERB) && !defined(DIAG_VERB_OFF)
			int ret = dlFree(addr);
#else
			dlFree(addr);
#endif
#if DEBUG_ALLOC_FREE
			dlMalloc_Status(NULL);
			DIAG_DEBUG("------------------------ FREE --------------------------\n");
#endif
			up(&chunkmem->sem);
			DIAG_VERB("dlFree: %d\n", ret);
		}
	}
}
EXPORT_SYMBOL(gp_chunk_free);

void gp_chunk_free_cache(void *addr, int size)
{
	pte_t pte = 0;
	int i;

	gp_chunk_free(addr);

	pte &= PAGE_MASK;
	pte |= L_PTE_MT_DEV_CACHED;
	pte |= PROT_PTE_DEVICE;
	pte |= L_PTE_SHARED;

	for(i = 0; i < size; i += PAGE_SIZE) {
		set_page_attr(addr + i, pte);
	}
}
EXPORT_SYMBOL(gp_chunk_free_cache);

void gp_chunk_show(void)
{
	if (chunkmem != NULL) {
#ifdef DOWN_INTERRUPTIBLE
		if (DOWN_INTERRUPTIBLE(&chunkmem->sem) != 0) {
			DIAG_ERROR("%s: acquire sem interrupted!\n", __FUNCTION__);
		}
		else {
#else
		{
			down(&chunkmem->sem);
#endif
			chunkmem_pool_t *pool = chunkmem_pools;
			while (pool->pbase != 0) {
				CHUNKMEM_REBASE(pool);
				dlMalloc_Status(NULL);
				pool++;
			}
			up(&chunkmem->sem);
		}
	}
}
EXPORT_SYMBOL(gp_chunk_show);

/**
 * @brief   Free chunkmem blocks by ownerID.
 * @param   id [in] ownerID of the chunkmem blocks to free.
 * @return  None
 * @see     dlFreeAll
 */
void gp_chunk_free_all(unsigned int id)
{
	if (chunkmem != NULL) {
#ifdef DOWN_INTERRUPTIBLE
		if (DOWN_INTERRUPTIBLE(&chunkmem->sem) != 0) {
			DIAG_ERROR("%s: acquire sem interrupted!\n", __FUNCTION__);
		}
		else {
#else
		{
			down(&chunkmem->sem);
#endif
			chunkmem_pool_t *pool = chunkmem_pools;
			while (pool->pbase != 0) {
				CHUNKMEM_REBASE(pool);
#if (DIAG_LEVEL >= DIAG_LVL_VERB) && !defined(DIAG_VERB_OFF)
				int ret = dlFreeAll(id);
#else
				dlFreeAll(id);
#endif
				pool++;
			}
			up(&chunkmem->sem);
			DIAG_VERB("dlFreeAll: %d\n", ret);
		}
	}
}
EXPORT_SYMBOL(gp_chunk_free_all);

/**
 * @brief   Translate chunkmem kernel virtual address to physical address.
 * @param   va [in] kernel virtual address.
 * @return  success: physical address, fail: 0
 * @see
 */
unsigned int gp_chunk_pa(void *ka)
{
	chunkmem_pool_t *pool = chunkmem_pools;

	while (pool->pbase != 0) {
		unsigned int i = ka - pool->vbase;
		if (i < pool->size) {
			return (pool->pbase + i);
		}
		pool++;
	}

	return 0; /* ka not in chunkmem region */
}
EXPORT_SYMBOL(gp_chunk_pa);

/**
 * @brief   Translate physical address to chunkmem kernel virtual address.
 * @param   pa [in] physical address.
 * @return  success: kernel virtual address, fail: NULL
 * @see     CHUNK_MEM_FREE
 */
void *gp_chunk_va(unsigned int pa)
{
	chunkmem_pool_t *pool = chunkmem_pools;

	while (pool->pbase != 0) {
		unsigned int i = pa - pool->pbase;
		if (i < pool->size) {
			return (pool->vbase + i);
		}
		pool++;
	}

	return NULL; /* pa not in chunkmem region */
}
EXPORT_SYMBOL(gp_chunk_va);

/**
 * @brief   Check whether ka is belong to a allocated chunkmem block.
 * @param   ka [in] kernel virtual address.
 * @param   pSize [out] if true, return chunk block size.
 * @return  success: start address of the chunkmem block , fail: NULL
 * @see     CHUNK_MEM_CHECK
 */
void *gp_chunk_check(void *ka, unsigned int *pSize)
{
	down(&chunkmem->sem);
	ka = dlMalloc_Check(chunk_pool(ka)->vbase, ka, pSize);
	up(&chunkmem->sem);

	return ka;
}
EXPORT_SYMBOL(gp_chunk_check);

/**
 * @brief   Translate user virtual address to physical address.
 * @param   va [in] user virtual address.
 * @return 	success: physical address, fail: 0
 * @see     CHUNK_MEM_FREE
 */
unsigned int gp_user_va_to_pa(void *va)
{
	pgd_t *pgd = NULL;
	pud_t *pud = NULL;
	pmd_t *pmd = NULL;
	pte_t *pte = NULL;
	struct mm_struct *mm = current->mm;
	unsigned int addr = (unsigned int)va;
	unsigned int pa = 0;

	down_read(&mm->mmap_sem);

	/* query page tables */
	if (!find_vma(mm, addr)) {
		DIAG_VERB("virt_addr %08X not available.\n", addr);
		goto out;
	}
	pgd = pgd_offset(mm, addr);
	if (pgd_none(*pgd)) {
		DIAG_VERB("Not mapped in pgd.\n");
		goto out;
	}
	pud = pud_offset(pgd, addr);
	if (pud_none(*pud)) {
		DIAG_VERB("Not mapped in pud.\n");
		goto out;
	}
	pmd = pmd_offset(pud, addr);
	if (pmd_none(*pmd)) {
		DIAG_VERB("Not mapped in pmd.\n");
		goto out;
	}
	pte = pte_offset_kernel(pmd, addr);
	if (pte_none(*pte)) {
		DIAG_VERB("Not mapped in pte.\n");
		goto out;
	}
	if (!pte_present(*pte)) {
		DIAG_VERB("pte not in RAM.\n");
		goto out;
	}

	pa = (pte_val(*pte) & PAGE_MASK) | (addr & ~PAGE_MASK);

out:
	up_read(&mm->mmap_sem);
	return pa;
}
EXPORT_SYMBOL(gp_user_va_to_pa);

/**
 * @brief   Query bank number of physic address
 */
int sp_get_bank_num(u32 pa)
{
	void __iomem *regs = (void __iomem *)SYSTEM_BASE;
	uint32_t cfg7 = readl(regs + 0x9C);
	u32 bank0_start = PHYS_OFFSET;
	u32 bank0_end = bank0_start + (1 << (27 + ((cfg7 >> 21) & 3)));

	if (pa >= bank0_start && pa < bank0_end) {
		/* return bank 0 */
		return 0;
	}

	if (!(readl(regs + 0x1888) & 0xd00)) {
		u32 bank1_start = bank0_end + PHYS_OFFSET;
		u32 bank1_end = bank1_start + (1 << (27 + ((cfg7 >> 23) & 3)));

		if (pa >= bank1_start && pa < bank1_end) {
			/* return bank 1 */
			return 1;
		}
	}

	/* return error */
	return -1;
}

/**
 * @brief   Chunkmem device open function
 */
static int chunkmem_open(struct inode *inode, struct file *file)
{
	int ret = 0;
	unsigned int i;
	unsigned int pid = current->tgid;
	open_process_t *opens = chunkmem->opens;

#ifdef DOWN_INTERRUPTIBLE
	if (DOWN_INTERRUPTIBLE(&chunkmem->opens_sem) != 0) {
		return -ERESTARTSYS;
	}
#else
	down(&chunkmem->opens_sem);
#endif

	i = chunkmem->opens_count;
	if (i == MAX_OPEN_PROCESS) {
		DIAG_ERROR("chunkmem can't be opened by more than %d processes!\n", MAX_OPEN_PROCESS);
		ret = -EBUSY;
		goto out;
	}

	while (i--) {
		if (opens[i].pid == pid) { /* alreay open */
			opens[i].count++;
			goto out;
		}
	}

	/* append current process to opens */
	i = chunkmem->opens_count;
	opens[i].pid = pid;
	opens[i].count = 1;
	chunkmem->opens_count++;

out:
	up(&chunkmem->opens_sem);
	DIAG_VERB("[chunkmem_open] opens_count=%d pid=%d count=%d\n", chunkmem->opens_count, pid, opens[i].count);

	return ret;
}

/**
 * @brief   Chunkmem device release function
 */
static int chunkmem_release(struct inode *inode, struct file *file)
{
	int ret = 0;
	int i;
	unsigned int pid = current->tgid;
	open_process_t *opens = chunkmem->opens;

	// DIAG_DEBUG("chunkmem_release: pid = %d\n", pid);
#ifdef DOWN_INTERRUPTIBLE
	if (DOWN_INTERRUPTIBLE(&chunkmem->opens_sem) != 0) {
		return -ERESTARTSYS;
	}
#else
	down(&chunkmem->opens_sem);
#endif

	i = chunkmem->opens_count;
	while (i--) {
		if (opens[i].pid == pid) { /* found */
			break;
		}
	}

	if (i < 0) {
		DIAG_ERROR("Not found current pid(%d) in opens!!!\n", pid);
	} else {
		opens[i].count--;
		DIAG_VERB("[chunkmem_release] opens_count=%d pid=%d count=%d\n", chunkmem->opens_count, pid, opens[i].count);
		if (opens[i].count == 0) {
			/* remove current process from opens */
			chunkmem->opens_count--;
			memcpy(&opens[i],
			       &opens[i + 1],
			       (chunkmem->opens_count - i) * sizeof(open_process_t));

			/* free all chunkmem blocks belong to current process */
			gp_chunk_free_all(pid);
		}
	}

	up(&chunkmem->opens_sem);
	return ret;
}

/**
 * @brief   Chunkmem device mmap function
 */
static int chunkmem_mmap(struct file *file, struct vm_area_struct *vma)
{
	int ret;

	if (!chunkmem->mmap_enable) {
		ret = -EPERM; /* disable calling mmap from user AP */
		goto out;
	}

	/* This is an IO map - tell maydump to skip this VMA */
	vma->vm_flags |= VM_IO | VM_RESERVED;
	// vma->vm_page_prot = pgprot_dmacoherent(vma->vm_page_prot); //non-cache & writebufferable
	ret = io_remap_pfn_range(vma,
				 vma->vm_start,
				 vma->vm_pgoff,
				 vma->vm_end - vma->vm_start,
				 chunkmem->mmap_nocache ? pgprot_noncached(vma->vm_page_prot) : vma->vm_page_prot);
	if (ret != 0) {
		ret = -EAGAIN;
	}
out:
	return ret;
}

#if CHUNK_SUSPEND_TEST
typedef struct data_block_s {
	void *addr;
	unsigned long size;
	struct data_block_s *next;
	unsigned char data[0];
} data_block_t;

data_block_t *blocks = NULL;

void my_save_data(unsigned long addr, unsigned long size)
{
	void *va;
	data_block_t *block = kmalloc(sizeof(data_block_t) + size, GFP_KERNEL);

	if (block == NULL) {
		DIAG_ERROR("save data error: out of memory! %p %08X\n", addr, size);
		return;
	}

	va = gp_chunk_va(addr);
	if (va == NULL) {
		va = __va(addr);
	}
	memcpy(&block->data, va, size);
	block->addr = va;
	DIAG_DEBUG("save data: %08X(%p) %08X\n", addr, va, size);
	block->size = size;
	block->next = blocks;
	blocks = block;
}
#endif

/**
 * @brief   Chunkmem device ioctl function
 */
static long chunkmem_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	chunk_block_t block;
	void *ka;		/* kernel_addr */
	unsigned int va;	/* user_addr */
	unsigned int pa;	/* phy_addr*/
	long ret = 0;
	unsigned int offset = 0;
	chunkmem_pool_t *pool = chunkmem_pools;

	switch (cmd) {
#ifdef CHUNK_MEM_CHECK
	case CHUNK_MEM_CHECK:
		if (copy_from_user(&block, (void __user*)arg, sizeof(block))) {
			ret = -EFAULT;
			break;
		}

		pa = gp_user_va_to_pa(block.addr);    /* user_addr to phy_addr */
		if (pa == 0) {
			DIAG_ERROR("CHUNK_MEM_CHECK: chunkmem user_va_to_pa fail! (%s:%08X)\n", current->comm, block.addr);
			ret = -EFAULT;
			break;
		}
		DIAG_VERB("pa = %08X\n", pa);
		ka = gp_chunk_va(pa);                  /* phy_addr to kernel_addr */
		if (ka == NULL) {
			DIAG_ERROR("CHUNK_MEM_CHECK: not a chunkmem address! (%s:%08X)\n", current->comm, pa);
			ret = -EFAULT;
			break;
		}

		ka = gp_chunk_check(ka, &block.size);
		if (ka == NULL) {
			DIAG_ERROR("CHUNK_MEM_CHECK: not found a allocated chunk! (%s:%08X)\n", current->comm, pa);
			ret = -EFAULT;
			break;
		}

		block.pool_idx = chunk_pool_idx(ka);
		block.phy_addr = gp_chunk_pa(ka);

		if (copy_to_user((void __user *)arg, &block, sizeof(block))) {
			ret = -EFAULT;
			break;
		}
		break;
#endif

	case CHUNK_MEM_ALLOC_CACHE:
	case CHUNK_MEM_SHARE:
	case CHUNK_MEM_MMAP:
	case CHUNK_MEM_ALLOC_NOCACHE:
		if (copy_from_user(&block, (void __user*)arg, sizeof(block))) {
			ret = -EFAULT;
			break;
		}

		/* alloc|share|mmap memory */
		if (cmd == CHUNK_MEM_MMAP) {
			DIAG_VERB("CHUNK_MEM_MMAP:\n");
			ka = gp_chunk_va(block.phy_addr);
			if (ka == NULL) {
				DIAG_ERROR("CHUNK_MEM_MMAP: bad address! (%s:%08X)\n", current->comm, block.phy_addr);
				ret = -EFAULT; /* mmap fail */
				break;
			}
			/* page alignment */
			offset = block.phy_addr & ~PAGE_MASK;
			ka = (void *)((unsigned long)ka & PAGE_MASK);
			DIAG_VERB("CHUNK_MEM_MMAP: phy_addr                  = %08X\n", block.phy_addr);
			DIAG_VERB("CHUNK_MEM_MMAP: size                      = %08X\n", block.size);
			DIAG_VERB("CHUNK_MEM_MMAP: ka                        = %08X\n", (unsigned int)ka);
			DIAG_VERB("CHUNK_MEM_MMAP: offset                    = %08X\n", offset);
			DIAG_VERB("CHUNK_MEM_MMAP: PAGE_ALIGN(size + offset) = %08X\n", PAGE_ALIGN(block.size + offset));
		} else {
			if (cmd == CHUNK_MEM_ALLOC_CACHE || cmd == CHUNK_MEM_ALLOC_NOCACHE) {
				DIAG_VERB("CHUNK_MEM_ALLOC_CACHE or CHUNK_MEM_ALLOC_NOCACHE:\n");
				DIAG_VERB("size = %08X (%d)\n", block.size, block.size);

				/* for backward compatibility: old code not set the pool_idx */
				if (block.pool_idx >= chunkmem_pool_count)
					block.pool_idx = 0;

				ka = gp_chunk_malloc_nocache(block.pool_idx, current->tgid, block.size);
				DIAG_VERB("gp_chunk_malloc_nocache return ka=%08X\n", ka);
				if (ka == NULL) {
					ret = -ENOMEM;
					break;
				}
				block.phy_addr = gp_chunk_pa(ka);
				dlSetTag(ka, block.tag);
			} else { /* CHUNK_MEM_SHARE */
				DIAG_VERB("CHUNK_MEM_SHARE:\n");
				ka = gp_chunk_va(block.phy_addr);
				if ((ka == NULL) || (dlShare(chunk_pool(ka)->vbase, ka) == 0)) {
					DIAG_ERROR("CHUNK_MEM_SHARE: bad address! (%s:%08X)\n", current->comm, block.phy_addr);
					ret = -EFAULT; /* share fail */
					break;
				}
			}
			block.size = dlMalloc_Usable_Size(ka) & PAGE_MASK; /* actual allocated size */
			DIAG_VERB("actual size = %08X (%d)\n", block.size, block.size);
			DIAG_VERB("ka = %08X\n", (unsigned int)ka);
		}
		pool = chunk_pool(ka);

		/* mmap to userspace */
		down(&chunkmem->sem);
		down_write(&current->mm->mmap_sem);
		chunkmem->mmap_enable = 1; /* enable mmap in CHUNK_MEM_ALLOC_CACHE or CHUNK_MEM_ALLOC_NOCACHE */
		if (cmd == CHUNK_MEM_ALLOC_NOCACHE) {
			DIAG_VERB("CHUNK_MEM_ALLOC_NOCACHE\n");
			chunkmem->mmap_nocache = 1; /* enable nocache mmap */
		}
		va = do_mmap(file, 0, PAGE_ALIGN(block.size + offset),
			     (file->f_mode & FMODE_WRITE) ? (PROT_READ | PROT_WRITE) : PROT_READ,
			     MAP_SHARED,
			     (pool->pbase + (ka - pool->vbase)));
		chunkmem->mmap_enable = 0; /* disable it */
		chunkmem->mmap_nocache = 0; /* disable nocache mmap */
		up_write(&current->mm->mmap_sem);
		up(&chunkmem->sem);
		if (IS_ERR_VALUE(va)) {
			ret = va; /* errcode */
			DIAG_ERROR("%s: chunkmem mmap fail(%d)! (%s)\n",
				   (cmd == CHUNK_MEM_MMAP) ? "CHUNK_MEM_MMAP" : ((cmd == CHUNK_MEM_ALLOC_CACHE) ? "CHUNK_MEM_ALLOC_CACHE" : "CHUNK_MEM_SHARE"),
				   ret, current->comm);
			DIAG_ERROR("chunkmem base:%08X  offset:%08X  size:%08X\n", pool->pbase, ka - pool->vbase, block.size);

			down(&chunkmem->sem);
			CHUNKMEM_REBASE(pool);
			dlMalloc_Status(NULL);
			up(&chunkmem->sem);

			/* free memory */
			gp_chunk_free(ka);
			break;
		}
		va += offset;
		block.addr = (void *)va;
		DIAG_VERB("va = %08X\n\n", va);

		if (copy_to_user((void __user *)arg, &block, sizeof(block))) {
			ret = -EFAULT;
			break;
		}
		break;

	case CHUNK_MEM_ALLOC_EX:
		if (copy_from_user(&block, (void __user *)arg, sizeof(block))) {
			ret = -EFAULT;
			break;
		}

		/* for backward compatibility: old code not set the pool_idx */
		if (block.pool_idx >= chunkmem_pool_count)
			block.pool_idx = 0;

		ka = gp_chunk_malloc_nocache(block.pool_idx, current->tgid, block.size);
		if (ka == NULL) {
			ret = -ENOMEM;
			break;
		}
		block.phy_addr = gp_chunk_pa(ka);
		block.size = dlMalloc_Usable_Size(ka) & PAGE_MASK; /* actual allocated size */
		pool = chunk_pool(ka);

		/* mmap to userspace */
		down(&chunkmem->sem);
		down_write(&current->mm->mmap_sem);
		chunkmem->mmap_enable = 1; /* enable mmap in CHUNK_MEM_ALLOC_EX */
		va = do_mmap(file, (unsigned long)block.addr, PAGE_ALIGN(block.size),
			     (file->f_mode & FMODE_WRITE) ? (PROT_READ | PROT_WRITE | PROT_EXEC) : (PROT_READ | PROT_EXEC),
			     MAP_SHARED,
			     (pool->pbase + (ka - pool->vbase)));
		chunkmem->mmap_enable = 0; /* disable it */
		up_write(&current->mm->mmap_sem);
		up(&chunkmem->sem);
		if (IS_ERR_VALUE(va)) {
			ret = va; /* errcode */
			DIAG_ERROR("CHUNK_MEM_ALLOC_EX: chunkmem mmap fail(%d)! (%s)\n", ret, current->comm);
			/* free memory */
			gp_chunk_free(ka);
			break;
		}
		DIAG_DEBUG("CHUNK_MEM_ALLOC_EX: req(%08X) ret(%08X)\n", (unsigned long)block.addr, va);
		block.addr = (void *)va;

		if (copy_to_user((void __user *)arg, &block, sizeof(block))) {
			ret = -EFAULT;
			break;
		}
		break;

	case CHUNK_MEM_FREE:
		if (copy_from_user(&block, (void __user *)arg, sizeof(block))) {
			ret = -EFAULT;
			break;
		}

		/* translate user_va to ka */
		DIAG_VERB("CHUNK_MEM_FREE:\n");
		DIAG_VERB("va = %08X\n", (unsigned int)block.addr);
		pa = gp_user_va_to_pa(block.addr);    /* user_addr to phy_addr */
		if (pa == 0) {
			DIAG_ERROR("CHUNK_MEM_FREE: chunkmem user_va_to_pa fail! (%s:%08X)\n", current->comm, block.addr);
			ret = -EFAULT;
			break;
		}
		DIAG_VERB("pa = %08X\n", pa);
		ka = gp_chunk_va(pa);                  /* phy_addr to kernel_addr */
		if (ka == NULL) {
			DIAG_ERROR("CHUNK_MEM_FREE: not a chunkmem address! (%s:%08X)\n", current->comm, pa);
			ret = -EFAULT;
			break;
		}
		block.size = dlMalloc_Usable_Size(ka) & PAGE_MASK;
		DIAG_VERB("ka = %08X\n", (unsigned int)ka);
		DIAG_VERB("actual size = %08X (%d)\n\n", block.size, block.size);

		/* munmap memory */
		down_write(&current->mm->mmap_sem);
		do_munmap(current->mm, (unsigned int)block.addr, block.size);
		up_write(&current->mm->mmap_sem);

		/* free memory */
		gp_chunk_free(ka);
#if (DIAG_LEVEL >= DIAG_LVL_VERB) && !defined(DIAG_VERB_OFF)
		down(&chunkmem->sem);
		dlMalloc_Status(NULL);
		up(&chunkmem->sem);
#endif
		break;

	case CHUNK_MEM_INFO:
	{
		chunk_info_t info;

		if (copy_from_user(&info, (void __user *)arg, sizeof(info))) {
			ret = -EFAULT;
			break;
		}

		if (info.pool_idx >= chunkmem_pool_count) {
			ret = -EINVAL;
			break;
		}

		if (info.pid == (unsigned int)(-1)) {
			info.pid = current->tgid;
		}

		down(&chunkmem->sem);
		info.total_pool_num = chunkmem_pool_count;
		pool += info.pool_idx;
		CHUNKMEM_REBASE(pool);
#if CHUNK_SUSPEND_TEST
		if (info.pid) {
			dlMalloc_Status(NULL);
		} else {
			dlMalloc_Status(NULL);
			gp_chunk_suspend(my_save_data);
			memset(pool->vbase, 0, pool->size);
			/* restore */
			while (blocks != NULL) {
				data_block_t *block = blocks;
				blocks = block->next;
				DIAG_DEBUG("restore data: %p %08X\n", block->addr, block->size);
				memcpy(block->addr, &block->data, block->size);
				kfree(block);
			}
		}
#else
		dlMalloc_Status((mem_info_t *)&info);
		if (info.pid == (unsigned int)(-2)) { // this is a special op: return game_emu user key
			info.used_bytes = *(unsigned int *)(pool->vbase + 0xFD0);
		}
#endif
		up(&chunkmem->sem);

		if (copy_to_user((void __user *)arg, &info, sizeof(info))) {
			ret = -EFAULT;
			break;
		}
		break;
	}

	case CHUNK_MEM_VA2PA:
		ret = -EFAULT;
		if (copy_from_user(&block, (void __user *)arg, sizeof(block))) {
			break;
		}

		pa = gp_user_va_to_pa(block.addr);    /* user_addr to phy_addr */
		if (pa != 0) {
			ka = gp_chunk_va(pa);             /* phy_addr to kernel_addr */
			if (ka != NULL) {
				block.phy_addr = pa;
				//block.size = dlMalloc_Usable_Size(ka) & PAGE_MASK;
				if (copy_to_user((void __user *)arg, &block, sizeof(block)) == 0) {
					ret = 0;
				}
			}
		}
		break;

	case CHUNK_MEM_MUNMAP:
		if (copy_from_user(&block, (void __user *)arg, sizeof(block))) {
			ret = -EFAULT;
			break;
		}

		va = (unsigned int)block.addr;
		/* page alignment */
		offset = va & ~PAGE_MASK;
		va &= PAGE_MASK;

		/* munmap memory */
		down_write(&current->mm->mmap_sem);
		do_munmap(current->mm, va, PAGE_ALIGN(block.size + offset));
		up_write(&current->mm->mmap_sem);
		break;

	case CHUNK_MEM_BANK:
		ret = -EFAULT;
		if (copy_from_user(&block, (void __user *)arg, sizeof(block))) {
			break;
		}
		pa = gp_user_va_to_pa(block.addr);    /* user_addr to phy_addr */
		block.bank_idx = sp_get_bank_num(pa);
		if (block.bank_idx != -1)
			ret = 0;
		break;

	default:
		ret = -ENOTTY; /* Inappropriate ioctl for device */
		break;
	}

	return ret;
}

static struct file_operations chunkmem_fops = {
	.owner		= THIS_MODULE,
	.open		= chunkmem_open,
	.release	= chunkmem_release,
	.mmap		= chunkmem_mmap,
	.unlocked_ioctl	= chunkmem_ioctl,
};

static void chunkmem_uninit(void)
{
	/* release/unmap all chunkmem resource */
	if (chunkmem) {
		chunkmem_pool_t *pool = chunkmem_pools;

		while (pool->pbase != 0) {
			if (pool->vbase != NULL) {
				iounmap(pool->vbase);
			}
			if (pool->mem != NULL) {
				release_resource(pool->mem);
			}
			pool++;
		}

		kfree(chunkmem);
		chunkmem = NULL;
		kfree(chunkmem_pools);
		chunkmem_pools = NULL;
	}
}

#include <linux/memblock.h>
#include <asm/setup.h>
#include <asm/memblock.h>
extern struct meminfo meminfo;
extern struct chunkmeminfo chunkmeminfo;

void assign_chunkmem_param()
{
	memnr = 1;
	membase1 = res_chunkmem_addr;
	memsize1 = res_chunkmem_size;
	membase2 = -1; // not used
	memsize2 = -1; // not used
}

/**
 * @brief   Chunkmem driver init function
 */
static int __init chunkmem_init(void)
{
	int ret = -ENXIO;
	chunkmem_pool_t *pool;

	// use new early parameter if existed
	if (res_chunkmem_addr != -1 && res_chunkmem_size != -1)
		assign_chunkmem_param();

	printk("%s: memnr=%d, membase1=%08X memsize1=%08X membase2=%08X memsize2=%08X\n",
	       __FUNCTION__, memnr, membase1, memsize1, membase2, memsize2);

	if (memnr == 0) {
		DIAG_ERROR("chunkmem init fail\n");
		goto fail_init;
	}

	chunkmem_pools = kmalloc(sizeof(chunkmem_pool_t) * NR_POOLS, GFP_KERNEL);
	if (chunkmem_pools == NULL) {
		DIAG_ERROR("chunkmem_pools kmalloc fail\n");
		ret = -ENOMEM;
		goto fail_kmalloc;
	}

	chunkmem_pools[0].pbase = membase1; // DRAM B
	chunkmem_pools[0].size = memsize1;

	if (memnr == 1) {
		chunkmem_pools[1].pbase=0;
	} else if (memnr == 2) {
		chunkmem_pools[1].pbase = membase2; // DRAM A
		chunkmem_pools[1].size = memsize2;

		chunkmem_pools[2].pbase = 0;
	} else {
		DIAG_ERROR("chunkmem pool number fail\n");
		goto fail_init;
	}
	pool = chunkmem_pools;

	chunkmem = (chunkmem_info_t *)kzalloc(sizeof(chunkmem_info_t), GFP_KERNEL);
	if (chunkmem == NULL) {
		DIAG_ERROR("chunkmem kmalloc fail\n");
		ret = -ENOMEM;
		goto fail_kmalloc;
	}

	/* for backward compatibility */
	if (membase && memsize) {
		pool->pbase = membase;
		pool->size = memsize;
	}

	chunkmem_pool_count = 0;
	while (pool->pbase != 0) {
#if 0
		/* request memory region */
		pool->mem = request_mem_region(pool->pbase, pool->size, "chunkmem");
		if (pool->mem == NULL) {
			DIAG_ERROR("chunkmem request mem region fail\n");
			goto fail_init;
		}
#endif

		/* remap memory region */
		pool->vbase = ioremap(pool->pbase, pool->size);
		if (pool->vbase == NULL) {
			DIAG_ERROR("chunkmem ioremap fail\n");
			goto fail_init;
		}
#if 0
		else {
			DIAG_ERROR("chunkmem ioremap successfully\n");
		}
#endif
		/* initialize */
		ret = dlMallocInit(pool->vbase, pool->size, PAGE_SIZE);
		if (ret != 0) {
			DIAG_ERROR("dlmalloc init fail\n");
			ret = -ENXIO;
			goto fail_init;
		}
		DIAG_INFO("[chunkmem.%d] pbase=%08X vbase=%08X size=%08X\n",
			  chunkmem_pool_count, pool->pbase, pool->vbase, pool->size);
		chunkmem_pool_count++;
		dlMalloc_Status(NULL);

		pool++;
	}

	sema_init(&chunkmem->sem, 1);
	sema_init(&chunkmem->opens_sem, 1);

	/* register device */
	chunkmem->dev.name  = "chunkmem";
	chunkmem->dev.minor = MISC_DYNAMIC_MINOR;
	chunkmem->dev.fops  = &chunkmem_fops;
	ret = misc_register(&chunkmem->dev);
	if (ret != 0) {
		DIAG_ERROR("chunkmem device register fail\n");
		goto fail_init;
	}

	return 0;

	/* error rollback */
fail_init:
	chunkmem_uninit();
fail_kmalloc:
	return ret;
}

/**
 * @brief   Chunkmem driver exit function
 */
static void __exit chunkmem_exit(void)
{
	misc_deregister(&chunkmem->dev);
	chunkmem_uninit();
}

module_init(chunkmem_init);
module_exit(chunkmem_exit);

/**************************************************************************
 *                  M O D U L E    D E C L A R A T I O N                  *
 **************************************************************************/

MODULE_AUTHOR("Sunplus");
MODULE_DESCRIPTION("Sunplus Chunk Memory Driver");
MODULE_LICENSE("GPL");
