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
#include <nand.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
#include <asm/arch/regmap.h>

#include "sp_bch.h"
#include "sp_nand.h"  // SUPPORT_DOUBLE_BUFFER_PREFETCH
#include "sp_cbdma.h"

#ifndef CONFIG_SP_NAND_CSNR
#define CONFIG_SP_NAND_CSNR 1
#endif

/* #define CFG_BBT_USE_FLASH */
//#define CFG_NAND_HZ			clk_get_rate(clk_get_sys(NULL, "SYS"))
#define CFG_NAND_HZ			304000000 /* 304 MHz */
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
#define CFG_BUFF_MAX		(9 << 10) /* Noted that Nand Page size must <= 8K Byte & OOB <= 1K Byte for current ping-ping arch */
#else
#define CFG_BUFF_MAX		(18 << 10)
#endif
#define CFG_DESC_MAX		4
#define CFG_CMD_TIMEOUT_MS	10

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_SP_NFTL
unsigned int *sp_nand_ftl_info;
unsigned int* Get_SP_NAND_Info(void)
{
	return sp_nand_ftl_info;
}
#endif
#ifndef MAX
#define MAX(a, b)			((a) > (b) ? (a) : (b))
#endif

unsigned int isp_ng_blk_addr = 0; // ISP BCH decode fail block address
int snand_bad_block_cnt;

#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
int g_row;
#endif

#ifdef CONFIG_SP_ERASECNT_TABLE
void GetEraseCntTableInfo(void);
void DumpEraseCntTable(void);
#endif

static struct sp_nand_info our_nfc = {
	.regs = (void __iomem *)CONFIG_SP_NAND_BASE,
};

static int sp_nand_reset(struct sp_nand_info *info);

#if defined(CONFIG_SP_PNG_DECODER)
int sp_png_dec_run(void);
#endif

static int sp_nand_wait(struct sp_nand_info *info)
{
	struct sp_nand_regs *regs = info->regs;
	unsigned long now = get_timer(0);
	int ret = -1;

	while (get_timer(now) < CFG_CMD_TIMEOUT_MS) {
#if defined(CONFIG_SP_PNG_DECODER)
		sp_png_dec_run();
#endif

		if (!(readl(&regs->csr) & CSR_DESC_FETCH)) {
			ret = 0;
			break;
		}
	}

	return ret;
}

int sp_nand_desc_prep(struct sp_nand_info *info,
	uint8_t cmd, int col, int row)
{
	struct sp_nand_desc *desc = info->desc.virt;
	int mfs = (info->cs < 0) ? BIT_MASK(0) : BIT_MASK(info->cs);

	memset(info->desc.virt, 0, info->desc.size);

	desc->ier     = ISR_DESC;
	desc->mfs     = mfs;
	desc->end     = 1;
	desc->owner   = 1; /* HW */
	desc->cs_ctrl = 1;
	desc->desc_sz = sizeof(struct sp_nand_desc);

#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
if (cmd==NAND_CMD_READOOB || cmd==NAND_CMD_READ0)
	if ((cmd == NAND_CMD_READ0) && (row == info->prefetch_row)) return 0;
#endif
	switch (cmd) {
	case NAND_CMD_READOOB:	/* 0x50 */
		col += info->mtd->writesize;
		/* fall through */
	case NAND_CMD_READ0:	/* 0x00 */
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
		if (info->nand_working_buff_flag == 0)
		{
			info->buff0.idx = col;
		}
		else
		{
			info->buff1.idx = col;
		}
#else
		info->buff.idx = col;
#endif
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
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
		g_row = row;
#endif
		if (col < info->mtd->writesize) {
			desc->oob_en = 1;
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
			if (info->nand_working_buff_flag == 0)
			{
				desc->buf_dma = info->buff0.phys;
				desc->buf_ssz = info->mtd->writesize - 1;
				desc->oob_dma = info->buff0.phys + info->mtd->writesize;
				desc->oob_ssz = info->mtd->oobsize - 1;
			}
			else
			{
				desc->buf_dma = info->buff1.phys;
				desc->buf_ssz = info->mtd->writesize - 1;
				desc->oob_dma = info->buff1.phys + info->mtd->writesize;
				desc->oob_ssz = info->mtd->oobsize - 1;
			}
#else
			desc->buf_dma = info->buff.phys;
			desc->buf_ssz = info->mtd->writesize - 1;
			desc->oob_dma = info->buff.phys + info->mtd->writesize;
			desc->oob_ssz = info->mtd->oobsize - 1;
#endif
		} else {
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
			if (info->nand_working_buff_flag == 0)
			{
				desc->buf_dma = info->buff0.phys + info->mtd->writesize;
				desc->buf_ssz = info->mtd->oobsize - 1;
			}
			else
			{
				desc->buf_dma = info->buff1.phys + info->mtd->writesize;
				desc->buf_ssz = info->mtd->oobsize - 1;
			}
#else
			desc->buf_dma = info->buff.phys + info->mtd->writesize;
			desc->buf_ssz = info->mtd->oobsize - 1;
#endif

#ifdef CONFIG_SP_MTD_SCAN_BBT_FAST
            struct mtd_info *mtd = info->mtd;
            if(mtd->scan_oob_fast && col )
                desc->buf_ssz = info->nand.badblock_pattern->len - 1;
// DEBUG:   printf("desc->buf_ssz = %d\n",desc->buf_ssz);
#endif
		}
		desc->sect_nr = 0;
#ifdef CONFIG_SP_NAND_SCRAMBLER
		if (cmd == NAND_CMD_READ0)
		desc->rand_en = 1;
#endif

#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
		if (cmd == NAND_CMD_READOOB)
		{
			if (info->nand_working_buff_flag == 0) info->nand_working_buff_flag = 1;
			else info->nand_working_buff_flag = 0;
		}
#endif
		break;

	case NAND_CMD_SEQIN:	/* 0x80 */
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
		if (info->nand_working_buff_flag == 0)
		{
			info->buff0.idx = col;
		}
		else
		{
			info->buff1.idx = col;
		}
#else
		info->buff.idx = col;
#endif
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
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
		if (info->nand_working_buff_flag == 0)
		{
			desc->buf_dma = info->buff0.phys;
			desc->buf_ssz = info->mtd->writesize - 1;
			desc->oob_dma = info->buff0.phys + info->mtd->writesize;
			desc->oob_ssz = info->mtd->oobsize - 1;
		}
		else
		{
			desc->buf_dma = info->buff1.phys;
			desc->buf_ssz = info->mtd->writesize - 1;
			desc->oob_dma = info->buff1.phys + info->mtd->writesize;
			desc->oob_ssz = info->mtd->oobsize - 1;
		}
#else
		desc->buf_dma = info->buff.phys;
		desc->buf_ssz = info->mtd->writesize - 1;
		desc->oob_dma = info->buff.phys + info->mtd->writesize;
		desc->oob_ssz = info->mtd->oobsize - 1;
#endif
		desc->sect_nr = 0;
#ifdef CONFIG_SP_NAND_SCRAMBLER
		desc->rand_en = 1;
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
		desc->ctrl = BIT_MASK(1) | BIT_MASK(3); /* address + data read */
		desc->addr2 = (uint8_t)col;
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
		if (info->nand_working_buff_flag == 0)
		{
			desc->buf_dma = info->buff0.phys;
		}
		else
		{
			desc->buf_dma = info->buff1.phys;
		}
#else
		desc->buf_dma = info->buff.phys;
#endif
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
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
		if (info->nand_working_buff_flag == 0)
		{
			info->buff0.idx = 0;
			memset(info->buff0.virt, 0xff, desc->buf_ssz);
		}
		else
		{
			info->buff1.idx = 0;
			memset(info->buff1.virt, 0xff, desc->buf_ssz);
		}
#else
		info->buff.idx = 0;
		memset(info->buff.virt, 0xff, desc->buf_ssz);
#endif
		break;

	case NAND_CMD_SET_FEATURES: /* 0xEF */
		desc->cmd = SP_NAND_CMD_SUPER;
		desc->cmd0 = cmd;
		desc->ctrl = BIT(1) | BIT(2) | BIT(4); /* addr + R/B + write */
		desc->addr2 = (uint8_t)col;
		desc->buf_ssz = 4;
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
		if (info->nand_working_buff_flag == 0)
		{
			desc->buf_dma = info->buff0.phys;
			info->buff0.idx = 0;
			memset(info->buff0.virt, 0xff, desc->buf_ssz);
		}
		else
		{
			desc->buf_dma = info->buff0.phys;
			info->buff1.idx = 0;
			memset(info->buff1.virt, 0xff, desc->buf_ssz);
		}
#else
		desc->buf_dma = info->buff.phys;
		info->buff.idx = 0;
		memset(info->buff.virt, 0xff, desc->buf_ssz);
#endif
		break;

	case NAND_CMD_RESET:	/* 0xFF */
		desc->cmd = SP_NAND_CMD_SUPER;
		desc->cmd0 = 0xff;
		desc->ctrl = BIT_MASK(2); /* R/B# */
		break;

	default:
		break;
	}

	return 0;
}

