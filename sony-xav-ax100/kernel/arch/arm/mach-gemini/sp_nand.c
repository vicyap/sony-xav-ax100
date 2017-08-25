/*
 * (C) Copyright 2014, 2015, 2016
 * Sunplus Technology
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>
#include <linux/dma-mapping.h>
#include <mach/hardware.h>
#include <mach/sp_clk.h>

#include <asm/io.h>
#include <asm/div64.h>
#include <asm/uaccess.h>

// #include <asm/sysconfig.h>

#include "sp_bch.h"
#include "sp_nand.h"
#ifdef CONFIG_MTD_SP_NAND_CBDMA
#include "sp_cbdma.h"
#endif

#define NAND_RW_DATASIZE_FOR_ISP  1
#if NAND_RW_DATASIZE_FOR_ISP
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

unsigned long long Total_ISP_Write_Byte = 0;
struct proc_dir_entry *proc_entry_zzNAND;
#define NAND_RW_DBG 1
#if NAND_RW_DBG
int nand_addr_prompt=0;
unsigned long long nand_phy_addr;
#endif
#endif
#ifdef CONFIG_MTD_SP_NAND_HZ
#define CFG_NAND_HZ			CONFIG_MTD_SP_NAND_HZ
#else
#define CFG_NAND_HZ			304000000 /* 304 MHz */
#endif

#define CFG_NAND_IDS_FILE	"/usr/sp_nand.ids"
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER /* Noted that Page Size <= 8K,OOB <= 1K for this Ping-Pong Arch */
#define CFG_BUFF_MAX		(9 << 10)
#else
#define CFG_BUFF_MAX		(18 << 10)
#endif
#define CFG_DESC_MAX		4

#ifndef setbits_le32
#define setbits_le32(reg, val)	writel(readl(reg) | (val), reg)
#endif

#ifndef clrbits_le32
#define clrbits_le32(reg, val)	writel(readl(reg) & (~(val)), reg)
#endif

int grand_en;

#ifdef CONFIG_SP_NAND_IOCTL
#include <linux/miscdevice.h>
#endif

// #define gbootRom_boot_mode	 (*(volatile unsigned char *)(SRAM0_BASE + 0x9904))
unsigned int *sp_nand_ftl_info;
unsigned int* Get_SP_NAND_Info(void)
{
	return sp_nand_ftl_info;
}

struct sp_nand_info *info;

/* Wrapper for do_div(). Doesn't modify dividend and returns
 * the result, not reminder.
 */
static inline uint64_t lldiv(uint64_t dividend, uint32_t divisor)
{
	uint64_t result = dividend;
	do_div(result, divisor);
	return result;
}


static int sp_nand_reset(struct sp_nand_info *info);

static irqreturn_t sp_nand_irq(int irq, void *dev)
{
	struct sp_nand_info *info = dev;
	struct sp_nand_regs *regs = info->regs;
	uint32_t st;
	unsigned long timeout;

	timeout = jiffies + msecs_to_jiffies(1);

	st = readl(&regs->isr);
	writel(st, &regs->isr);

	info->busy = 0;
	wake_up(&info->wq);

	if (time_after(jiffies, timeout))
	{
		printk(KERN_ERR "more 1ms");
	}

	return IRQ_HANDLED;
}

static int sp_nand_wait(struct sp_nand_info *info)
{
	int ret = 0;

	if (!wait_event_timeout(info->wq, !info->busy, HZ/10))  /*HZ=100, means timeout=100*jiffies, jiffies=every 10ms plus one */
		ret = -1;

	return ret;
}

int sp_nand_desc_prep(struct sp_nand_info *info,
	uint8_t cmd, int col, int row)
{
	struct sp_nand_desc *desc = info->desc.virt;
	int mfs = (info->cs < 0) ? BIT(0) : BIT(info->cs);

    mutex_lock(&info->lock);

	memset(info->desc.virt, 0, info->desc.size);

	desc->ier     = ISR_DESC;
	desc->mfs     = mfs;
	desc->end     = 1;
	desc->owner   = 1; /* HW */
	desc->cs_ctrl = 1;
	desc->desc_sz = sizeof(struct sp_nand_desc);

	switch (cmd) {
	case NAND_CMD_READOOB:	/* 0x50 */
		col += info->mtd.writesize;
		/* fall through */
	case NAND_CMD_READ0:	/* 0x00 */
		info->buff.idx = col;
		desc->ff_check = 1;
		desc->cmd = SP_NAND_CMD_READ;
		desc->cmd0 = 0x00;
		desc->cmd1 = 0x30;
		desc->ier |= ISR_RB0;
		desc->addr_len = (info->cac + info->rac) - 1;
		desc->addr0 = (uint8_t)(col >> 0);
		desc->addr1 = (uint8_t)(col >> 8);
		desc->addr2 = (uint8_t)(row >> 0);
		desc->addr3 = (uint8_t)(row >> 8);
		desc->addr4 = (uint8_t)(row >> 16);
		desc->addr5 = (uint8_t)(row >> 24);
		if (col < info->mtd.writesize) {
			desc->oob_en = 1;
			desc->buf_dma = info->buff.phys;
			desc->buf_ssz = info->mtd.writesize - 1;
			desc->oob_dma = info->buff.phys + info->mtd.writesize;
			desc->oob_ssz = info->mtd.oobsize - 1;
		} else {
			desc->buf_dma = info->buff.phys + info->mtd.writesize;
			desc->buf_ssz = info->mtd.oobsize - 1;
		}
		desc->sect_nr = 0;
#ifdef CONFIG_MTD_SP_NAND_SCRAMBLER
		if (cmd == NAND_CMD_READ0)
		desc->rand_en = grand_en;
#endif

#if NAND_RW_DBG
	if(nand_addr_prompt){
        //nand_phy_addr = row<<info->mtd.writesize_shift;
        nand_phy_addr = row*info->mtd.writesize;
        printk("Nand Read phy addr:0x%llx \n",nand_phy_addr);
    }
#endif
		break;

	case NAND_CMD_SEQIN:	/* 0x80 */
		info->buff.idx = col;
		desc->cmd = SP_NAND_CMD_WRITE;
		desc->cmd0 = 0x80;
		desc->cmd1 = 0x10;
		desc->ier |= ISR_RB0;
		desc->addr_len = (info->cac + info->rac) - 1;
		desc->addr0 = (uint8_t)0;
		desc->addr1 = (uint8_t)0;
		desc->addr2 = (uint8_t)(row >> 0);
		desc->addr3 = (uint8_t)(row >> 8);
		desc->addr4 = (uint8_t)(row >> 16);
		desc->addr5 = (uint8_t)(row >> 24);
		desc->oob_en = 1;
		desc->buf_dma = info->buff.phys;
		desc->buf_ssz = info->mtd.writesize - 1;
		desc->oob_dma = info->buff.phys + info->mtd.writesize;
		desc->oob_ssz = info->mtd.oobsize - 1;
		desc->sect_nr = 0;
#ifdef CONFIG_MTD_SP_NAND_SCRAMBLER
		desc->rand_en = 1;
#endif
		//desc->ac_timing_select = 1;
#if NAND_RW_DBG
        if(nand_addr_prompt){
            //nand_phy_addr = row<<info->mtd.writesize_shift;
            nand_phy_addr = row*info->mtd.writesize;
            printk("Nand Write phy addr:0x%llx \n",nand_phy_addr);
        }
#endif
		break;

	case NAND_CMD_ERASE1:	/* 0x60 */
		desc->cmd = SP_NAND_CMD_ERASE;
		desc->cmd0 = 0x60;
		desc->cmd1 = 0xd0;
		desc->addr_len = info->rac - 1;
		if (info->rac <= 3) {
			desc->addr2 = (uint8_t)(row >> 0);
			desc->addr3 = (uint8_t)(row >> 8);
			desc->addr4 = (uint8_t)(row >> 16);
		} else {
			desc->addr0 = (uint8_t)(row >> 0);
			desc->addr1 = (uint8_t)(row >> 8);
			desc->addr2 = (uint8_t)(row >> 16);
			desc->addr3 = (uint8_t)(row >> 24);
		}
		break;

	case NAND_CMD_STATUS:	/* 0x70 */
		desc->cmd = SP_NAND_CMD_RDST;
		desc->cmd0 = 0x70;
		break;

	case NAND_CMD_READID:	/* 0x90 */
	case NAND_CMD_PARAM:	/* 0xEC */
	case NAND_CMD_GET_FEATURES: /* 0xEE */
		desc->cmd = SP_NAND_CMD_SUPER;
		desc->cmd0 = cmd;
		desc->ctrl = BIT(1) | BIT(3); /* address + data read */
		desc->addr2 = (uint8_t)col;
		desc->buf_dma = info->buff.phys;
		switch (cmd) {
		case NAND_CMD_PARAM:
			desc->ctrl |= BIT(2); /* R/B */
			desc->buf_ssz = 1024;
			break;
		case NAND_CMD_GET_FEATURES:
			desc->ctrl |= BIT(2); /* R/B */
			desc->buf_ssz = 4;
			break;
		default:
			desc->buf_ssz = 7;
			break;
		}
		info->buff.idx = 0;
		memset(info->buff.virt, 0xff, desc->buf_ssz);
		break;

	case NAND_CMD_SET_FEATURES: /* 0xEF */
		desc->cmd = SP_NAND_CMD_SUPER;
		desc->cmd0 = cmd;
		desc->ctrl = BIT(1) | BIT(2) | BIT(4); /* addr + R/B + write */
		desc->addr2 = (uint8_t)col;
		desc->buf_dma = info->buff.phys;
		desc->buf_ssz = 4;
		info->buff.idx = 0;
		memset(info->buff.virt, 0xff, desc->buf_ssz);
		break;

	case NAND_CMD_RESET:	/* 0xFF */
		desc->cmd = SP_NAND_CMD_SUPER;
		desc->cmd0 = 0xff;
		desc->ctrl = BIT(2); /* R/B# */
		break;

	default:
		BUG();
		break;
	}

	return 0;
}
EXPORT_SYMBOL(sp_nand_desc_prep);

