/*
 * (C) Copyright 2014
 * Sunplus Technology
 * Kuo-Jung Su <dante.su@sunplus.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <linux/compat.h>

DECLARE_GLOBAL_DATA_PTR;

struct sp_cbdma_regs {
    uint32_t revr; /* Revision register */
    uint32_t cr;   /* Control register */
    uint32_t lr;   /* Length register */
    uint32_t src;  /* Source address */
    uint32_t dst;  /* Destination address */
    uint32_t isr;  /* Interrupt status register */
    uint32_t ier;  /* Interrupt enable register */
    uint32_t val;  /* MEMSET value register */
    uint32_t ram;  /* SDRAM config */
};

enum sp_cbdma_mode {
    SP_CBDMA_MEMSET = 0,
    SP_CBDMA_WRITE,
    SP_CBDMA_READ,
    SP_CBDMA_COPY,
};

#define CR_GO       BIT(8)
#define CR_MODE(x)  ((x) & 3)
#define CR_CMDQ(x)  (((x) & 3) << 16)

#define ISR_OVERLAP	BIT(6)
#define ISR_SDRAM_B	BIT(5)
#define ISR_SDRAM_A	BIT(4)
#define ISR_SRAM	BIT(3)
#define ISR_CBSRAM	BIT(2)
#define ISR_ERRORS	0x7c
#define ISR_DONE	BIT(0)

static struct sp_cbdma_regs *regs = (void *)CONFIG_SP_CBDMA_BASE;

static dma_addr_t phys_to_dma(dma_addr_t addr)
{
	unsigned long base = gd->bd->bi_dram[0].start;
	unsigned long size = gd->bd->bi_dram[0].size;
	dma_addr_t ret;

	if (addr > 0x7fffffff)
	{	 
	   ret = addr & 0x1ffff;
       return ret;
    }
	else
	  return addr;

/*		
	elseif (addr >= base && addr < base + size)
		ret = addr & (size - 1);
	else
		ret = 0x40000000 + (addr & (size - 1));

	return ret;
*/

}

static void sp_cbdma_reset(void)
{
	/* Reset interrupt status */
	writel(0x7d, &regs->ier);
	writel(0x7f, &regs->isr);

	/* SDRAM windows size, both bank A and B are 1GB */
	writel(0x0707, &regs->ram);
}

static int sp_cbdma_wait(void)
{
	unsigned long now;
	int ret = -EBUSY;

	for (now = get_timer(0); get_timer(now) < 50; ) {
		if (!(readl(&regs->cr) & CR_GO)) {
			ret = 0;
			break;
		}
	}

	return ret;
}

int sp_cbdma_memset(dma_addr_t buf, int val, size_t len)
{
	int ret;

	len = min_t(int, len, SZ_16M);

	sp_cbdma_reset();

	writel(val, &regs->val);
	writel(0, &regs->src);
	writel(phys_to_dma(buf), &regs->dst); /* DRAM */
	writel(len, &regs->lr);

	writel(CR_GO | CR_MODE(SP_CBDMA_MEMSET) | CR_CMDQ(3), &regs->cr);
	ret = sp_cbdma_wait();

	if (ret)
		printf("sp_cbdma: memset timeout\n");
	else
		ret = len;

	return ret;
}

int sp_cbdma_write(dma_addr_t src, dma_addr_t dst, size_t len)
{
	int ret;

	len = min_t(size_t, len, SZ_128K);

	sp_cbdma_reset();

	writel(phys_to_dma(src), &regs->src); /* sram */
	writel(phys_to_dma(dst), &regs->dst); /* DRAM */
	writel(len, &regs->lr);

	writel(CR_GO | CR_MODE(SP_CBDMA_WRITE) | CR_CMDQ(3), &regs->cr);
	ret = sp_cbdma_wait();

	if (ret)
		printf("sp_cbdma: write timeout\n");
	else
		ret = len;

	return ret;
}

int sp_cbdma_read(dma_addr_t src, dma_addr_t dst, size_t len)
{
	int ret;

	len = min_t(size_t, len, SZ_128K);

	sp_cbdma_reset();

	writel(phys_to_dma(src), &regs->src); /* DRAM */
	writel(phys_to_dma(dst), &regs->dst); /* sram */
	writel(len, &regs->lr);

	writel(CR_GO | CR_MODE(SP_CBDMA_READ) | CR_CMDQ(3), &regs->cr);
	ret = sp_cbdma_wait();

	if (ret)
		printf("sp_cbdma: read timeout\n");
	else
		ret = len;

	return ret;
}

int sp_cbdma_memcpy(dma_addr_t src, dma_addr_t dst, size_t len)
{
	int ret;

	len = min_t(int, len, SZ_16M);

	sp_cbdma_reset();

	writel(phys_to_dma(src), &regs->src); /* DRAM */
	writel(phys_to_dma(dst), &regs->dst); /* DRAM */
	writel(len, &regs->lr);

	writel(CR_GO | CR_MODE(SP_CBDMA_COPY) | CR_CMDQ(3), &regs->cr);
	ret = sp_cbdma_wait();

	if (ret)
		printf("sp_cbdma: copy timeout\n");
	else
		ret = len;

	return ret;
}