int sp_nand_desc_send(struct sp_nand_info *info, int need_wait)
{
	struct sp_nand_regs *regs = info->regs;
	int ret;

    if(info->cmd == NAND_CMD_STATUS ) 	// Mason@2016-02-16 dis-able EDO mode for NAND Status Read,CMD=0x70
        clrbits_le32(&regs->csr,CSR_NAND_EDO);

	writel(info->desc.phys, &regs->dbar);
	setbits_le32(&regs->csr, CSR_DESC_FETCH);

#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
	if (need_wait)
	{
		ret = sp_nand_wait(info);
		if (ret) {
			pr_info("sp_nand: descriptor timeout, cmd=0x%x\n", info->cmd);
			sp_nand_reset(info);
		}
	}
	else
	{
		ret = 0;
	}
#else
	ret = sp_nand_wait(info);
	if (ret) {
		pr_info("sp_nand: descriptor timeout, cmd=0x%x\n", info->cmd);
		sp_nand_reset(info);
	}
#endif
    if(info->cmd == NAND_CMD_STATUS )
        setbits_le32(&regs->csr,CSR_NAND_EDO);
	return ret;
}

#ifdef CONFIG_SP_NAND_SCRAMBLER

static void sp_nand_get_oob_scrambled(struct mtd_info *mtd, int row)
{
	struct sp_nand_info *info = &our_nfc;
	struct sp_nand_desc *desc = info->desc.virt;
	int mfs = (info->cs < 0) ? BIT_MASK(0) : BIT_MASK(info->cs);

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
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
	if (info->nand_working_buff_flag == 0)
	{
		desc->buf_dma = info->buff0.phys;
		desc->buf_ssz = info->mtd->writesize - 1;
		desc->oob_dma = info->buff0.phys + info->mtd->writesize;
		desc->oob_ssz = info->mtd->oobsize - 1;
	}
	else
	{
		desc->buf_dma = info->buff1.phys;
		desc->buf_ssz = info->mtd->writesize - 1;
		desc->oob_dma = info->buff1.phys + info->mtd->writesize;
		desc->oob_ssz = info->mtd->oobsize - 1;
	}
#else
	desc->buf_dma = info->buff.phys;
	desc->buf_ssz = info->mtd->writesize - 1;
	desc->oob_dma = info->buff.phys + info->mtd->writesize;
	desc->oob_ssz = info->mtd->oobsize - 1;
#endif
	desc->sect_nr = 0;
	desc->rand_en = 1;

	sp_nand_desc_send(info, 1);
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
//		if (cmd == NAND_CMD_READOOB)
		{
			if (info->nand_working_buff_flag == 0) info->nand_working_buff_flag = 1;
			else info->nand_working_buff_flag = 0;
		}
#endif
}

#endif /* CONFIG_SP_NAND_SCRAMBLER */

void francis_prefetch_next_page(void)
{
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
	struct sp_nand_info *info = &our_nfc;

	if (info->nand_working_buff_flag == 0) info->nand_working_buff_flag = 1;
	else info->nand_working_buff_flag = 0;

	sp_nand_desc_prep(info, NAND_CMD_READ0, 0, g_row + 1);
	sp_nand_desc_send(info, 0);

	info->prefetch_row = g_row;
#endif
}

#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
int PingPongBuf_0_PageReadCmdSend(struct sp_nand_info *info, uint8_t cmd, int col, int row)
{
	struct sp_nand_desc *desc = info->desc.virt;
	struct sp_nand_regs *regs = info->regs;
	int ret;

	int mfs = (info->cs < 0) ? BIT_MASK(0) : BIT_MASK(info->cs);

	if (row == info->buff_page && info->buff_state > PP_BUF_IDLE) {
		/* DBG printf("b0-row:%d pre-fetched\n",row); */
		return 0;
	}

	info->buff_state = PP_BUF_IDLE;
	/* DBG  printf("Re-Read b0-row:%d,%d\n",row,info->buff_page); */
	info->buff_page = row;

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
	desc->buf_ssz = info->mtd->writesize - 1;
	desc->oob_dma = info->buff.phys + info->mtd->writesize;
	desc->oob_ssz = info->mtd->oobsize - 1;

	desc->oob_en = 1;

	writel(info->desc.phys, &regs->dbar);
	setbits_le32(&regs->csr, CSR_DESC_FETCH);

	info->buff_state = PP_BUF_CMD_SEND;

#if defined(CONFIG_SP_PNG_DECODER)
	sp_png_dec_run();
#endif

}

int PingPongBuf_1_PageReadCmdSend(struct sp_nand_info *info, uint8_t cmd, int col, int row)
{
	struct sp_nand_desc *desc = info->desc.virt;
	struct sp_nand_regs *regs = info->regs;

	int mfs = (info->cs < 0) ? BIT_MASK(0) : BIT_MASK(info->cs);

	if (row == info->buff1_page && info->buff1_state > PP_BUF_IDLE) {
		/* DBG  printf("b1-row:%d pre-fetched\n",row); */
		return 0;
	}

	info->buff1_state = PP_BUF_IDLE;
	/* DBG  printf("Re Read b1-row:%d,%d\n",row,info->buff1_page); */
	info->buff1_page = row;

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
	desc->buf_ssz = info->mtd->writesize - 1;
	desc->oob_dma = info->buff1.phys + info->mtd->writesize;
	desc->oob_ssz = info->mtd->oobsize - 1;

	desc->oob_en = 1;

	writel(info->desc.phys, &regs->dbar);
	setbits_le32(&regs->csr, CSR_DESC_FETCH);

	info->buff1_state = PP_BUF_CMD_SEND;

#if defined(CONFIG_SP_PNG_DECODER)
	sp_png_dec_run();
#endif

}
#endif

static int sp_nand_read_page(struct mtd_info *mtd,
	struct nand_chip *chip, uint8_t *buf, int oob_required, int page)
{
	struct sp_nand_info *info = &our_nfc;
	int ret;
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
	ret = sp_nand_wait(info);
	if (ret) {
		pr_info("sp_nand: descriptor timeout, cmd=0x%x\n", info->cmd);
		sp_nand_reset(info);
		return 0;
	}