int sp_nand_desc_send(struct sp_nand_info *info)
{
	int ret;
	struct sp_nand_regs *regs = info->regs;

	while ((readl(&regs->isr) & (ISR_HW_BUSY)) != 0) ;

	if( info->cmd == NAND_CMD_STATUS )
	   clrbits_le32(&regs->csr, CSR_NAND_EDO);

	writel(0xffff, &regs->isr);
	while (readl(&regs->isr)) ;

	writel(info->desc.phys, &regs->dbar);

	info->busy = 1;
	setbits_le32(&regs->csr, CSR_DESC_FETCH);
	ret = sp_nand_wait(info);

	if (ret) {
		printk("sp_nand: descriptor timeout, cmd=0x%x\n", info->cmd);
		sp_nand_reset(info);
	}

	//while ((readl(&regs->isr) & (ISR_HW_BUSY)) != 0) ;

	if( info->cmd == NAND_CMD_STATUS )
	   setbits_le32(&regs->csr, CSR_NAND_EDO);

    mutex_unlock(&info->lock);
	return 0;
}
EXPORT_SYMBOL(sp_nand_desc_send);

#ifdef CONFIG_MTD_SP_NAND_SCRAMBLER

static void sp_nand_get_oob_scrambled(struct mtd_info *mtd, int row)
{
	struct sp_nand_info *info = container_of(mtd, struct sp_nand_info, mtd);
	struct sp_nand_desc *desc = info->desc.virt;
	int mfs = (info->cs < 0) ? BIT(0) : BIT(info->cs);

	memset(desc, 0, sizeof(*desc));

	desc->ier     = ISR_DESC;
	desc->mfs     = mfs;
	desc->end     = 1;
	desc->owner   = 1; /* HW */
	desc->cs_ctrl = 1;
	desc->desc_sz = sizeof(struct sp_nand_desc);

	desc->cmd = SP_NAND_CMD_READ;
	desc->cmd0 = 0x00;
	desc->cmd1 = 0x30;
	desc->ier |= ISR_RB0;
	desc->addr_len = (info->cac + info->rac) - 1;
	desc->addr0 = (uint8_t)0;
	desc->addr1 = (uint8_t)0;
	desc->addr2 = (uint8_t)(row >> 0);
	desc->addr3 = (uint8_t)(row >> 8);
	desc->addr4 = (uint8_t)(row >> 16);
	desc->addr5 = (uint8_t)(row >> 24);
	desc->oob_en = 1;
	desc->buf_dma = info->buff.phys;
	desc->buf_ssz = info->mtd.writesize - 1;
	desc->oob_dma = info->buff.phys + info->mtd.writesize;
	desc->oob_ssz = info->mtd.oobsize - 1;
	desc->sect_nr = 0;
	desc->rand_en = 1;

	sp_nand_desc_send(info);
}

#endif /* CONFIG_MTD_SP_NAND_SCRAMBLER */

#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
int PingPongBuf_0_PageReadCmdSend(struct sp_nand_info *info, uint8_t cmd, int col, int row)
{
    struct sp_nand_desc *desc = info->desc.virt;
    struct sp_nand_regs *regs = info->regs;
    int ret;

    int mfs = (info->cs < 0) ? BIT_MASK(0) : BIT_MASK(info->cs);

    if (row == info->buff_page && info->buff_state > PP_BUF_IDLE) {
        // DBG printk("b0-row:%d pre-fetched\n",row);
        return 0;
    }

    mutex_lock(&info->lock);

    // DBG printk("b0-row:%d,%d\n",row,info->buff_page);
    info->buff_state = PP_BUF_IDLE;
    info->buff_page  = row;

    memset(info->desc.virt, 0, info->desc.size);

    desc->ier     = ISR_DESC;
    desc->mfs     = mfs;
    desc->end     = 1;
    desc->owner   = 1; /* HW */
    desc->cs_ctrl = 1;
    desc->desc_sz = sizeof(struct sp_nand_desc);

    info->buff.idx = col;

    desc->cmd = SP_NAND_CMD_READ;
    desc->cmd0 = 0x00;
    desc->cmd1 = 0x30;
    desc->ier |= ISR_RB0;
    desc->addr_len = (info->cac + info->rac) - 1;
    desc->addr0 = (uint8_t)(col >> 0);
    desc->addr1 = (uint8_t)(col >> 8);
    desc->addr2 = (uint8_t)(row >> 0);
    desc->addr3 = (uint8_t)(row >> 8);
    desc->addr4 = (uint8_t)(row >> 16);
    desc->addr5 = (uint8_t)(row >> 24);

    desc->buf_dma = info->buff.phys;
    desc->buf_ssz = info->mtd.writesize - 1;
    desc->oob_dma = info->buff.phys + info->mtd.writesize;
    desc->oob_ssz = info->mtd.oobsize - 1;

    desc->oob_en = 1;

    while ((readl(&regs->isr) & (ISR_HW_BUSY)) != 0);

    writel(0xffff, &regs->isr);
    while (readl(&regs->isr));

    writel(info->desc.phys, &regs->dbar);
    info->busy = 1;
    setbits_le32(&regs->csr, CSR_DESC_FETCH);

    info->buff_state = PP_BUF_CMD_SEND;

    mutex_unlock(&info->lock);
}

