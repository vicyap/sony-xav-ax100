/*
 * videobuf2-chunk-contig.c - Chunk contig memory allocator for videobuf2
 *
 * Copyright (C) 2014 Sunplus Technology
 *
 * Author: ky.chao <ky.chao@sunplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>

#include <media/videobuf2-core.h>
#include <media/videobuf2-chunk-contig.h>
#include <media/videobuf2-memops.h>

#include <mach/gp_chunkmem.h>

struct vb2_chunk_conf {
	struct device		*dev;
};

struct vb2_chunk_buf {
	struct vb2_chunk_conf		*conf;
	void				*vaddr;
	dma_addr_t			dma_addr;
	unsigned long			size;
	struct vm_area_struct		*vma;
	atomic_t			refcount;
	struct vb2_vmarea_handler	handler;
};

static void vb2_chunk_contig_put(void *buf_priv);

static void *vb2_chunk_contig_alloc(void *alloc_ctx, unsigned long size)
{
	struct vb2_chunk_conf *conf = alloc_ctx;
	struct vb2_chunk_buf *buf;

	buf = kzalloc(sizeof *buf, GFP_KERNEL);
	if (!buf)
		return ERR_PTR(-ENOMEM);

	buf->vaddr = gp_chunk_malloc_nocache(0, 0, size);
	buf->dma_addr = (dma_addr_t)gp_chunk_pa(buf->vaddr);
	if (!buf->vaddr) {
		dev_err(conf->dev, "gp_chunk_malloc_nocache of size %ld failed\n",
			size);
		kfree(buf);
		return ERR_PTR(-ENOMEM);
	}

	buf->conf = conf;
	buf->size = size;

	buf->handler.refcount = &buf->refcount;
	buf->handler.put = vb2_chunk_contig_put;
	buf->handler.arg = buf;

	atomic_inc(&buf->refcount);

	return buf;
}

static void vb2_chunk_contig_put(void *buf_priv)
{
	struct vb2_chunk_buf *buf = buf_priv;

	if (atomic_dec_and_test(&buf->refcount)) {
		gp_chunk_free(buf->vaddr);
		kfree(buf);
	}
}

static void *vb2_chunk_contig_cookie(void *buf_priv)
{
	struct vb2_chunk_buf *buf = buf_priv;

	return &buf->dma_addr;
}

static void *vb2_chunk_contig_vaddr(void *buf_priv)
{
	struct vb2_chunk_buf *buf = buf_priv;
	if (!buf)
		return NULL;

	return buf->vaddr;
}

static unsigned int vb2_chunk_contig_num_users(void *buf_priv)
{
	struct vb2_chunk_buf *buf = buf_priv;

	return atomic_read(&buf->refcount);
}

static int vb2_chunk_contig_mmap(void *buf_priv, struct vm_area_struct *vma)
{
	struct vb2_chunk_buf *buf = buf_priv;

	if (!buf) {
		printk(KERN_ERR "No buffer to map\n");
		return -EINVAL;
	}

	return vb2_mmap_pfn_range(vma, buf->dma_addr, buf->size,
				  &vb2_common_vm_ops, &buf->handler);
}

static void *vb2_chunk_contig_get_userptr(void *alloc_ctx, unsigned long vaddr,
					unsigned long size, int write)
{
	struct vb2_chunk_buf *buf;
	struct vm_area_struct *vma;
	dma_addr_t dma_addr = 0;
	int ret;

	buf = kzalloc(sizeof *buf, GFP_KERNEL);
	if (!buf)
		return ERR_PTR(-ENOMEM);

	ret = vb2_get_contig_userptr(vaddr, size, &vma, &dma_addr);
	if (ret) {
		printk(KERN_ERR "Failed acquiring VMA for vaddr 0x%08lx\n",
				vaddr);
		kfree(buf);
		return ERR_PTR(ret);
	}

	buf->size = size;
	buf->dma_addr = dma_addr;
	buf->vma = vma;

	return buf;
}

static void vb2_chunk_contig_put_userptr(void *mem_priv)
{
	struct vb2_chunk_buf *buf = mem_priv;

	if (!buf)
		return;

	vb2_put_vma(buf->vma);
	kfree(buf);
}

const struct vb2_mem_ops vb2_chunk_contig_memops = {
	.alloc		= vb2_chunk_contig_alloc,
	.put		= vb2_chunk_contig_put,
	.cookie		= vb2_chunk_contig_cookie,
	.vaddr		= vb2_chunk_contig_vaddr,
	.mmap		= vb2_chunk_contig_mmap,
	.get_userptr	= vb2_chunk_contig_get_userptr,
	.put_userptr	= vb2_chunk_contig_put_userptr,
	.num_users	= vb2_chunk_contig_num_users,
};
EXPORT_SYMBOL_GPL(vb2_chunk_contig_memops);

void *vb2_chunk_contig_init_ctx(struct device *dev)
{
	struct vb2_chunk_conf *conf;

	conf = kzalloc(sizeof *conf, GFP_KERNEL);
	if (!conf)
		return ERR_PTR(-ENOMEM);

	conf->dev = dev;

	return conf;
}
EXPORT_SYMBOL_GPL(vb2_chunk_contig_init_ctx);

void vb2_chunk_contig_cleanup_ctx(void *alloc_ctx)
{
	kfree(alloc_ctx);
}
EXPORT_SYMBOL_GPL(vb2_chunk_contig_cleanup_ctx);

MODULE_DESCRIPTION("Chunk-contig memory handling routines for videobuf2");
MODULE_AUTHOR("ky.chao <ky.chao@sunplus.com>");
MODULE_LICENSE("GPL");