	if (info->buff_flag == USE_PP_BUF_0) {
		info->buff_state = PP_BUF_DATA_RDY; /* buff-0 data rdy */
		PingPongBuf_1_PageReadCmdSend(info, NAND_CMD_READ0, 0, page + 1);
		ret = sp_bch_decode(mtd, info->buff.virt,info->buff.virt + mtd->writesize);
		if (ret) {
#ifdef CONFIG_SP_NAND_PAGE_RELOAD
#ifdef SP_NAND_RELOAD_RETURN_ZERO
            static int chkpage = -1, chkcnt = 0;
#endif
            ret = sp_bch_decode(mtd, info->buff.virt,
                    info->buff.virt + mtd->writesize);
            if (ret) {
#ifdef SP_NAND_RELOAD_RETURN_ZERO
                if (chkpage != page) {
                    chkpage = page;
                    chkcnt++;
                } else {
                    chkcnt = 0;
                    chkpage = -1;
                }
                if ((chkpage == page) && (chkcnt == 1))
#endif
                {
                    info->buff_state = PP_BUF_IDLE;
                    printf("PP0:bch decode failed twice at page=%d\n", page);
                    isp_ng_blk_addr = ((page * info->mtd->writesize)/info->mtd->erasesize) * info->mtd->erasesize;
                    printf("Mardbad Blk at 0x%x\n", isp_ng_blk_addr);
                    return -1;
                }
            }
#endif
			printf("PP-Buf0 sp_nand: bch decode failed at page=%d\n", page);
			isp_ng_blk_addr = ((page * info->mtd->writesize)/info->mtd->erasesize) * info->mtd->erasesize;
			printf("Mardbad Blk at 0x%x\n", isp_ng_blk_addr);
        }
		info->buff_state = PP_BUF_DECODE_RDY; /* buff-0 decode rdy */
	} else {
		info->buff1_state = PP_BUF_DATA_RDY; /* buff-1 data rdy */
		PingPongBuf_0_PageReadCmdSend(info, NAND_CMD_READ0, 0, page + 1);
		ret = sp_bch_decode(mtd, info->buff1.virt,info->buff1.virt + mtd->writesize);
		if (ret) {
#ifdef CONFIG_SP_NAND_PAGE_RELOAD
#ifdef SP_NAND_RELOAD_RETURN_ZERO
            static int chkpage1 = -1, chkcnt1 = 0;
#endif
            ret = sp_bch_decode(mtd, info->buff1.virt,
                    info->buff1.virt + mtd->writesize);
            if (ret) {
#ifdef SP_NAND_RELOAD_RETURN_ZERO
                if (chkpage1 != page) {
                    chkpage1 = page;
                    chkcnt1++;
                } else {
                    chkcnt1 = 0;
                    chkpage1 = -1;
                }
                if ((chkpage1 == page) && (chkcnt1 == 1))
#endif
                {
                    info->buff1_state = PP_BUF_IDLE;
                    printf("PP1:bch decode failed twice at page=%d\n", page);
                    isp_ng_blk_addr = ((page * info->mtd->writesize)/info->mtd->erasesize) * info->mtd->erasesize;
                    printf("Mardbad Blk at 0x%x\n", isp_ng_blk_addr);
                    return -1;
                }
            }
#endif

			printf("PP-Buf-1 sp_nand: bch decode failed at page=%d\n", page);
			isp_ng_blk_addr = ((page * info->mtd->writesize)/info->mtd->erasesize) * info->mtd->erasesize;
			printf("Mardbad Blk at 0x%x\n", isp_ng_blk_addr);
        }
		info->buff1_state = PP_BUF_DECODE_RDY; /* buff-1 decode rdy */
	}

	chip->read_buf(mtd, buf, mtd->writesize);

	if (!oob_required || page < 0)
		return 0;

	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);
	return 0;
#endif

#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
	if (info->prefetch_row != -1)
	{
		int ret;
		ret = sp_nand_wait(info);
		if (ret) {
			pr_info("sp_nand: descriptor timeout, cmd=0x%x\n", info->cmd);
			sp_nand_reset(info);
		}
		info->prefetch_row = -1;
	}

	if (info->nand_working_buff_flag == 0)
	{
		ret = sp_bch_decode(mtd, info->buff0.virt,
				info->buff0.virt + mtd->writesize);
	}
	else
	{
		ret = sp_bch_decode(mtd, info->buff1.virt,
				info->buff1.virt + mtd->writesize);
	}
#else
	ret = sp_bch_decode(mtd, info->buff.virt,
			info->buff.virt + mtd->writesize);
#ifdef CONFIG_SP_NAND_PAGE_RELOAD
#ifdef SP_NAND_RELOAD_RETURN_ZERO
    static int chkpage = -1, chkcnt = 0;
#endif
    if (ret) {
        ret = sp_bch_decode(mtd, info->buff.virt,
                info->buff.virt + mtd->writesize);
        if (ret) {
#ifdef SP_NAND_RELOAD_RETURN_ZERO
            if (chkpage != page) {
                chkpage = page;
                chkcnt++;
            } else {
                chkcnt = 0;
                chkpage = -1;
            }
            if ((chkpage == page) && (chkcnt == 1))
#endif
            {
                printf("sp_nand:bch decode failed twice at page=%d\n", page);
                isp_ng_blk_addr = ((page * info->mtd->writesize)/info->mtd->erasesize) * info->mtd->erasesize;
                printf("Mardbad Blk at 0x%x\n", isp_ng_blk_addr);
                return -1;
            }
        }
    }
#endif
#endif
	if (ret){
		printf("sp_nand: bch decode failed at page=%d\n", page);
		isp_ng_blk_addr = ((page * info->mtd->writesize)/info->mtd->erasesize) * info->mtd->erasesize;
		printf("Mardbad Blk at 0x%x\n", isp_ng_blk_addr);
	}

	chip->read_buf(mtd, buf, mtd->writesize);

	if (!oob_required || page < 0)
		return 0;

	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);

	return 0;
}

static int sp_nand_write_page(struct mtd_info *mtd, struct nand_chip *chip,
	const uint8_t *buf, int oob_required)
{
	struct sp_nand_info *info = &our_nfc;

	chip->write_buf(mtd, buf, mtd->writesize);
	chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);

#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
	if (info->nand_working_buff_flag == 0)
	{
		sp_bch_encode(mtd, info->buff0.virt, info->buff0.virt + mtd->writesize);
	}
	else
	{
		sp_bch_encode(mtd, info->buff1.virt, info->buff1.virt + mtd->writesize);
	}
#else
	sp_bch_encode(mtd, info->buff.virt, info->buff.virt + mtd->writesize);
#endif
	return 0;
}

static int sp_nand_verify_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	void *tmp = malloc(mtd->writesize);
	int ret = 0;

	if (!tmp)
		return -ENOMEM;

	sp_nand_read_page(mtd, mtd->priv, tmp, 0, -1);
	if (memcmp(buf, tmp, len)) {
		printf("sp_nand: verify failed\n");
		ret = -EFAULT;
	}

	free(tmp);
	return ret;
}

static int sp_nand_dev_ready(struct mtd_info *mtd)
{
	struct sp_nand_info *info = &our_nfc;
	struct sp_nand_regs *regs = info->regs;

	return (readl(&regs->isr) & (ISR_BUS_BUSY | ISR_HW_BUSY)) == 0;
}

static uint8_t sp_nand_read_byte(struct mtd_info *mtd)
{
	struct sp_nand_info *info = &our_nfc;
	struct sp_nand_desc *desc = info->desc.virt;
	uint8_t ret = 0;

	switch (info->cmd) {
	case NAND_CMD_STATUS:
		ret = desc->ctrl;
		break;

	default:
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
		if (info->nand_working_buff_flag == 0)
		{
			if (info->buff0.idx < info->buff0.size) {
				ret = readb(info->buff0.virt + info->buff0.idx);
				info->buff0.idx += 1;
			}
		}
		else
		{
			if (info->buff1.idx < info->buff1.size) {
				ret = readb(info->buff1.virt + info->buff1.idx);
				info->buff1.idx += 1;
			}
		}
#else
		if (info->buff.idx < info->buff.size) {
			ret = readb(info->buff.virt + info->buff.idx);
			info->buff.idx += 1;
		}
#endif
		break;
	}

	return ret;
}