int PingPongBuf_1_PageReadCmdSend(struct sp_nand_info *info, uint8_t cmd, int col, int row)
{
    struct sp_nand_desc *desc = info->desc.virt;
    struct sp_nand_regs *regs = info->regs;

    int mfs = (info->cs < 0) ? BIT_MASK(0) : BIT_MASK(info->cs);

    if (row == info->buff1_page && info->buff1_state > PP_BUF_IDLE) {
        // DBG printk("b1-row:%d pre-fetched\n",row);
        return 0;
    }

    mutex_lock(&info->lock);
    // DBG printk("b1-row:%d,%d\n",row,info->buff1_page);
    info->buff1_state = PP_BUF_IDLE;
    info->buff1_page  = row;

    memset(info->desc.virt, 0, info->desc.size);

    desc->ier     = ISR_DESC;
    desc->mfs     = mfs;
    desc->end     = 1;
    desc->owner   = 1; /* HW */
    desc->cs_ctrl = 1;
    desc->desc_sz = sizeof(struct sp_nand_desc);

    info->buff1.idx = col;

    desc->cmd = SP_NAND_CMD_READ;
    desc->cmd0 = 0x00;
    desc->cmd1 = 0x30;
    desc->ier |= ISR_RB0;
    desc->addr_len = (info->cac + info->rac) - 1;
    desc->addr0 = (uint8_t)(col >> 0);
    desc->addr1 = (uint8_t)(col >> 8);
    desc->addr2 = (uint8_t)(row >> 0);
    desc->addr3 = (uint8_t)(row >> 8);
    desc->addr4 = (uint8_t)(row >> 16);
    desc->addr5 = (uint8_t)(row >> 24);

    desc->buf_dma = info->buff1.phys;
    desc->buf_ssz = info->mtd.writesize - 1;
    desc->oob_dma = info->buff1.phys + info->mtd.writesize;
    desc->oob_ssz = info->mtd.oobsize - 1;

    desc->oob_en = 1;

    while ((readl(&regs->isr) & (ISR_HW_BUSY)) != 0);

    writel(0xffff, &regs->isr);
    while (readl(&regs->isr));

    writel(info->desc.phys, &regs->dbar);
    info->busy = 1;
    setbits_le32(&regs->csr, CSR_DESC_FETCH);

    info->buff1_state = PP_BUF_CMD_SEND;

    mutex_unlock(&info->lock);
}
#endif

static int sp_nand_read_page(struct mtd_info *mtd,
	struct nand_chip *chip, uint8_t *buf, int page)
{
	struct sp_nand_info *info = container_of(mtd, struct sp_nand_info, mtd);
	int ret;

#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
	// DBG printk("read-page:0x%x\n",page);
    down(&info->spnand_sem);
    ret = sp_nand_wait(info);
    if (ret) {
        pr_info("sp_nand: descriptor timeout, cmd=0x%x\n", info->cmd);
        sp_nand_reset(info);
        goto exit_pp;
    }

	if (info->buff_flag == USE_PP_BUF_0) {
        info->buff_state = PP_BUF_DATA_RDY; // buff-0 data rdy,
        PingPongBuf_1_PageReadCmdSend(info, NAND_CMD_READ0, 0, page + 1);
        ret = sp_bch_decode(mtd, info->buff.phys,info->buff.phys + mtd->writesize);
        if (ret)
		    printk("pp-buf0 sp_nand: bch decode failed at page=%d\n", page);
        info->buff_state = PP_BUF_DECODE_RDY; // buff-0 decode rdy,
    } else {
        info->buff1_state = PP_BUF_DATA_RDY; // buff-1 data rdy,
        PingPongBuf_0_PageReadCmdSend(info, NAND_CMD_READ0, 0, page + 1);
        ret = sp_bch_decode(mtd, info->buff1.phys,info->buff1.phys + mtd->writesize);
        if (ret)
		    printk("pp-buf1 sp_nand: bch decode failed at page=%d\n", page);
	    info->buff1_state = PP_BUF_DECODE_RDY; // buff-1 decode rdy,
    }

	chip->read_buf(mtd, buf, mtd->writesize);
	if (page < 0)
        goto exit_pp;

	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);
exit_pp:
	up(&info->spnand_sem);
	return 0;
#endif
    down(&info->spnand_sem);
	ret = sp_bch_decode(mtd, info->buff.phys,
			info->buff.phys + mtd->writesize);
	if (ret)
		printk("sp_nand: bch decode failed at page=%d\n", page);

	chip->read_buf(mtd, buf, mtd->writesize);

    #if NAND_RW_DATASIZE_FOR_ISP
	Total_ISP_Write_Byte +=mtd->writesize;
	#endif
	if (page < 0)
		goto exit_read;

	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);

exit_read:
	up(&info->spnand_sem);
	return 0;
}

static void sp_nand_write_page(struct mtd_info *mtd,
	struct nand_chip *chip, const uint8_t *buf)
{
	struct sp_nand_info *info = container_of(mtd, struct sp_nand_info, mtd);

	down(&info->spnand_sem);
	chip->write_buf(mtd, buf, mtd->writesize);
	chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);

	sp_bch_encode(mtd, info->buff.phys, info->buff.phys + mtd->writesize);
	#if NAND_RW_DATASIZE_FOR_ISP
	Total_ISP_Write_Byte +=mtd->writesize;
	#endif
	up(&info->spnand_sem);
}

static int sp_nand_verify_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	int ret = 0;
	void *tmp = kmalloc(mtd->writesize, GFP_KERNEL);

	if (!tmp)
		return -ENOMEM;

	sp_nand_read_page(mtd, mtd->priv, tmp, -1);
	if (memcmp(buf, tmp, len)) {
		printk("sp_nand: verify failed\n");
		ret = -EFAULT;
	}

	kfree(tmp);
	return ret;
}

/*
 * Check hardware register for wait status. Returns 1 if device is ready,
 * 0 if it is still busy.
 */
static int sp_nand_dev_ready(struct mtd_info *mtd)
{
	//struct sp_nand_info *info = container_of(mtd, struct sp_nand_info, mtd);
	//struct sp_nand_regs *regs = info->regs;

	//return (readl(&regs->isr) & (ISR_BUS_BUSY | ISR_HW_BUSY)) == 0;

	return 1;
}

static uint8_t sp_nand_read_byte(struct mtd_info *mtd)
{
	struct sp_nand_info *info = container_of(mtd, struct sp_nand_info, mtd);
	struct sp_nand_desc *desc = info->desc.virt;
	uint8_t ret = 0;

	switch (info->cmd) {
	case NAND_CMD_STATUS:
		ret = desc->ctrl;
		break;

	default:
		if (info->buff.idx < info->buff.size) {
			ret = readb(info->buff.virt + info->buff.idx);
			info->buff.idx += 1;
		}
		break;
	}

	return ret;
}

static void sp_nand_read_buf(struct mtd_info *mtd,
	u_char *buf, int len)
{
	struct sp_nand_info *info = container_of(mtd, struct sp_nand_info, mtd);

	/* refer nand_read_buf() callback function at every nand driver,
	 * they all use pio mode NOT to use dma mode.
	 *
	 * we do an experiment to try to use dma mode when buf is vaild.
	 * it will increade performance but it may have side effect.
	 * if yes then we will use completely pio mode at this function. */
	if (!virt_addr_valid(buf) || !virt_addr_valid(buf + len - 1)) {
		memcpy(buf, info->buff.virt + info->buff.idx, len);
		info->buff.idx += len;
		return;
	}

#ifdef CONFIG_MTD_SP_NAND_CBDMA
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
  struct sp_nand_regs *regs = info->regs;
  if (info->buff_flag == USE_PP_BUF_0) {
    if (info->buff_state == PP_BUF_CMD_SEND)
        while (readl(&regs->csr) & CSR_DESC_FETCH);
#endif
	dma_addr_t src = (dma_addr_t)info->buff.phys + info->buff.idx;
	dma_addr_t dst = dma_map_single(info->dev, buf, len, DMA_FROM_DEVICE);

	sp_cbdma_write(info->dma, src, dst, len);
	dma_unmap_single(info->dev, dst, len, DMA_FROM_DEVICE);
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
    info->buff_state = PP_BUF_IDLE; // buf-0 back to idle
  } else {
    if (info->buff1_state == PP_BUF_CMD_SEND)
        while (readl(&regs->csr) & CSR_DESC_FETCH);
    dma_addr_t src = (dma_addr_t)info->buff1.phys + info->buff1.idx;
    dma_addr_t dst = dma_map_single(info->dev, buf, len, DMA_FROM_DEVICE);

    sp_cbdma_write(info->dma, src, dst, len);
    dma_unmap_single(info->dev, dst, len, DMA_FROM_DEVICE);

    info->buff1.idx += len;
    info->buff1_state = PP_BUF_IDLE; // buf-1 back to idle
    return;
  }
#endif
#else
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
    struct sp_nand_regs *regs = info->regs;
    if (info->buff_flag == USE_PP_BUF_0) {
        if (info->buff_state == PP_BUF_CMD_SEND)
            while (readl(&regs->csr) & CSR_DESC_FETCH);
#endif
	memcpy(buf, info->buff.virt + info->buff.idx, len);
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
        info->buff_state = PP_BUF_IDLE; // buf-0 back to idle
    } else {
        if (info->buff1_state == PP_BUF_CMD_SEND)
            while (readl(&regs->csr) & CSR_DESC_FETCH);
        memcpy(buf, info->buff1.virt + info->buff1.idx, len);
        info->buff1.idx += len;
        info->buff1_state = PP_BUF_IDLE; // buf-1 back to idle
        return;
    }
#endif
#endif
	info->buff.idx += len;
}

static void sp_nand_write_buf(struct mtd_info *mtd,
	const u_char *buf, int len)
{
	struct sp_nand_info *info = container_of(mtd, struct sp_nand_info, mtd);

	if (!virt_addr_valid(buf) || !virt_addr_valid(buf + len - 1)) {
		memcpy(info->buff.virt + info->buff.idx, buf, len);
		info->buff.idx += len;

		if (info->cmd == NAND_CMD_SET_FEATURES)
			sp_nand_desc_send(info);
		return;
	}

#ifdef CONFIG_MTD_SP_NAND_CBDMA
	dma_addr_t src = dma_map_single(info->dev, (void *)buf, len,
			DMA_TO_DEVICE);
	dma_addr_t dst = (dma_addr_t)info->buff.phys + info->buff.idx;

	sp_cbdma_read(info->dma, src, dst, len);

	dma_unmap_single(info->dev, src, len, DMA_TO_DEVICE);
#else
	memcpy(info->buff.virt + info->buff.idx, buf, len);
#endif
	info->buff.idx += len;

	if (info->cmd == NAND_CMD_SET_FEATURES)
		sp_nand_desc_send(info);
}

static void sp_nand_cmdfunc(struct mtd_info *mtd,
	unsigned cmd, int col, int row)
{
	struct sp_nand_info *info = container_of(mtd, struct sp_nand_info, mtd);
	struct sp_nand_regs *regs = info->regs;
	uint32_t val;

	info->cmd = cmd;

	switch (cmd)  {
	case NAND_CMD_READ0:	/* 0x00 */
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
        while (readl(&regs->csr) & CSR_DESC_FETCH);
	    if (row%2) {
            // DBG printk("row=%d ,BUF-1,",row);
            info->buff_flag = USE_PP_BUF_1;
            PingPongBuf_1_PageReadCmdSend(info, cmd, col, row);
        } else {
            // DBG printk("row=%d ,BUF-0,",row);
            info->buff_flag = USE_PP_BUF_0;
            PingPongBuf_0_PageReadCmdSend(info, cmd, col, row);
        }
        break;
#endif
		grand_en = 1;
		sp_nand_desc_prep(info, cmd, col, row);
		sp_nand_desc_send(info);
		if (readb(info->buff.virt + mtd->writesize + 1) == 0xff) break;
		grand_en = 0;
	case NAND_CMD_READOOB:	/* 0x50 */
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
        while (readl(&regs->csr) & CSR_DESC_FETCH);
        info->buff_flag   = USE_PP_BUF_0;
        info->buff_page   = -1;
        info->buff_state  = PP_BUF_IDLE;
        info->buff1_page  = -1;
        info->buff1_state = PP_BUF_IDLE;
#endif
		sp_nand_desc_prep(info, cmd, col, row);
		sp_nand_desc_send(info);
#ifdef CONFIG_MTD_SP_NAND_SCRAMBLER
		if (readb(info->buff.virt + mtd->writesize + 1) != 0xff)
			sp_nand_get_oob_scrambled(mtd, row);
#endif
//printk("Francis [sp_nand_cmdfunc] cmd=read\n");
		break;

	case NAND_CMD_SEQIN:	/* 0x80 */
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
        while (readl(&regs->csr) & CSR_DESC_FETCH);
        info->buff_state  = PP_BUF_IDLE;
#endif
        sp_nand_desc_prep(info, cmd, col, row);
		break;

	case NAND_CMD_PAGEPROG:	/* 0x10 */
#ifdef CONFIG_MTD_SP_NAND_SCRAMBLER
		writeb(0xff, info->buff.virt + mtd->writesize + 1);
#endif
//printk("Francis [sp_nand_cmdfunc] cmd=write\n");
		sp_nand_desc_send(info);
		break;

	case NAND_CMD_ERASE1:	/* 0x60 */
		sp_nand_desc_prep(info, cmd, col, row);
#ifdef CONFIG_MTD_SP_NAND_BKER_COUNTER
		info->blk_erased[row / (info->mtd.erasesize / info->mtd.writesize)] += 1;
#endif
		break;

	case NAND_CMD_ERASE2:	/* 0xD0 */
//printk("Francis [sp_nand_cmdfunc] cmd=erase\n");
		sp_nand_desc_send(info);
		break;

	case NAND_CMD_STATUS:	/* 0x70 */
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
        while (readl(&regs->csr) & CSR_DESC_FETCH);
        info->buff_flag   = USE_PP_BUF_0;
        info->buff_page   = -1;
        info->buff_state  = PP_BUF_IDLE;
        info->buff1_page  = -1;
        info->buff1_state = PP_BUF_IDLE;
#endif
		sp_nand_desc_prep(info, cmd, -1, -1);
		sp_nand_desc_send(info);
		break;

	case NAND_CMD_READID:	/* 0x90 */
	case NAND_CMD_PARAM:    /* 0xEC */
	case NAND_CMD_GET_FEATURES: /* 0xEE */
	case NAND_CMD_RESET:	/* 0xFF */
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
        while (readl(&regs->csr) & CSR_DESC_FETCH);
        info->buff_flag   = USE_PP_BUF_0;
        info->buff_page   = -1;
        info->buff_state  = PP_BUF_IDLE;
        info->buff1_page  = -1;
        info->buff1_state = PP_BUF_IDLE;
#endif
		val = readl(&regs->actr0);
		writel(0xffffffff, &regs->actr0);
		sp_nand_desc_prep(info, cmd, col, -1);
		sp_nand_desc_send(info);
		writel(val, &regs->actr0);
//		writel(0x18187c44, &regs->actr0);
//		writel(0x0c0c4833, &regs->actr1);
		break;

	case NAND_CMD_SET_FEATURES: /* 0xEF */
		sp_nand_desc_prep(info, cmd, col, -1);
		break;

	default:
		printk("sp_nand: unknown command=0x%02x.\n", cmd);
		break;
	}
}