static void sp_nand_read_buf(struct mtd_info *mtd,
	u_char *buf, int len)
{
	struct sp_nand_info *info = &our_nfc;

#ifdef CONFIG_SP_MTD_SCAN_BBT_FAST
    if(mtd->scan_oob_fast){
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
	if (info->nand_working_buff_flag == 0)
	{
		memcpy(buf, info->buff1.virt + info->buff1.idx, len);
		info->buff1.idx += len;
	}
	else
	{
		memcpy(buf, info->buff0.virt + info->buff0.idx, len);
		info->buff0.idx += len;
	}
#else
	memcpy(buf, info->buff.virt + info->buff.idx, len);
	info->buff.idx += len;
#endif
    return;
    }
#endif

#ifdef CONFIG_SP_CBDMA
	unsigned long start = (unsigned long)buf;
	unsigned long stop = start + len;

#ifdef CONFIG_SP_NAND_READ_HELP_FLUSH_BUF
	flush_dcache_range(start, stop);
#else
	invalidate_dcache_range(start, stop);
#endif

#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
	if (info->nand_working_buff_flag == 0)
	{
		sp_cbdma_write(info->buff1.phys + info->buff1.idx,
			virt_to_phys(buf), len);
		info->buff1.idx += len;
	}
	else
	{
		sp_cbdma_write(info->buff0.phys + info->buff0.idx,
			virt_to_phys(buf), len);
		info->buff0.idx += len;
	}
#else
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
	struct sp_nand_regs *regs = info->regs;
	if (info->buff_flag == USE_PP_BUF_0) {
		if (info->buff_state == PP_BUF_CMD_SEND)
			while (readl(&regs->csr) & CSR_DESC_FETCH);
#endif
	sp_cbdma_write(info->buff.phys + info->buff.idx,
		virt_to_phys(buf), len);
	info->buff.idx += len;
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
		info->buff_state = PP_BUF_IDLE; /* buf-0 back to idle */
	} else {
		if (info->buff1_state == PP_BUF_CMD_SEND)
			while (readl(&regs->csr) & CSR_DESC_FETCH);

		sp_cbdma_write(info->buff1.phys + info->buff1.idx,
			virt_to_phys(buf), len);
		info->buff1.idx += len;
		info->buff1_state = PP_BUF_IDLE; /* buf-1 back to idle */
	}
#endif
#endif
#else
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
	if (info->working_nand_buff_flag == 0)
	{
		memcpy(buf, info->buff1.virt + info->buff1.idx, len);
		info->buff1.idx += len;
	}
	else
	{
		memcpy(buf, info->buff0.virt + info->buff0.idx, len);
		info->buff0.idx += len;
	}
#else
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
	struct sp_nand_regs *regs = info->regs;
		if (info->buff_flag == USE_PP_BUF_0) {
			if (info->buff_state == PP_BUF_CMD_SEND)
				while (readl(&regs->csr) & CSR_DESC_FETCH);
#endif
	memcpy(buf, info->buff.virt + info->buff.idx, len);
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
		info->buff_state = PP_BUF_IDLE; /* buf-0 back to idle */
	} else {
		if (info->buff1_state == PP_BUF_CMD_SEND)
			while (readl(&regs->csr) & CSR_DESC_FETCH);

		memcpy(buf, info->buff1.virt + info->buff1.idx, len);
		info->buff1.idx += len;
		info->buff1_state = PP_BUF_IDLE; /* buf-1 back to idle */
		return;
	}
#endif
	info->buff.idx += len;
#endif
#endif
}

static void sp_nand_write_buf(struct mtd_info *mtd,
	const u_char *buf, int len)
{
	struct sp_nand_info *info = &our_nfc;
#ifdef CONFIG_SP_CBDMA
	unsigned long start = (unsigned long)buf;
	unsigned long stop = start + len;

	flush_dcache_range(start, stop);
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
	if (info->nand_working_buff_flag == 0)
	{
		sp_cbdma_read(virt_to_phys((void *)buf),
			info->buff0.phys + info->buff0.idx, len);
		info->buff0.idx += len;
	}
	else
	{
		sp_cbdma_read(virt_to_phys((void *)buf),
			info->buff1.phys + info->buff1.idx, len);
		info->buff1.idx += len;
	}
#else
	sp_cbdma_read(virt_to_phys((void *)buf),
		info->buff.phys + info->buff.idx, len);
	info->buff.idx += len;
#endif
#else
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
	if (info->working_nand_buff_flag == 0)
	{
		memcpy(info->buff0.virt + info->buff0.idx, buf, len);
		info->buff0.idx += len;
	}
	else
	{
		memcpy(info->buff1.virt + info->buff1.idx, buf, len);
		info->buff1.idx += len;
	}
#else
	memcpy(info->buff.virt + info->buff.idx, buf, len);
	info->buff.idx += len;
#endif
#endif

	if (info->cmd == NAND_CMD_SET_FEATURES)
		sp_nand_desc_send(info, 1);
}

static void sp_nand_select_chip(struct mtd_info *mtd, int chipnr)
{
	struct sp_nand_info *info = &our_nfc;

	switch (chipnr) {
	case 0:
	case 1:
	case 2:
	case 3:
		info->cs = chipnr;
		break;
	default:
		break;
	}
}

#ifdef CONFIG_SP_NAND_ACTR_OPTIMIZED
static void sp_nand_acopt(struct sp_nand_info *info)
{
	struct sp_nand_regs *regs = info->regs;
	uint32_t val = 0;

	/* Samsung K9GBG08U0B */
	int tCLS = 12;
	int tCS = 20;
	int tWP = 11;
	int tCH = 5;
	int tCLH = 5;
	int tALS = 12;
	int tRC = 25;
	int tREH = 11;
	int tWH = 11;
	int tRR = 20;
	int tWHR = 120;
	int tWB = 100;
	int tick = 1000000000 / CFG_NAND_HZ;

	/* SP_NAND AC Timing */
	int cle = MAX(MAX(tCLS, tCS) - tWP, MAX(tCH, tCLH)) / tick;
	int ale = (tALS - tWP) / tick;
	int act = MAX(tWP, tRC - tREH) / tick;
	int rec = MAX(tREH, tWH) / tick;
	int wait = tRR / tick;
	int rdst = (tWHR - cle) / tick;

	val = ((cle + 1) / 2) & 0x0f;
	val |= (ale & 0x0f) << 4;
	val |= (act & 0x0f) << 8;
	val |= (rec & 0x0f) << 12;
	val |= (wait & 0xff) << 16;
	val |= (rdst & 0xff) << 24;

	writel((tWB / tick) & 0xff, &regs->rbcr);

	if (val)
		writel(val, &regs->actr0);
}
#endif

static int sp_nand_reset(struct sp_nand_info *info)
{
	struct sp_nand_regs *regs = info->regs;
	unsigned long now = get_timer(0);
	int ret = -1;

	unsigned int tRC;
    tRC = clk_get_rate(clk_get_sys(NULL, "SYS")) / 1000000;
    if(tRC >= 320) tRC = 0x18184444;
    else tRC = 0x18183344;

	writel(CSR_RESET, &regs->csr);
	while (get_timer(now) < CFG_CMD_TIMEOUT_MS) {
		if (!(readl(&regs->csr) & CSR_RESET)) {
			ret = 0;
			break;
		}
	}
	if (ret) {
		printf("sp_nand: reset timeout\n");
		ret = -1;
	}

	writel(CSR_EDO_TYPE_SET(2) | CSR_MODE_DESC | CSR_DUMMY_RD | CSR_NAND_EDO | CSR_EN, &regs->csr);

#ifdef CONFIG_SP_NAND_ACTR_OPTIMIZED
	sp_nand_acopt(info);
#elif defined(CONFIG_SP_NAND_ACTR_FAILSAFE)
	writel(0xffffffff, &regs->actr0);
#else
	writel(tRC, &regs->actr0);
#endif
	writel(readl(&regs->actr0), &regs->actr1);

#ifdef CONFIG_SP_NAND_SCRAMBLER
	writel(0, &regs->seed_cr);
	writel(1, &regs->rand_cr);
	pr_info("sp_nand: scrambler enabled\n");
#else
    printf("sp_nand: CSR:0x%x,AC-timing0:0x%x,AC-timing1:0x%x\n",readl(&regs->csr),readl(&regs->actr0),readl(&regs->actr1));
#endif

	writel(ISR_DESC, &regs->ier);
	writel(ISR_DESC, &regs->isr);

	return ret;
}

static void sp_nand_cmd_ctrl(struct mtd_info *mtd,
	int cmd, unsigned int ctrl)
{
	return;
}