/**
 * nand_select_chip - control CE line
 * @mtd:	MTD device structure
 * @chipnr:	chipnumber to select, -1 for deselect
 */
static void sp_nand_select_chip(struct mtd_info *mtd, int chipnr)
{
	struct sp_nand_info *info = container_of(mtd, struct sp_nand_info, mtd);

	switch (chipnr) {
	case -1:
	case 0:
	case 1:
	case 2:
	case 3:
		info->cs = chipnr;
		break;

	default:
		BUG();
	}
}

#if 0
/*
 * Load NAND flash parameters from configuration file
 */
static int sp_nand_loadids(struct sp_nand_info *info, const char *fn)
{
	int ret, loc_ids, ext_ids;
	loff_t pos;
	char *s, *tok, buf[256];
	struct file *filp;
	mm_segment_t fs_bak;
	struct nand_flash_dev *type;

	if (info->ids) {
		kfree(info->ids);
		info->ids = NULL;
	}

	printk("sp_nand: reading '%s' ...\n", fn);

	fs_bak = get_fs();
	set_fs(get_ds());

	filp = filp_open(fn, O_RDONLY, 0);
	if (IS_ERR(filp) || !filp->f_op || !filp->f_op->read) {
		printk("sp_nand: file not found!\n");
		ret = -EIO;
		info->ids = nand_flash_ids;
		goto ids_err;
	}

	loc_ids = 0;
	for (type = nand_flash_ids; type->name; ++type)
		++loc_ids;

	ext_ids = 0;
	filp->f_pos = pos = 0;
	do {
		ret = filp->f_op->read(filp, buf, sizeof(buf), &filp->f_pos);
		if (ret <= 0)
			break;
		s = strchr(buf, '\n');
		if (s)
			*s = 0;
		else
			buf[sizeof(buf) - 1] = 0;
		pos += strlen(buf) + 1;
		filp->f_pos = pos;

		/* configuration line parsing */
		s = strim(buf);
		/* skip empty lines */
		if (!strlen(s))
			continue;

		/* skip comments */
		if (s[0] == ';' || s[0] == '#')
			continue;

		++ext_ids;
	} while (1);

	printk("sp_nand: nr_ids = %d+%d\n", ext_ids, loc_ids);

	info->ids = kzalloc((loc_ids + ext_ids + 1) * sizeof(*type), GFP_KERNEL);
	if (!info->ids) {
		printk("sp_nand: out of memory\n");
		ret = -ENOMEM;
		info->ids = nand_flash_ids;
		goto ids_err;
	}

	/* read external nand flash ids */
	type = info->ids;
	filp->f_pos = pos = 0;
	do {
		ret = filp->f_op->read(filp, buf, sizeof(buf), &filp->f_pos);
		if (ret <= 0)
			break;
		s = strchr(buf, '\n');
		if (s)
			*s = 0;
		else
			buf[sizeof(buf) - 1] = 0;
		pos += strlen(buf) + 1;
		filp->f_pos = pos;

		/* configuration line parsing */
		s = strim(buf);
		/* skip empty lines */
		if (!strlen(s))
			continue;

		/* skip comments */
		if (s[0] == ';' || s[0] == '#')
			continue;

		/* name */
		tok = strsep(&s, ",");
		type->name = kstrdup(strim(tok), GFP_KERNEL);

		/* id */
		tok = strsep(&s, ",");
		type->id = simple_strtoul(strim(tok), NULL, 0);

		/* ext_id */
		tok = strsep(&s, ",");
		type->ext_id = simple_strtoul(strim(tok), NULL, 0);

		/* page size */
		tok = strsep(&s, ",");
		type->pagesize = simple_strtoul(strim(tok), NULL, 0);

		/* oob size */
		tok = strsep(&s, ",");
		type->pagesize += simple_strtoul(strim(tok), NULL, 0);

		/* block size */
		tok = strsep(&s, ",");
		type->erasesize = simple_strtoul(strim(tok), NULL, 0);

		/* chip size */
		tok = strsep(&s, ",");
		type->chipsize = simple_strtoul(strim(tok), NULL, 0);

		/* options */
		tok = strsep(&s, ",");
		type->options = simple_strtoul(strim(tok), NULL, 0);

		++type;
	} while (1);

	/* copy nand_flash_ids */
	memcpy(info->ids + ext_ids, nand_flash_ids, loc_ids * sizeof(*type));

	ret = 0;
	printk("sp_nand: reading '%s' ...done\n", fn);

ids_err:
	if (!IS_ERR(filp))
		filp_close(filp, NULL);
	set_fs(fs_bak);
	return ret;
}

#else

// CFG_NAND_IDS_FILE:
// # name, id, ext_id, pagesize, oobsize, blocksize, chipsize(MB), option
//
// # Hynix
// H27UCG8T2ATR, 0xADDE, 0x94DA74C4,  8192,  640, 0x200000, 8192, 0x11D
// H27UCG8T2BTR, 0xADDE, 0x94EB7444, 16384, 1280, 0x400000, 8192, 0x11D
// H27UBG8T2CTR, 0xADD7, 0x94916044,  8192,  640, 0x200000, 4096, 0x11D
//
// # Micron
// MT29F32G08CBADA, 0x2C44, 0x444BA900, 8192, 744, 0x200000, 4096, 0x11D
// MT29F8G08ABACA,  0x2CD3, 0x90A66400, 4096, 224,  0x40000, 1024, 0x11D
//
// # Toshiba
// TC58NVG3SOFTA00, 0x98d3, 0x90267615, 4096, 232,  0x40000, 1024, 0x11D

struct nand_flash_dev nand_flash_ids2[] = {
    {"H27UCG8T2ATR",    0xADDE, ( 8192 +  640), 8192, 0x200000, 0x11D, 0x94DA74C4},
    {"H27UCG8T2BTR",    0xADDE, (16384 + 1280), 8192, 0x400000, 0x11D, 0x94EB7444},
    {"H27UBG8T2CTR",    0xADD7, ( 8192 +  640), 4096, 0x200000, 0x11D, 0x94916044},
    {"MT29F32G08CBADA", 0x2C44, ( 8192 +  744), 4096, 0x200000, 0x11D, 0x444BA900},
    {"MT29F8G08ABACA",  0x2CD3, ( 4096 +  224), 1024,  0x40000, 0x11D, 0x90A66400},
    {"TC58NVG3SOFTA00", 0x98d3, ( 4096 +  232), 1024,  0x40000, 0x11D, 0x90267615}
};

static int sp_nand_loadids(struct sp_nand_info *info, const char *fn)
{
    int num, num_nand_flash_ids;
    struct nand_flash_dev *ptr;

    printk("sp_nand: %s\n", __func__);

    num_nand_flash_ids = 0;
    for(ptr=nand_flash_ids; ptr->name; ptr++)
        num_nand_flash_ids++;

    num = ARRAY_SIZE(nand_flash_ids2) + num_nand_flash_ids;

    info->ids = kzalloc(num * sizeof(struct nand_flash_dev), GFP_KERNEL);
    if (!info->ids) {
        printk("sp_nand: out of memory\n");
        info->ids = nand_flash_ids;
        return -ENOMEM;
    }

    memcpy(info->ids, nand_flash_ids2, sizeof(nand_flash_ids2));
    memcpy(&(info->ids[ARRAY_SIZE(nand_flash_ids2)]), nand_flash_ids, num_nand_flash_ids*sizeof(struct nand_flash_dev));

    return 0;
}

#endif


static int sp_nand_fixup(struct sp_nand_info *info)
{
	struct mtd_info *mtd = &info->mtd;
	struct nand_chip *chip = mtd->priv;
	uint64_t nrpg;

	/* column address */
	info->cac = 2;

	/* row address */
	nrpg = lldiv(chip->chipsize, mtd->writesize);
	if (nrpg & 0xff000000)
		info->rac = 4;
	else if (nrpg & 0xffff0000)
		info->rac = 3;
	else
		info->rac = 2;

	return 0;
}

static void sp_nand_cmd_ctrl(struct mtd_info *mtd,
	int cmd, unsigned int ctrl)
{
	return;
}

#ifdef CONFIG_MTD_SP_NAND_ACTR_OPTIMIZED
static void sp_nand_acopt(struct sp_nand_info *info)
{
	struct sp_nand_regs *regs = info->regs;
	uint32_t val = 0;

#if 0
	int twb = 100;  /* ns, tWB */
	int cle = 100;  /* ns, max(tCLS, tCLH, tCS) */
	int ale = 50;   /* ns, max(tALS, tALH, tCH) */
	int act = 50;   /* ns, max(tRP, tWP) */
	int rec = 50;   /* ns, max(tREH, tWH) */
	int wait = 500; /* ns, max(tRR, tADL) */
	int rdst = 50;  /* ns, tREA */
	int tick = 1000000000 / CFG_NAND_HZ;

	val = ((twb / tick) + 1) & 0xff;
	writel(val, &regs->rbcr);

	val = (((cle / tick) + 1) & 0x0f) << 0;
	val |= (((ale / tick) + 1) & 0x0f) << 4;
	val |= (((act / tick) + 1) & 0x0f) << 8;
	val |= (((rec / tick) + 1) & 0x0f) << 12;
	val |= (((wait / tick) + 1) & 0xff) << 16;
	val |= (((rdst / tick) + 1) & 0xff) << 24;
#else
	//val = 0x1f1f999f;
	val = 0x18187c44;
#endif

	if (val)
		writel(val, &regs->actr0);
}
#endif

#define USE_NAND_EDO_TIMING
static int sp_nand_reset(struct sp_nand_info *info)
{
	struct sp_nand_regs *regs = info->regs;
	unsigned long timeout = jiffies + msecs_to_jiffies(50);
	int ret=0;
    uint32_t tRC;

    tRC = sp_clk_get(SP_CLK_SYSFAST)/1000000;
#ifdef USE_NAND_EDO_TIMING
    if(tRC >= 320 ) tRC = 0x18184444;
    else tRC = 0x18183344;// act_no=3+1= 4*3.225ns ~=12.9ns w/ EDO t1 setting
#else
    if(tRC >= 320 ) tRC = 0x18185944;
    else tRC = 0x18184844;// act_no=6+1= 7*3.225ns ~=22.xns w/o EDO setting
#endif

	writel(CSR_RESET, &regs->csr);
	while (jiffies < timeout) {
		if (!(readl(&regs->csr) & CSR_RESET))
			break;
		cpu_relax();
	}
	if (jiffies >= timeout) {
		printk("sp_nand: reset timeout\n");

		ret = -1;
	}

#ifdef USE_NAND_EDO_TIMING
	writel(CSR_EDO_TYPE_SET(2) | CSR_MODE_DESC | CSR_DUMMY_RD | CSR_NAND_EDO | CSR_EN, &regs->csr);
#else
	 writel(CSR_MODE_DESC | CSR_DUMMY_RD | CSR_EN, &regs->csr);
#endif

#ifdef CONFIG_MTD_SP_NAND_ACTR_OPTIMIZED
	sp_nand_acopt(info);
#elif defined(CONFIG_MTD_SP_NAND_ACTR_FAILSAFE)
	writel(0xffffffff, &regs->actr0);
#else
	writel(tRC, &regs->actr0);
#endif
	writel(readl(&regs->actr0), &regs->actr1);

#ifdef CONFIG_MTD_SP_NAND_SCRAMBLER
	writel(0, &regs->seed_cr);
	writel(1, &regs->rand_cr);
	printk("sp_nand: scrambler enabled\n");
#else
	printk("sp_nand: CSR:0x%x,AC timing-0:0x%x\n",readl(&regs->csr),readl(&regs->actr0));
#endif

	writel(ISR_DESC, &regs->ier);
	writel(ISR_DESC, &regs->isr);
	writel(24,&regs->tole_cnt_bit0);
	writel(24,&regs->tole_cnt);

	return ret;
}

static ssize_t sp_nand_sysfs_get_info(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct sp_nand_info *info = dev_get_drvdata(dev);
	struct mtd_info *mtd = &info->mtd;
	struct nand_chip *nand = mtd->priv;
	ssize_t ret = 0;

	ret += sprintf(buf + ret, "CS Number: %u\n",
		nand->numchips);
	ret += sprintf(buf + ret, "Chip Size: %u MB\n",
		(unsigned int)(nand->chipsize >> 20));
	ret += sprintf(buf + ret, "Block Size: %u KB\n",
		mtd->erasesize >> 10);
	ret += sprintf(buf + ret, "Page Size: %u\n", mtd->writesize);
	ret += sprintf(buf + ret, "OOB Size: %u\n", mtd->oobsize);
	ret += sprintf(buf + ret, "ECC Mode: %ux%u\n", nand->ecc.size,
		(nand->ecc.bytes << 3) / (12 + (nand->ecc.size >> 9)));

	return ret;
}

#ifdef CONFIG_MTD_SP_NAND_BKER_COUNTER

static ssize_t sp_nand_sysfs_get_erased(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct sp_nand_info *info = dev_get_drvdata(dev);
	struct mtd_partition *part;
	uint64_t offset, length, ertot;
	ssize_t ret = 0;
	int i, b, start, end, eravg, ermax, ermax_bk, ermin, ermin_bk;

	for (i = 1; i < info->nr_parts; ++i) {
		part = info->parts + i;

		offset = part->offset;
		length = part->size ? part->size : info->nand.chipsize - offset;

		start = (int)lldiv(offset, info->mtd.erasesize);
		end = (int)lldiv(offset + length - 1, info->mtd.erasesize);

		ertot = 0;
		ermax = -1;
		ermax_bk = start;
		ermin = 0x1ffff;
		ermin_bk = start;
		for (b = start; b <= end; ++b) {
			ertot += info->blk_erased[b];

			if (info->blk_erased[b] > ermax) {
				ermax = info->blk_erased[b];
				ermax_bk = b;
			}

			if (info->blk_erased[b] < ermin) {
				ermin = info->blk_erased[b];
				ermin_bk = b;
			}
		}

		eravg = lldiv(ertot, lldiv(length, info->mtd.erasesize));

		ret += sprintf(buf + ret, "mtd%u block erased status:\n", i);
		ret += sprintf(buf + ret, "    AVG. = %u (BLK NR=%u)\n",
			eravg, (unsigned int)lldiv(length, info->mtd.erasesize));
		ret += sprintf(buf + ret, "    Max. = %u (BLK ID=%u)\n",
			ermax, ermax_bk);
		ret += sprintf(buf + ret, "    Min. = %u (BLK ID=%u)\n",
			ermin, ermin_bk);
		ret += sprintf(buf + ret, "\n");
	}

	return ret;
}

#endif /* CONFIG_MTD_SP_NAND_BKER_COUNTER */

static struct device_attribute sp_nand_attrs[] = {
	__ATTR(info, S_IRUGO, sp_nand_sysfs_get_info, NULL),
#ifdef CONFIG_MTD_SP_NAND_BKER_COUNTER
	__ATTR(erased, S_IRUGO, sp_nand_sysfs_get_erased, NULL),
#endif
};