static void sp_nand_cmdfunc(struct mtd_info *mtd,
	unsigned cmd, int col, int row)
{
	struct sp_nand_info *info = &our_nfc;
	struct sp_nand_regs *regs = info->regs;
	uint32_t val;

	info->cmd = cmd;

	switch (cmd)  {
	case NAND_CMD_READ0:	/* 0x00 */
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
		if (row != info->prefetch_row)
		{
			if (info->prefetch_row != -1)
			{
				int ret;
				ret = sp_nand_wait(info);
				if (ret) {
					pr_info("sp_nand: descriptor timeout, cmd=0x%x\n", info->cmd);
					sp_nand_reset(info);
				}
				info->prefetch_row = -1;
			}
		sp_nand_desc_prep(info, cmd, col, row);

			sp_nand_desc_send(info, 1);
			if (info->nand_working_buff_flag == 0) {
				if (readb(info->buff0.virt + mtd->writesize + 1) == 0xff) break;
			} else {
				if (readb(info->buff1.virt + mtd->writesize + 1) == 0xff) break;
			}
		}
#else
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
		while (readl(&regs->csr) & CSR_DESC_FETCH);
		if (row%2) {
			/* DBG printf("row=%d ,BUF-1,",row); */
			info->buff_flag = USE_PP_BUF_1;
			PingPongBuf_1_PageReadCmdSend(info, cmd, 0, row);
		} else {
			/* DBG printf("row=%d ,BUF-0,",row); */
			info->buff_flag = USE_PP_BUF_0;
			PingPongBuf_0_PageReadCmdSend(info, cmd, 0, row);
		}
		break;
#endif
		sp_nand_desc_prep(info, cmd, col, row);
		sp_nand_desc_send(info, 1);
		if (readb(info->buff.virt + mtd->writesize + 1) == 0xff) break;
#endif
		break;
	case NAND_CMD_READOOB:	/* 0x50 */
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
		info->nand_working_buff_flag = 0;
#endif
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
		info->buff_flag = USE_PP_BUF_0;
		info->buff_state = PP_BUF_IDLE;
#endif
		sp_nand_desc_prep(info, cmd, col, row);
		sp_nand_desc_send(info, 1);
#ifdef CONFIG_SP_NAND_SCRAMBLER
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
		if (info->nand_working_buff_flag == 0)
		{
			if (readb(info->buff1.virt + mtd->writesize + 1) != 0xff)
			{
				info->nand_working_buff_flag = 0;
				sp_nand_get_oob_scrambled(mtd, row);
			}
		}
		else
		{
			if (readb(info->buff0.virt + mtd->writesize + 1) != 0xff)
			{
				info->nand_working_buff_flag = 0;
				sp_nand_get_oob_scrambled(mtd, row);
			}
		}
#else
		if (readb(info->buff.virt + mtd->writesize + 1) != 0xff)
			sp_nand_get_oob_scrambled(mtd, row);
#endif
#endif
		break;

	case NAND_CMD_SEQIN:	/* 0x80 */
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
		while (readl(&regs->csr) & CSR_DESC_FETCH);
		info->buff_state  = PP_BUF_IDLE;
#endif
		sp_nand_desc_prep(info, cmd, col, row);
		break;

	case NAND_CMD_PAGEPROG:	/* 0x10 */
#ifdef CONFIG_SP_NAND_SCRAMBLER
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
		if (info->nand_working_buff_flag == 0)
		{
			writeb(0xff, info->buff0.virt + mtd->writesize + 1);
		}
		else
		{
			writeb(0xff, info->buff1.virt + mtd->writesize + 1);
		}
#else
		writeb(0xff, info->buff.virt + mtd->writesize + 1);
#endif
#endif
		sp_nand_desc_send(info, 1);
		break;

	case NAND_CMD_ERASE1:	/* 0x60 */
		sp_nand_desc_prep(info, cmd, col, row);
#ifdef CONFIG_SP_ERASECNT_TABLE
        uint32_t count;
        struct nand_chip *nand = &info->nand;
        count = info->EraseCntTable[row >> (nand->phys_erase_shift - nand->page_shift)];
        count++;
        info->EraseCntTable[row >> (nand->phys_erase_shift - nand->page_shift)] = count;
#endif
		break;

	case NAND_CMD_ERASE2:	/* 0xD0 */
		sp_nand_desc_send(info, 1);
		break;

	case NAND_CMD_STATUS:	/* 0x70 */
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
		//while( readl(&regs->csr) & CSR_DESC_FETCH );
		info->buff_flag   = USE_PP_BUF_0;
		info->buff_page   = -1;
		info->buff_state  = PP_BUF_IDLE;
		info->buff1_page  = -1;
		info->buff1_state = PP_BUF_IDLE;
#endif
		sp_nand_desc_prep(info, cmd, -1, -1);
		sp_nand_desc_send(info, 1);
		break;

	case NAND_CMD_RESET:	/* 0xFF */
	case NAND_CMD_READID:	/* 0x90 */
	case NAND_CMD_PARAM:    /* 0xEC */
	case NAND_CMD_GET_FEATURES: /* 0xEE */
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
		//while( readl(&regs->csr) & CSR_DESC_FETCH );
		info->buff_flag   = USE_PP_BUF_0;
		info->buff_page   = -1;
		info->buff_state  = PP_BUF_IDLE;
		info->buff1_page  = -1;
		info->buff1_state = PP_BUF_IDLE;
#endif
		val = readl(&regs->actr0);
		writel(0xffffffff, &regs->actr0);
		sp_nand_desc_prep(info, cmd, col, -1);
		sp_nand_desc_send(info, 1);
		mdelay(5);
		writel(val, &regs->actr0);
		break;

	case NAND_CMD_SET_FEATURES: /* 0xEF */
		sp_nand_desc_prep(info, cmd, col, -1);
		break;

	default:
		printf("sp_nand: unknown command=0x%02x.\n", cmd);
		break;
	}
}