#ifdef CONFIG_SP_NAND_IOCTL
extern unsigned int *Get_SP_BCH_Info(void);

#ifndef SR_ERR_BITS(x)
#define SR_ERR_BITS(x)		(((x) >> 8) & 0x07FF)
#endif

#ifdef SR_ERR_MAX(x)
#define SR_ERR_MAX(x)		(((x) >> 20) & 0x7F)
#endif

#define BCH_ECC_BITS(x)		(((x) >> 8) & 0x07)

static long sp_nand_ioctl(struct file *filp, unsigned int cmd,
			  unsigned long arg)
{
	long ret = 0;
	int row;
	unsigned char nand_cmd;
	struct sp_nand_info *info = (struct sp_nand_info *)Get_SP_NAND_Info();
	struct sp_bch_chip *chip = (struct sp_bch_chip *)Get_SP_BCH_Info();
	struct sp_bch_regs *regs = chip->regs;
	struct sp_nand_req req;
	unsigned int bch_tmp;

	switch (cmd) {
	case SP_NAND_IOCREAD:
		if (copy_from_user((void *)&req, (const void __user *)arg, sizeof(req))) {
			ret = -EFAULT;
			break;
		}

		nand_cmd = NAND_CMD_READ0;
		info->cmd = nand_cmd;
		row = req.page_addr;
		// printk("IOCTL sp_nand read at page=%d\n", row);
		sp_nand_desc_prep(info, cmd, 0, row);
		sp_nand_desc_send(info);
		down(&info->spnand_sem);
		bch_tmp = readl(&regs->cr0);
		req.ecc_sts = BCH_ECC_BITS(bch_tmp); // allowed_err_bits

		switch (req.ecc_sts) {
		case 0:
			bch_tmp = 60;
			break;
		case 1:
			bch_tmp = 40;
			break;
		case 2:
			bch_tmp = 24;
			break;
		case 3:
			bch_tmp = 16;
			break;
		case 4:
			bch_tmp = 8;
			break;
		case 5:
			bch_tmp = 4;
			break;
		default:
			bch_tmp = 0xFFFE;
			break;
		}

		req.ecc_sts = bch_tmp;
		// printk("IOCTL bch ecc bits=0x%x\n", req.ecc_sts);
		ret = sp_bch_decode(&info->mtd, info->buff.phys,
				    info->buff.phys + info->mtd.writesize);
		if (ret) {
			printk("IOCTL sp_nand: bch decode failed at page=%d\n", row);
			req.max_err_bits = req.ecc_sts + 1;
			req.total_err_bits = req.max_err_bits;
		} else {
			bch_tmp = readl(&regs->sr);
			req.max_err_bits = SR_ERR_MAX(bch_tmp);
			req.total_err_bits = SR_ERR_BITS(bch_tmp);
		}
		req.ecc_sts |= (req.max_err_bits << 16);
		up(&info->spnand_sem);
		//printk("IOCTL sp_nand bch ecc max_err=%d,total_err=%d\n",req.max_err_bits,req.total_err_bits);

		if (copy_to_user((void __user *)arg, (void *)&req, sizeof(req)))
			ret = -EFAULT;

		break;

	case SP_NAND_IOCWRITE:
		printk("IOCTL sp_nand write\n");
		/* fall through */
	default:
		ret = -ENOTTY;
		break;
	}

	return ret;
}

static struct file_operations sp_nand_fops = {
	.owner          = THIS_MODULE,
	.unlocked_ioctl = sp_nand_ioctl,
};

static struct miscdevice sp_nand_dev = {
	.name = "sp_nand",
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &sp_nand_fops,
};
#endif

static int sp_nand_probe(struct platform_device *pdev)
{
	int i, ret = 0;
	struct resource *res;

	struct mtd_part_parser_data data;
	/* struct mtd_partition parts[] = {
		{
			.name = "header",
			.size = (2 << 20),
			.offset = 0,
		}, {
			.name = "other",
			.size = MTDPART_SIZ_FULL,
			.offset = (2 << 20),
		},
	}; */

	const char *part_types[] = {
#ifdef CONFIG_MTD_CMDLINE_PARTS
		"cmdlinepart",
#endif
		NULL
};

	// if (SYSCONFIG_MAINSTORAGE != "NAND") return 0;

	/*
	 * use reasonable defaults so platforms don't have to provide these.
	 * with DT probing on ARM, none of these are set.
	 */
	if (!pdev->dev.dma_mask)
		pdev->dev.dma_mask = &pdev->dev.coherent_dma_mask;
	if (!pdev->dev.coherent_dma_mask)
		pdev->dev.coherent_dma_mask = DMA_BIT_MASK(32);

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

  sp_nand_ftl_info = (unsigned int *)info;
  sema_init(&info->spnand_sem,1);
  mutex_init(&info->lock);
	// printk("\033[1;32m struct info addr:0x%p(for external:0x%p)\n\033[0m",info,sp_nand_ftl_info);

#ifdef CONFIG_MTD_SP_NAND_CBDMA
	info->dma = sp_cbdma_getbyname(CONFIG_MTD_SP_NAND_CBDMA_NAME);
	if (!info->dma) {
		printk("sp_nand: dma not found!\n");
		kfree(info);
		return -ENXIO;
	}
	// printk("sp_nand: cbus dma enabled\n");
#endif

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		kfree(info);
		return -ENXIO;
	}
	if (!request_mem_region(res->start, resource_size(res), "sp_nand")) {
		kfree(info);
		return -EBUSY;
	}

	platform_set_drvdata(pdev, info);
#ifdef CONFIG_SP_NAND_IOCTL
	ret = misc_register(&sp_nand_dev);
	if (ret < 0 )
		printk("Register sp_nand for IOCTL failed\n");
	else
		printk("Register sp_nand_ioctl in misc-device OK\n");
#endif

	info->hdr       = pdev->dev.platform_data;
	info->dev       = &pdev->dev;
	info->mtd.priv  = &info->nand;
	info->mtd.name  = dev_name(info->dev);
	info->mtd.owner = THIS_MODULE;

	init_waitqueue_head(&info->wq);

	info->regs = ioremap(res->start, resource_size(res));
	if (!info->regs) {
		ret = -ENOMEM;
		goto err1;
	}

	if (sp_nand_reset(info) < 0) {
		ret = -ENXIO;
		goto err1;
	}

	info->irq = platform_get_irq(pdev, 0);
	if (info->irq > 0 && request_irq(info->irq, sp_nand_irq, 0, "sp_nand", info)) {
		printk("sp_nand: unable to register IRQ(%d)\n", info->irq);
		return -EBUSY;
	}

	printk("sp_nand: mmio=0x%p, irq=%d\n", info->regs, info->irq);

	info->buff.size = CFG_BUFF_MAX;
	info->buff.phys = CONFIG_MTD_SP_NAND_SRAM_BASE;
	info->buff.virt = ioremap(info->buff.phys, info->buff.size);
	if (!info->buff.virt) {
		ret = -ENOMEM;
		goto err1;
	}

	printk("sp_nand: buff=0x%p@0x%08x size=%u\n",
		info->buff.virt, info->buff.phys, info->buff.size);
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
	info->buff1.size = CFG_BUFF_MAX;
	info->buff1.phys = CONFIG_MTD_SP_NAND_SRAM_BASE + CFG_BUFF_MAX;
	info->buff1.virt = ioremap(info->buff1.phys, info->buff1.size);

	printk("sp_nand: buff1=0x%p@0x%08x size1=%u\n",
		info->buff1.virt, info->buff1.phys, info->buff1.size);

    info->buff_flag   = USE_PP_BUF_0;
    info->buff_page   = -1;
    info->buff_state  = PP_BUF_IDLE;
    info->buff1_page  = -1;
    info->buff1_state = PP_BUF_IDLE;
#endif
	info->desc.size = CFG_DESC_MAX * 32;
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
	info->desc.phys = CONFIG_MTD_SP_NAND_SRAM_BASE + CFG_BUFF_MAX + CFG_BUFF_MAX;
#else
	info->desc.phys = CONFIG_MTD_SP_NAND_SRAM_BASE + CFG_BUFF_MAX;
#endif
	info->desc.virt = ioremap(info->desc.phys, info->desc.size);
	if (!info->desc.virt) {
		ret = -ENOMEM;
		goto err1;
	}

	printk("sp_nand: desc=0x%p@0x%08x size=%u\n",
		info->desc.virt, info->desc.phys, info->desc.size);

	info->nand.select_chip    = sp_nand_select_chip;
	info->nand.cmd_ctrl       = sp_nand_cmd_ctrl;
	info->nand.cmdfunc        = sp_nand_cmdfunc;
	info->nand.dev_ready      = sp_nand_dev_ready;
	info->nand.chip_delay     = 0;

	info->nand.read_byte      = sp_nand_read_byte;
	info->nand.read_buf       = sp_nand_read_buf;
	info->nand.write_buf      = sp_nand_write_buf;
	info->nand.verify_buf     = sp_nand_verify_buf;

	info->nand.ecc.read_page  = sp_nand_read_page;
	info->nand.ecc.write_page = sp_nand_write_page;
	info->nand.ecc.layout     = &info->ecc_layout;
	info->nand.ecc.mode       = NAND_ECC_HW;

	sp_nand_loadids(info, CFG_NAND_IDS_FILE);

	/**
	 * scan for NAND devices attached to the controller
	 * this is the first stage in a two step process to register
	 * with the nand subsystem
	 */

	if (nand_scan_ident(&info->mtd, CONFIG_MTD_SP_NAND_CSNR, info->ids)) {
		ret = -ENXIO;
		goto err1;
	}

	if (sp_nand_fixup(info) < 0) {
		ret = -ENXIO;
		goto err1;
	}

	printk("sp_nand: page=%u+%u, block=%uKB, chip=%lluMB (adrc=%u+%u)\n",
		info->mtd.writesize, info->mtd.oobsize, info->mtd.erasesize / SZ_1K,
		lldiv(info->nand.chipsize, SZ_1M), info->cac, info->rac);

	if (sp_bch_init(&info->mtd) < 0) {
		ret = -ENXIO;
		goto err1;
	}

	if (nand_scan_tail(&info->mtd)) {
		ret = -ENXIO;
		goto err1;
	}

#ifdef CONFIG_MTD_SP_NAND_BKER_COUNTER
	i = (int)lldiv(info->nand.chipsize, info->mtd.erasesize);
	info->blk_erased = kzalloc(2 * i, GFP_KERNEL);
	if (!info->blk_erased) {
		ret = -ENOMEM;
		goto err1;
	}
#endif

	/* create sysfs */
	for (i = 0; i < ARRAY_SIZE(sp_nand_attrs); ++i) {
		ret = device_create_file(info->dev, sp_nand_attrs + i);
		if (ret)
			goto err1;
	}

	data.origin = (unsigned long)info->hdr;
	/* ret = mtd_device_parse_register(&info->mtd, part_types, &data,
			parts, ARRAY_SIZE(parts)); */
	ret = mtd_device_parse_register(&info->mtd, part_types, &data, NULL, 0);

	return ret;
err1:
	release_mem_region(res->start, resource_size(res));

	if (info->regs)
		iounmap(info->regs);

	if (info->buff.virt)
		iounmap(info->buff.virt);

	if (info->desc.virt)
		iounmap(info->desc.virt);
#ifdef CONFIG_PM_WARP
        if (info->irq)
                free_irq(info->irq, info);
#endif
	kfree(info);

	return ret;
}

static int sp_nand_remove(struct platform_device *pdev)
{
	struct sp_nand_info *info = platform_get_drvdata(pdev);
	struct resource *res;
#ifdef CONFIG_SP_NAND_IOCTL
	misc_deregister(&sp_nand_dev);
#endif

	nand_release(&info->mtd);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res)
		release_mem_region(res->start, resource_size(res));

	if (info->regs)
		iounmap(info->regs);

	if (info->buff.virt)
		iounmap(info->buff.virt);

	if (info->desc.virt)
		iounmap(info->desc.virt);

	kfree(info);

	platform_set_drvdata(pdev, NULL);

	return 0;
}

#ifdef CONFIG_PM
static int sp_nand_suspend(struct platform_device *pdev, pm_message_t mesg)
{
	return 0;
}

static int sp_nand_resume(struct platform_device *pdev)
{
	struct sp_nand_info *info = platform_get_drvdata(pdev);
	sp_nand_reset(info);
	return 0;
}
#else
#define sp_nand_suspend	NULL
#define sp_nand_resume	NULL
#endif

/* work with "modprobe sp_nand" from hotplugging or coldplugging */
MODULE_ALIAS("platform:sp_nand");

static struct platform_driver sp_nand_driver = {
	.probe		= sp_nand_probe,
	.remove		= sp_nand_remove,
	.suspend	= sp_nand_suspend,
	.resume		= sp_nand_resume,
	.driver		= {
		.name	= "sp_nand",
		.owner	= THIS_MODULE,
	},
};

#if NAND_RW_DATASIZE_FOR_ISP
int zzNAND_proc_read( char *page, char **start, off_t off,
                   int count, int *eof, void *data )
{
  int len;

  if (off > 0) {
    *eof = 1;
    return 0;
  }


  len = sprintf(page,"%lld",Total_ISP_Write_Byte);

  // printk("len:%d,%s,Nand Write %lld Bytes\n",len,page,Total_ISP_Write_Byte);
	return len;
}

ssize_t zzNAND_proc_write( struct file *filp, const char __user *buff,
                        unsigned long len, void *data )
{
	unsigned char temp[80];

	memset(temp, 0, 80);

	if (copy_from_user(temp, buff, len)) {
		return -EFAULT;
	}

	temp[len-1] = 0;

	if (strcmp(temp,"clear") == 0)
	{
		Total_ISP_Write_Byte = 0;
	}

#if NAND_RW_DBG
	if (strcmp(temp,"addr_on") == 0){
		nand_addr_prompt = 1;
	}else if (strcmp(temp,"addr_off") == 0){
		nand_addr_prompt = 0;
	}

#endif
  return len;
}
#endif
static int __init sp_nand_init(void)
{
//	if(gbootRom_boot_mode != 0x1D) return 0; /* main storage is not NAND, return! */

	#if NAND_RW_DATASIZE_FOR_ISP
	proc_entry_zzNAND = create_proc_entry( "nand_rw_cnt", 0644, NULL );
	if (proc_entry_zzNAND == NULL)
	{
		printk("create proc for NAND is failed\n");
		return -ENOMEM;
	}
	else
	{
      proc_entry_zzNAND->read_proc = zzNAND_proc_read;
      proc_entry_zzNAND->write_proc = zzNAND_proc_write;
	}
#endif
	return platform_driver_register(&sp_nand_driver);
}

static void __exit sp_nand_exit(void)
{
// 	if (gbootRom_boot_mode != 0x1D) return; /* main storage is not NAND, return! */

	platform_driver_unregister(&sp_nand_driver);
}


EXPORT_SYMBOL(Get_SP_NAND_Info);

module_init(sp_nand_init);
module_exit(sp_nand_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Sunplus NAND flash controller");