static int sp_nand_fixup(struct sp_nand_info *info)
{
	struct mtd_info *mtd = info->mtd;
	struct nand_chip *chip = mtd->priv;
	uint32_t nrpg;

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


void board_pnand_init(void)
{
	struct sp_nand_info *info = &our_nfc;
	struct nand_chip *nand = &info->nand;
	struct mtd_info *mtd = &nand_info[0];

	info->mtd = mtd;
	mtd->priv = nand;
	nand->IO_ADDR_R = nand->IO_ADDR_W = info->regs;

	debug("sp_nand: regs@0x%p\n", info->regs);

#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
	info->buff0.size = CFG_BUFF_MAX;
	info->buff0.phys = CONFIG_SP_NAND_SRAM_BASE;
	info->buff0.virt = (void *)info->buff0.phys;

	debug("sp_nand: buff0=0x%p@0x%08x size0=%u\n",
		info->buff0.virt, info->buff0.phys, info->buff0.size);

	info->buff1.size = CFG_BUFF_MAX;
	info->buff1.phys = CONFIG_SP_NAND_SRAM_BASE + CFG_BUFF_MAX;
	info->buff1.virt = (void *)info->buff1.phys;

	debug("sp_nand: buff1=0x%p@0x%08x size1=%u\n",
		info->buff1.virt, info->buff1.phys, info->buff1.size);

	info->desc.size = CFG_DESC_MAX * 32;
	info->desc.phys = CONFIG_SP_NAND_SRAM_BASE + CFG_BUFF_MAX + CFG_BUFF_MAX;
	info->desc.virt = (void *)info->desc.phys;
#else
	info->buff.size = CFG_BUFF_MAX;
	info->buff.phys = CONFIG_SP_NAND_SRAM_BASE;
	info->buff.virt = (void *)info->buff.phys;

	debug("sp_nand: buff=0x%p@0x%08x size=%u\n",
		info->buff.virt, info->buff.phys, info->buff.size);

#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
	info->buff1.size = CFG_BUFF_MAX;
	info->buff1.phys = CONFIG_SP_NAND_SRAM_BASE + CFG_BUFF_MAX;
	info->buff1.virt = (void *)info->buff1.phys;
	debug("sp_nand: buff1=0x%p@0x%08x size1=%u\n",
		info->buff1.virt, info->buff1.phys, info->buff1.size);
	info->buff_flag   = USE_PP_BUF_0;
	info->buff_page   = -1;
	info->buff_state  = PP_BUF_IDLE;
	info->buff1_page  = -1;
	info->buff1_state = PP_BUF_IDLE;
#endif
	info->desc.size = CFG_DESC_MAX * 32;
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
	info->desc.phys = CONFIG_SP_NAND_SRAM_BASE + CFG_BUFF_MAX + CFG_BUFF_MAX;
#else
	info->desc.phys = CONFIG_SP_NAND_SRAM_BASE + CFG_BUFF_MAX;
#endif
	info->desc.virt = (void *)info->desc.phys;
#endif

	debug("sp_nand: desc=0x%p@0x%08x size=%u\n",
		info->desc.virt, info->desc.phys, info->desc.size);

#ifdef CFG_BBT_USE_FLASH
	info->nand.bbt_options    = NAND_BBT_USE_FLASH;
#endif

	info->nand.select_chip    = sp_nand_select_chip;
	info->nand.cmd_ctrl       = sp_nand_cmd_ctrl;
	info->nand.cmdfunc        = sp_nand_cmdfunc;
	info->nand.dev_ready      = sp_nand_dev_ready;

	info->nand.read_byte      = sp_nand_read_byte;
	info->nand.read_buf       = sp_nand_read_buf;
	info->nand.write_buf      = sp_nand_write_buf;
	info->nand.verify_buf     = sp_nand_verify_buf;

	info->nand.ecc.read_page  = sp_nand_read_page;
	info->nand.ecc.write_page = sp_nand_write_page;
	info->nand.ecc.layout     = &info->ecc_layout;
	info->nand.ecc.mode       = NAND_ECC_HW;
	info->nand.options       |= NAND_NO_SUBPAGE_WRITE;

#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
	info->nand_working_buff_flag = 0;
	info->prefetch_row = -1;
#endif

#ifdef CONFIG_SP_NFTL
    sp_nand_ftl_info = (unsigned int *)info;
#endif

	if (sp_nand_reset(info) < 0)
		return;

	if (nand_scan_ident(mtd, CONFIG_SP_NAND_CSNR, sp_nand_ids))
		return;

	if (sp_nand_fixup(info) < 0)
		return;

	if (sp_bch_init(mtd))
		return;

	if (nand_scan_tail(mtd))
		return;

	nand_register(0);

#ifdef CONFIG_SP_ERASECNT_TABLE
    info->EraseCntTable = malloc(sizeof(uint32_t)*(mtd->size/mtd->erasesize));
    if(info){
        memset(info->EraseCntTable,0,sizeof(uint32_t)*(mtd->size/mtd->erasesize));
        printf("Allocated & clear %lld bytes in 0x%p\n",sizeof(uint32_t)*(mtd->size/mtd->erasesize),info->EraseCntTable);
    }else
        printf("Err to allocate buf for EraseCntTable\n");
#endif
}

#ifdef CONFIG_SP_ERASECNT_TABLE
// Prompt blk numer and its Erase count
void GetEraseCntTableInfo(void)
{
    struct sp_nand_info *info = &our_nfc;
    struct nand_chip *nand = &info->nand;
    int i,blkcnt;

    printf("\nBlk#[idx] = EraseCnt \n");
    printf("-----------------------\n");
    blkcnt = 1 << (nand->chip_shift - nand->phys_erase_shift);
    for(i=0;i<blkcnt;i++){
        if(info->EraseCntTable[i])printf("Blk#[%04d] = %04d\n",i,info->EraseCntTable[i]);
    }
}

void DumpEraseCntTable(void)
{
    struct sp_nand_info *info = &our_nfc;
    struct nand_chip *nand = &info->nand;
    int i,blkcnt;

    printf("\nBlk#: \n");
    printf("--------------------------------------------------------------------------------------");
    blkcnt = 1 << (nand->chip_shift - nand->phys_erase_shift);
    for(i=0;i<blkcnt;i++){
        if(i%16 == 0)printf("\n%04d:",i);
        printf("%04d ",info->EraseCntTable[i]);
    }
    printf("\n");
}
#endif
#ifdef CONFIG_SP_BCH_REPORT
extern unsigned int* Get_SP_BCH_Info(void);
u32 bch_status_for_kvdb;

static int sp_bch_sts(cmd_tbl_t *cmdtp, int flag, int argc,
			  char * const argv[])
{
	struct sp_nand_info *info = &our_nfc;
	struct sp_bch_info *bch_info = (struct sp_bch_info *)Get_SP_BCH_Info();
	struct sp_bch_regs *regs = bch_info->regs;
	struct mtd_info *mtd = info->mtd;
	ulong pagenumber; // Nand Page Number
	// uint32_t bch_tmp;
	int ret;

	if (argc < 2) {
		printf("usage: sp_bch_sts PageNumber\n");
		printf("i.e. sp_bch_sts 6\n");
		printf(" All number is in Hex\n");
		return 0;
	}

	pagenumber = (ulong)simple_strtoul(argv[1], NULL, 16);

	info->cmd = NAND_CMD_READ0;
	sp_nand_desc_prep(info, NAND_CMD_READ0, 0, (int)pagenumber);
	sp_nand_desc_send(info, 1);

	ret = sp_bch_decode(mtd, info->buff.virt,info->buff.virt + mtd->writesize);
	bch_status_for_kvdb = bch_info->ecc_sts;
	// bch_tmp = bch_info->ecc_sts;
	// printf("Page#:0x%x,ecc_sts:0x%x(0x%x)\n",pagenumber,bch_tmp,bch_info->ecc_sts);

	return 0;
}

U_BOOT_CMD(
	bch_sts, 5, 1, sp_bch_sts,
	"Sunplus BCH ECC Utility",
	"Report BCH ECC status\n"
);
#endif

//#define NAND_OFFREAD_DBG
//#define NAND_OFFREAD_DBG_DUMP
static int do_nand_offread(cmd_tbl_t *cmdtp, int flag, int argc,
			  char * const argv[])
{
#ifdef NAND_OFFREAD_DBG
	char cmd[256];
	int md_cnt;
#endif
	ulong off,size,ram_addr,nand_addr,blk_addr;
	int ret;
	int row,row_cnt,bbt_row;
	unsigned int fb_addr, cbdmalen;

	struct sp_nand_info *info = &our_nfc;
	struct mtd_info *mtd = info->mtd;
	struct sp_nand_regs *regs = info->regs;

	unsigned long start, stop;

	if (argc < 5)
		return CMD_RET_USAGE;

#ifdef NAND_OFFREAD_DBG
	snprintf(cmd, sizeof(cmd), "nand_offread 0x%s 0x%s 0x%s 0x%s", argv[1], argv[2], argv[3], argv[4]);
	printf("\n%s\n\n", cmd);
#endif
	ram_addr = (ulong)simple_strtoul(argv[1], NULL, 16);
	nand_addr = (ulong)simple_strtoul(argv[2], NULL, 16);
	size = (ulong)simple_strtoul(argv[3], NULL, 16);
	off = (ulong)simple_strtoul(argv[4], NULL, 16);

	if (size < info->mtd->writesize)
		size = info->mtd->writesize;

	if (off >= size)
		return CMD_RET_USAGE;

	row = nand_addr/info->mtd->writesize;
	row_cnt = size /info->mtd->writesize;
	if (size % info->mtd->writesize)
		row_cnt++;
	if (off >= info->mtd->writesize) {
		row += (off/info->mtd->writesize);
		row_cnt -= (off/info->mtd->writesize);
		off %= info->mtd->writesize;
	}

#ifdef NAND_OFFREAD_DBG
	md_cnt = row_cnt;
	printf("row:%d, row_cnt:%d, off=%d, erasesize:0x%x\n", row, row_cnt, off, info->mtd->erasesize);
#endif

	bbt_row = row;
rechk_bbt:
	blk_addr = bbt_row*info->mtd->writesize;
	if ((blk_addr % info->mtd->erasesize) == 0) {
		ret = mtd_block_isbad(info->mtd, blk_addr);
		if (ret) {
			printf("Skip Bad block:0x%x\n", blk_addr);
			blk_addr += info->mtd->erasesize;
			bbt_row = blk_addr/info->mtd->writesize;
			goto rechk_bbt;
		}
		if (bbt_row > row)
			row = bbt_row;
	} else {
		blk_addr -= blk_addr%info->mtd->erasesize;
		bbt_row = blk_addr/info->mtd->writesize;
		goto rechk_bbt;
	}

#ifdef NAND_OFFREAD_DBG
  printf("row:%d, row_cnt:%d, off=%d, erasesize:0x%x\n", row, row_cnt, off, info->mtd->erasesize);
#endif
	sp_nand_desc_prep(info, NAND_CMD_READ0, 0, row);
	sp_nand_desc_send(info, 1);

	ret = sp_bch_decode(info->mtd, info->buff.virt,
			info->buff.virt + info->mtd->writesize);

	start = (unsigned long)ram_addr;
	stop = start + info->mtd->writesize - off;
#ifdef CONFIG_SP_NAND_READ_HELP_FLUSH_BUF
	flush_dcache_range(start, stop);
#else
	invalidate_dcache_range(start, stop);
#endif
	sp_cbdma_write(info->buff.virt + off, ram_addr, info->mtd->writesize - off);

	fb_addr = ram_addr + info->mtd->writesize - off;
	cbdmalen = size - info->mtd->writesize + off;

#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
	if (--row_cnt) {
		row++;
		while (row_cnt) {

			blk_addr = row*info->mtd->writesize;
			if ((blk_addr % info->mtd->erasesize) == 0) {
				ret = mtd_block_isbad(info->mtd, blk_addr);
				if (ret) {
					printf("PP Skip Bad block:0x%x\n", blk_addr);
					blk_addr += info->mtd->erasesize;
					row = blk_addr/info->mtd->writesize;
					continue;
				}
			}

			while (readl(&regs->csr) & CSR_DESC_FETCH);
			if (row % 2) {
				// DBG printf("row=%d ,BUF-1,",row);
				info->buff_flag = USE_PP_BUF_1;
				PingPongBuf_1_PageReadCmdSend(info, NAND_CMD_READ0, 0, row);
			} else {
				// DBG printf("row=%d ,BUF-0,",row);
				info->buff_flag = USE_PP_BUF_0;
				PingPongBuf_0_PageReadCmdSend(info, NAND_CMD_READ0, 0, row);
			}
			//printf("read-page:0x%x\n",page); // DBG
			ret = sp_nand_wait(info);
			if (ret) {
				printf("sp_nand: descriptor timeout, cmd=0x%x\n", info->cmd);
				sp_nand_reset(info);
			}

			start = (unsigned long)fb_addr;
			stop = start + info->mtd->writesize;
#ifdef CONFIG_SP_NAND_READ_HELP_FLUSH_BUF
			flush_dcache_range(start, stop);
#else
			invalidate_dcache_range(start, stop);
#endif

			if (info->buff_flag == USE_PP_BUF_0) {
				info->buff_state = PP_BUF_DATA_RDY; // buff-0 data rdy
				PingPongBuf_1_PageReadCmdSend(info, NAND_CMD_READ0, 0, ++row);
				ret = sp_bch_decode(info->mtd, info->buff.virt,
								info->buff.virt + info->mtd->writesize);
				if (ret)
					printf("PP-Buf0 sp_nand: bch decode failed at page=%d\n", row - 1);

				info->buff_state = PP_BUF_DECODE_RDY; // buff-0 decode rdy,
				sp_cbdma_write(info->buff.virt, fb_addr,
					(cbdmalen/info->mtd->writesize) ?
					(info->mtd->writesize) : (cbdmalen % info->mtd->writesize));

				info->buff_state = PP_BUF_IDLE; // buf-0 back to idle
			} else {
				info->buff1_state = PP_BUF_DATA_RDY; // buff-1 data rdy
				PingPongBuf_0_PageReadCmdSend(info, NAND_CMD_READ0, 0, ++row);
				ret = sp_bch_decode(info->mtd, info->buff1.virt,
								info->buff1.virt + info->mtd->writesize);
				if (ret)
					printf("PP-Buf-1 sp_nand: bch decode failed at page=%d\n", row - 1);

				info->buff1_state = PP_BUF_DECODE_RDY; // buff-1 decode rdy,
				sp_cbdma_write(info->buff1.virt, fb_addr,
					(cbdmalen/info->mtd->writesize) ?
					(info->mtd->writesize) : (cbdmalen % info->mtd->writesize));

				info->buff1_state = PP_BUF_IDLE; // buf-1 back to idle
			}

			if (cbdmalen / info->mtd->writesize)
				cbdmalen -= info->mtd->writesize;
			else
				cbdmalen -= (cbdmalen % info->mtd->writesize);
#ifdef NAND_OFFREAD_DBG_DUMP
			printf("PP in loop -> row:%d, row_cnt:%d, size:%d\n", row, row_cnt, cbdmalen);
#endif
			fb_addr += info->mtd->writesize;
			row_cnt--;
		}
		info->buff_state = PP_BUF_IDLE;  // buf-0 back to idle
		info->buff1_state = PP_BUF_IDLE; // buf-1 back to idle
	}

#else

	if (--row_cnt) {
		row++;
		while (row_cnt) {

			blk_addr = row*info->mtd->writesize;
			if ((blk_addr % info->mtd->erasesize) == 0) {
				ret = mtd_block_isbad(info->mtd, blk_addr);
				if (ret) {
					printf("NoPP Skip Bad block:0x%x\n", blk_addr);
					blk_addr += info->mtd->erasesize;
					row = blk_addr/info->mtd->writesize;
					continue;
				}
			}

			sp_nand_desc_prep(info, NAND_CMD_READ0, 0, row++);
			sp_nand_desc_send(info, 1);

			ret = sp_bch_decode(info->mtd, info->buff.virt,
						info->buff.virt + info->mtd->writesize);
			if (ret)
				printf("nand_offread: bch decode failed at page=%d\n", row -1 );

			start = (unsigned long)fb_addr;
			stop = start + info->mtd->writesize;
#ifdef CONFIG_SP_NAND_READ_HELP_FLUSH_BUF
			flush_dcache_range(start, stop);
#else
			invalidate_dcache_range(start, stop);
#endif
			sp_cbdma_write(info->buff.virt, fb_addr,
				(cbdmalen/info->mtd->writesize) ?
				(info->mtd->writesize) : (cbdmalen % info->mtd->writesize));

			if (cbdmalen / info->mtd->writesize)
				cbdmalen -= info->mtd->writesize;
			else
				cbdmalen -= (cbdmalen % info->mtd->writesize);
#ifdef NAND_OFFREAD_DBG_DUMP
			printf("in loop -> row:%d, row_cnt:%d, size:%d\n", row, row_cnt, cbdmalen);
#endif
			fb_addr += info->mtd->writesize;
			row_cnt--;
		}
	}
#endif

#ifdef NAND_OFFREAD_DBG_DUMP
	snprintf(cmd, sizeof(cmd), "md.b %s 0x%x", argv[1], size);
	printf("%s\n", cmd);  run_command(cmd, 0);
#endif
	return 0;
}

static char nand_offread_help_text[] =
    " S+ Nand utility\n"
    "nand_offread D_addr F_addr size offset_addr\n";

U_BOOT_CMD(
    nand_offread, 5, 0, do_nand_offread,
    "Sunplus Nand utility", nand_offread_help_text
);
// #define SNAND_MARKBAD_DBG
static int do_snand_markbad(cmd_tbl_t *cmdtp, int flag, int argc,
					char *const argv[])
{
	char cmd[256];
	ulong nand_addr, blk_addr;
	int ret;
	int row;
	unsigned int blk_num;

	struct sp_nand_info *info = &our_nfc;
	struct nand_chip *nand = &info->nand;

	if (argc < 2)
		return CMD_RET_USAGE;

#ifdef SNAND_MARKBAD_DBG
	snprintf(cmd, sizeof(cmd), "snand_markbad 0x%s", argv[1]);
	printf("\n%s\n\n", cmd);
#endif
	nand_addr = (ulong)simple_strtoul(argv[1], NULL, 16);
	blk_addr = (nand_addr / info->mtd->erasesize) * info->mtd->erasesize;
#ifdef SNAND_MARKBAD_DBG
	printf("blk_addr:0x%lx, erasesize:0x%x\n", blk_addr, info->mtd->erasesize);
#endif

	row = blk_addr / info->mtd->writesize;
	ret = mtd_block_isbad(info->mtd, blk_addr);
	if (ret) {
		printf("Bad block:0x%lx\n", blk_addr);
		goto exit_snand_markbad;
	}

#ifdef SNAND_MARKBAD_DBG
	printf("new row:%d, erasesize:0x%x\n", row, info->mtd->erasesize);
#endif

	snprintf(cmd, sizeof(cmd), "nand erase 0x%x 0x800", row * info->mtd->writesize);
	// printf("%s\n", cmd);
	ret = run_command(cmd, 0);
	if (ret != 0) {
		printf("Error in snand_markbad:0x%x\n", row * info->mtd->writesize);
		goto exit_snand_markbad;
	}

	memset(info->buff.virt, 0xff, info->mtd->writesize + info->mtd->oobsize);
	memset(info->buff.virt + info->mtd->writesize, 0x00, 1);
	memset(info->buff.virt + info->mtd->writesize + 1, 0x42, 1);
	memset(info->buff.virt + info->mtd->writesize + 2, 0x41, 1);
	memset(info->buff.virt + info->mtd->writesize + 3, 0x44, 1);
	info->cmd = NAND_CMD_SEQIN;
	sp_nand_desc_prep(info, NAND_CMD_SEQIN, 0, row);
	sp_nand_desc_send(info, 1);

	/* Mark block bad in memory-based BBT */
	blk_num = blk_addr / info->mtd->erasesize;
	if (nand->bbt)
		nand->bbt[blk_num >> 2] |= 0x01 << ((blk_num & 0x03) << 1);
	printf("Block 0x%08x successfully marked as S+ bad\n", row * info->mtd->writesize);

exit_snand_markbad:
#if 0 //def SNAND_MARKBAD_DBG
	snprintf(cmd, sizeof(cmd), "nand dump.oob 0x%x", row * info->mtd->writesize);
	printf("%s\n", cmd);  run_command(cmd, 0);
#endif
	return 0;
}

static char snand_markbad_help_text[] =
    "S+ Nand utility\n"
    "snand_markbad Nand_addr\n";

U_BOOT_CMD(
    snand_markbad, 2, 0, do_snand_markbad,
    "Sunplus Nand utility", snand_markbad_help_text
);

int Chk_SNAND_Marker(struct sp_nand_info *info)
{
	if ((readb(info->buff.virt + info->mtd->writesize) == 0x00) &&
			(readb(info->buff.virt + info->mtd->writesize + 1) == 0x42) &&
			(readb(info->buff.virt + info->mtd->writesize + 2) == 0x41) &&
			(readb(info->buff.virt + info->mtd->writesize + 3) == 0x44))
		return 1;
	return 0;
}

//#define SNAND_SCANBAD_DBG
static int do_snand_scanbad(cmd_tbl_t *cmdtp, int flag, int argc,
					char *const argv[])
{
#ifdef SNAND_SCANBAD_DBG
	char cmd[256];
#endif
	int i, row, blk_cnt;
	struct sp_nand_info *info = &our_nfc;
	struct nand_chip *nand = &info->nand;

	if (argc < 1)
		return CMD_RET_USAGE;

#ifdef SNAND_SCANBAD_DBG
	snprintf(cmd, sizeof(cmd), "snand_scanbad");
	printf("\n%s\n\n", cmd);
#endif

	blk_cnt = nand->chipsize / info->mtd->erasesize;
	snand_bad_block_cnt = 0;

#ifdef SNAND_SCANBAD_DBG
	printf("blk_cnt:%d, erasesize:0x%x\n", blk_cnt, info->mtd->erasesize);
#endif

	for (i = 0; i < blk_cnt; i++) {
		memset(info->buff.virt, 0x0, 4);
		row = (info->mtd->erasesize * i) / info->mtd->writesize;
		info->cmd = NAND_CMD_READOOB;
		sp_nand_desc_prep(info, NAND_CMD_READOOB, 0, row);
		sp_nand_desc_send(info, 1);
		if (readb(info->buff.virt + info->mtd->writesize ) != 0xff) {
			if (Chk_SNAND_Marker(info)) {
				snand_bad_block_cnt++;
				printf("  %08x\n", row * info->mtd->writesize);
			}
		}
	}

#if 1 //def SNAND_SCANBAD_DBG
	printf("Total S+ bad block:%d\n", snand_bad_block_cnt);
#endif
	return 0;
}

static char snand_scanbad_help_text[] =
    "S+ Nand utility\n"
    "snand_scanbad\n";

U_BOOT_CMD(
    snand_scanbad, 1, 0, do_snand_scanbad,
    "Scan S+ Bad Block", snand_scanbad_help_text
);


// #define SNAND_SCRUB_DBG
static int do_snand_scrub(cmd_tbl_t *cmdtp, int flag, int argc,
					char *const argv[])
{
#ifdef SNAND_SCRUB_DBG
	char cmd[256];
#endif
	uint8_t ret;
	int i, row, blk_cnt, blk_num, scrub_cnt;
	struct sp_nand_info *info = &our_nfc;
	struct nand_chip *nand = &info->nand;

	if (argc < 1)
		return CMD_RET_USAGE;

#ifdef SNAND_SCRUB_DBG
	snprintf(cmd, sizeof(cmd), "snand_scrub");
	printf("\n%s\n\n", cmd);
#endif

	blk_cnt = nand->chipsize / info->mtd->erasesize;
	scrub_cnt = 0;

#ifdef SNAND_SCRUB_DBG
	printf("blk_cnt:%d, erasesize:0x%x\n", blk_cnt, info->mtd->erasesize);
#endif

	for (i = 0; i < blk_cnt; i++) {
		memset(info->buff.virt, 0x0, 4);
		row = (info->mtd->erasesize * i) / info->mtd->writesize;
		info->cmd = NAND_CMD_READOOB;
		sp_nand_desc_prep(info, NAND_CMD_READOOB, 0, row);
		sp_nand_desc_send(info, 1);
		if (readb(info->buff.virt + info->mtd->writesize ) != 0xff) {
			if (Chk_SNAND_Marker(info)) {

				printf("  %08x\n", row * info->mtd->writesize);
				sp_nand_desc_prep(info, NAND_CMD_ERASE1, 0, row);
				sp_nand_desc_send(info, 1);

				// check if Erase device is OK or not !
				info->cmd = NAND_CMD_STATUS;
				sp_nand_desc_prep(info, NAND_CMD_STATUS, -1, -1);
				sp_nand_desc_send(info, 1);
				ret = sp_nand_read_byte(info->mtd);
				if (ret & 0x01) {
					printf("Erase 0x%08x error\n", row * info->mtd->writesize);
					continue;
				}
				scrub_cnt++;
				/* Removed S+ bad block in memory-based BBT */
				blk_num = (row * info->mtd->writesize) / info->mtd->erasesize;
				if (nand->bbt)
					nand->bbt[blk_num >> 2] &= ~0x03 << ((blk_num & 0x03) << 1);
			}
		}
	}

#if 1 //def SNAND_SCRUB_DBG
	printf("Scrub S+ %d bad block\n", scrub_cnt);
#endif
	return 0;
}

static char snand_scrub_help_text[] =
    "S+ Nand utility\n"
    "snand_scrub Nand device\n";

U_BOOT_CMD(
    snand_scrub, 2, 0, do_snand_scrub,
    "Scrub S+ Bad Block marker", snand_scrub_help_text
);
