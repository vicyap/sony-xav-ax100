#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <nand.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
#include <asm/arch/regmap.h>

#include "sp_bch.h"
#include "sp_spinand.h"
#include "sp_cbdma.h"

#ifndef CONFIG_SP_SPINAND_CSNR
#define CONFIG_SP_SPINAND_CSNR	1
#endif

/* #define CFG_BBT_USE_FLASH */
#define CFG_NAND_HZ			304000000 /* 304 MHz */
#define CFG_BUFF_MAX		(18 << 10)
#define CFG_DESC_MAX		4
#define CFG_CMD_TIMEOUT_MS	10

#define CFG45_DefaultValue	0x15
#define CFG67_DefaultValue	0x95

#define USE_DESCRIPTOR_MODE	0
#define DEVICE_STS_AUTO_CHK	1
#define USE_SP_BCH		1   /* 1:Using S+ BCH, 0:using Device internal ECC */

DECLARE_GLOBAL_DATA_PTR;

#ifndef MAX
#define MAX(a, b)		((a) > (b) ? (a) : (b))
#endif

#define BIT(n)			(1 << (n))

static struct sp_spinand_info our_spinfc = {
	.regs = (void __iomem *)CONFIG_SP_SPINAND_BASE,
};

static int sp_spinand_reset(struct sp_spinand_info *info);

/* check SPI NAND Device status by ctrl_status_registers */
void wait_spi_idle(struct sp_spinand_info *info)
{
	struct sp_spinand_regs *regs = info->regs;

	while (readl(&regs->ctrl) & SPI_DEVICE_IDLE);	/* --> ctrl bit-15, wait spi_ctrl idle	 */
}

void spi_nand_readid(struct sp_spinand_info *info, uint32_t addr, uint32_t *data)
{
	struct sp_spinand_regs *regs = info->regs;

	printf("\n%s : %d\t<%s>      \n", __FILE__, __LINE__, __FUNCTION__);
	writel(addr	 , &regs->addr_low);
	writel(addr >> 16, &regs->addr_high);

	printf("\nregs->addr_low:%lx" , readl(&regs->addr_low));
	printf("\nregs->addr_high:%lx", readl(&regs->addr_high));

	writel(0x9F8D, &regs->cust_cmd);
	wait_spi_idle(info);

	printf("\nregs->data_low:%lx\tregs->ctrl:%lx" , readl(&regs->data_low),readl(&regs->ctrl));
	printf("\nregs->data_high:%lx", readl(&regs->data_high));

	printf("\nReadID:0x%02x,0x%02x,0x%02x,0x%02x\n", (readl(&regs->data_low) & 0xFF), ((readl(&regs->data_low) >> 8) & 0xFF), (readl(&regs->data_high) & 0xFF), ((readl(&regs->data_high) >> 16) & 0xFF));

	*data = (readl(&regs->data_low) | (readl(&regs->data_high) << 16));
}

int spi_nand_getfeatures(struct sp_spinand_info *info, uint32_t addr)
{
	struct sp_spinand_regs *regs = info->regs;

	/* cfg-4 & cfg-6 RGST bus( for Data_64 port usage ) */
	writel(CFG45_DefaultValue, &regs->cfg4);
	writel(CFG67_DefaultValue, &regs->cfg6);

	writel(addr      , &regs->addr_low);
	writel(addr >> 16, &regs->addr_high);

	writel(0x0F85, &regs->cust_cmd);
	wait_spi_idle(info);

	return (readl(&regs->data_low) & 0xFF);
}

void spi_nand_setfeatures(struct sp_spinand_info *info, uint32_t addr, uint32_t data)
{
	struct sp_spinand_regs *regs = info->regs;

	writel(CFG45_DefaultValue, &regs->cfg4);
	writel(CFG67_DefaultValue, &regs->cfg6);

	writel(addr, &regs->addr_low);
	writel(addr >> 16, &regs->addr_high);

	writel(data, &regs->data_low);
	writel(data >> 16, &regs->data_high);

	writel(0x1FA5, &regs->cust_cmd);

	wait_spi_idle(info);
	//printf("set features done\n");
}

static int sp_spinand_reset(struct sp_spinand_info *info)
{
	struct sp_spinand_regs *regs = info->regs;
	int ret = -1;

	while (readl(&regs->ctrl)& SPI_DEVICE_IDLE);
	printf("sp_spinand_reset\t");

	writel(0x211c, &regs->ctrl); /* clk/8 */

	writel(readl(&regs->wait) | SPI_NAND_ENABLE, &regs->wait);

	writel(1, &regs->cfg3);
	writel(CFG45_DefaultValue, &regs->cfg4);
	writel(CFG67_DefaultValue, &regs->cfg6);

	/* ==== Flash reset ==== */
	writel(0xFF80, &regs->cust_cmd);

#if DEVICE_STS_AUTO_CHK
	writel(0x0583, &regs->cust_cmd);
	wait_spi_idle(info);
	ret = spi_nand_getfeatures(info, 0xC0);
#else
	do {
		ret = spi_nand_getfeatures(info, 0xC0);
		printf("%s Status:%x\n",__FUNCTION__,ret);
	} while (ret & 0x01);

#endif

	return ret;
}

void spi_nand_pageread2cache(struct sp_spinand_info *info, uint32_t addr)
{
	struct sp_spinand_regs *regs = info->regs;

	// RGST bus
	writel(0x05, &regs->cfg4);
	writel(0x05, &regs->cfg6);

	addr &= 0xFFFFFF;	/* 3 byte addr cnt */

	writel(addr      ,&regs->addr_low);
	writel(addr >> 16,&regs->addr_high);

	writel(0x1383, &regs->cust_cmd);
	wait_spi_idle(info);

	//printf("will do Page to Cache\n");
#if DEVICE_STS_AUTO_CHK
	writel(0x0583, &regs->cust_cmd);
	wait_spi_idle(info);
#else
	int ret;
	do {
		ret = spi_nand_getfeatures(info, 0xC0);
	} while (ret & 0x1);
#endif

}

void spi_nand_readcacheQuadIO_byMapping(struct  sp_spinand_info *info,
							uint32_t addr,
						    unsigned int size,
							uint32_t *pbuf)
{
	struct sp_spinand_regs *regs = info->regs;
	int i;
	unsigned int cfg1,cfg5,cfg7;

	if (info->id == 0x21C8) {  /* ESMT */
#if USE_SP_BCH
		spi_nand_setfeatures(info, 0xB0, 0x01);	/* en-able QuadIO,ECC-off */
#else
		spi_nand_setfeatures(info, 0xB0, 0x11);	/* en-able QuadIO,ECC-on */
#endif
		cfg5 = 0x0435;
		cfg7 = 0x05;
		cfg1 = 0x6B10;
	} else if (info->id == 0xC8) { /* GD */
#if USE_SP_BCH
		spi_nand_setfeatures(info, 0xB0, 0x01);	/* en-able QuadIO,ECC-off */
#else
		spi_nand_setfeatures(info, 0xB0, 0x11);	/* en-able QuadIO,ECC-on */
#endif
		/* GD SPI QuadIO: 2 dummy address cycle, 4 bit data, 4 bit address, 1 bit cmd */
		cfg5 = 0x013d;
		cfg7 = 0x01;
		cfg1 = 0xEB10;
	} else if (info->id == 0xEF) { /* WB */
#if USE_SP_BCH
		spi_nand_setfeatures(info, 0xB0, 0x08);	/* ECC-off ,BUF-1 */
#else
		spi_nand_setfeatures(info, 0xB0, 0x18);	/* ECC on & BUF-1 mode on */
#endif
		/* Winbond SPI QuadIO: 4 dummy address cycle, 4 bit data, 4 bit address, 1 bit cmd */
		cfg5 = 0x023D;
		cfg7 = 0x01;
		cfg1 = 0xEB10;
	} else if (info->id == 0xC2) {	/* MXIC */
#if USE_SP_BCH
		spi_nand_setfeatures(info, 0xB0, 0x01);	/* en-able QuadIO,ECC-off */
#else
		spi_nand_setfeatures(info, 0xB0, 0x11);	/* en-able QuadIO,ECC-on */
#endif
		/* MXIC SPI Cachex4: 8 dummy address cycle, 4 bit data, 1 bit address, 1 bit cmd */
		cfg5 = 0x0435;
		cfg7 = 0x05;
		cfg1 = 0x6B10;
	}

	writel(cfg5,&regs->cfg5);
	writel(cfg7,&regs->cfg7);
	writel(cfg1,&regs->cfg1);

	//for(i=addr;i<size;i+=4) *(unsigned int*)pbuf++=*(unsigned int*)(0x94000000+i); // address is included in i
	if((info->row & (0x40)) &&  ((info->id & 0xFF) == 0xC2))
		memcpy(pbuf, (0x94001000), size);
	else
		memcpy(pbuf, (0x94000000), size);

	wait_spi_idle(info);
}

void spi_nand_wren(struct sp_spinand_info *info)
{
	struct sp_spinand_regs *regs = info->regs;

	writel(CFG45_DefaultValue, &regs->cfg4);
	writel(CFG67_DefaultValue, &regs->cfg6);
	writel(0x0680, &regs->cust_cmd);
	//wait_spi_idle(info);
}

void spi_nand_wrdi(struct sp_spinand_info *info)
{
	struct sp_spinand_regs *regs = info->regs;

	writel(CFG45_DefaultValue, &regs->cfg4);
	writel(CFG67_DefaultValue, &regs->cfg6);
	writel(0x0480, &regs->cust_cmd);
	//wait_spi_idle(info);
}

int spi_nand_blkerase(struct sp_spinand_info *info, uint32_t addr)
{
	struct sp_spinand_regs *regs = info->regs;
	unsigned short cmd_data = 0;
	int ret;

	writel(0x05, &regs->cfg4);
	writel(0x05, &regs->cfg6);

	cmd_data = (SPINAND_CMD_BLKERASE << SPI_CUSTCMD_SHIFT) | SPI_CUSTCMD_ENABLE | SPI_CUSTCMD_WRITE | SPINAND_CUSTCMD_3_ADDR;

	addr &= 0xFFFFFF;	/* for 1Gbit:1 dummy byte address + 2 byte(page/block) address */

	writel(addr    	 , &regs->addr_low);
	writel(addr >> 16, &regs->addr_high);

	writel(cmd_data, &regs->cust_cmd);	/* 0xd8a3 */
	writel(0x0, &regs->cust_cmd_2);

#if DEVICE_STS_AUTO_CHK
	wait_spi_idle(info);
	do {
		ret = spi_nand_getfeatures(info, 0xC0);
	} while (ret & 0x01);
#else
	do {
		ret=spi_nand_getfeatures(info, 0xC0);
		printf("%s Status:%x\n", __FUNCTION__, ret);
	} while (ret & 0x01);
#endif

	if (ret & 0x04)
		printf("\nErase Fail!\n");

	return (ret & 0x04);

}

void spi_nand_proloadx4(struct sp_spinand_info *info, uint32_t addr, uint8_t *pbuf, unsigned int size)
{
	struct sp_spinand_regs *regs = info->regs;
	uint16_t *pdata = (uint16_t *)pbuf;
	unsigned int len = 0;
	int i,ret;

	writel(0x211C, &regs->ctrl); /* SPI_CLK/8 */

	/* RGST bus cfg */
	writel(0x35, &regs->cfg4);	/* 0x35 -> 4-bit data, 1-bit cmd & addr */
	writel(0x05, &regs->cfg6);

	if(((info->row) & (0x40)) && ((info->id & 0xFF) == 0xC2)) { /* MXIC-2G */
		addr = (addr | BIT(12));
	}

	writel(addr & 0xFFFF, &regs->addr_low);
	writel(addr >> 16   , &regs->addr_high);

	len = (size & 0x01FF) | (((size & 0x0E00) << 2)) | (1 << 10);	/* bit-10 using Data_64 port */
	writel(len, &regs->cust_cmd_2);

	writel(0x32A2, &regs->cust_cmd);	/*  --> 0x32 is program loadx4,0xa-> bit7:cmd enable,bit5:to Flash, */

	for (i=0; i < size / 2; i++)
		writel(pdata[i], &regs->data_64);

	wait_spi_idle(info);
	writel(0, &regs->cust_cmd_2);	/* SW have to clear cust_cmd_2 bit-10 function */

#if DEVICE_STS_AUTO_CHK
	ret = 0;
	do {
		ret=spi_nand_getfeatures(info, 0xC0);
	} while (ret&0x01);
	if (ret & 0x08)
		printf("but fail\n");
#else
	do {
		ret = spi_nand_getfeatures(info, 0xC0);
	} while (ret&0x01);
#endif

}

int spi_nand_proexecute(struct sp_spinand_info *info,uint32_t addr)
{
	struct sp_spinand_regs *regs = info->regs;
	int ret;

	/* RGST bus */
	writel(0x05, &regs->cfg4);
	writel(0x05, &regs->cfg6);

	addr &= 0xFFFFFF;	/* 1Gbit:1 dummy byte address + 2 byte(page/block) address,using 3 byte address */

	writel(addr, &regs->addr_low);
	writel(addr >> 16,&regs->addr_high);

	writel(0x10A3, &regs->cust_cmd);
	wait_spi_idle(info);

#if DEVICE_STS_AUTO_CHK
	ret = 0;
	do {
		ret = spi_nand_getfeatures(info, 0xC0);
	} while (ret & 0x01);
	if (ret & 0x08)
		printf("but fail\n");
#else
	do {
		ret = spi_nand_getfeatures(info, 0xC0);

	} while (ret & 0x01);
#endif

#if !USE_SP_BCH
	printf("Pro Execute is done ...");
	if (ret & 0x08)
		printf("but fail\n");
	else
		printf("ok\n");
#endif
	return ret;
}

static int sp_spinand_wait(struct sp_spinand_info *info)
{
	struct sp_spinand_regs *regs = info->regs;
	unsigned long now = get_timer(0);
	int ret = -1;

	while (get_timer(now) < CFG_CMD_TIMEOUT_MS) {
		if (!(readl(&regs->ctrl) & SPI_DEVICE_IDLE) ) {
			ret = 0;
			break;
		}
	}

	return ret;
}

static int sp_spinand_desc_prep(struct sp_spinand_info *info,
						 uint8_t cmd, int col, int row)
{
	int temp1,temp2;

	switch (cmd) {
	case NAND_CMD_READOOB:		/* 0x50 */
		col += info->mtd->writesize;
		/* fall through */
	case NAND_CMD_READ0:		/* 0x00 */
		info->cmd = SPINAND_CMD_PAGE2CACHE;
		info->buff.idx = col;
		info->row = row;
		spi_nand_pageread2cache(info, row & 0x01FFFF);
		spi_nand_readcacheQuadIO_byMapping(info, 0, info->mtd->writesize + info->mtd->oobsize, (unsigned int*)info->buff.virt);
		break;
	case NAND_CMD_SEQIN:		/* 0x80 */
		info->cmd = SPINAND_CMD_PROLOADx4;
		info->buff.idx = col;
		//spi_nand_wren(info);
		spi_nand_setfeatures(info, 0xA0, 0x0);
		spi_nand_wren(info);
		info->row = (row & 0x01FFFF);
		break;
	case NAND_CMD_ERASE1:		/* 0x60 */
		info->cmd = SPINAND_CMD_BLKERASE;
		spi_nand_wren(info);	/* for MXIC,ESMT */
		spi_nand_setfeatures(info, 0xA0, 0x0);
		//spi_nand_wren(info);
		spi_nand_blkerase(info, (row & (0x01FFC0)));	/* bit-5~0 is page address */
		break;
	case NAND_CMD_STATUS:		/* 0x70 */
		info->cmd = SPINAND_CMD_GETFEATURES;
		info->buff.idx = 0;
		if (spi_nand_getfeatures(info, 0xA0) & 0x38) {
			*(unsigned int*)info->buff.virt = 0x0;	/* protected */
		} else {
			*(unsigned int*)info->buff.virt = 0x80;	/* not protectd. comply w/ raw NAND */
		}
		break;
	case NAND_CMD_READID:		/* 0x90 */
	case NAND_CMD_PARAM:		/* 0xEC */
	case NAND_CMD_GET_FEATURES: /* 0xEE */
		break;
	case NAND_CMD_SET_FEATURES: /* 0xEF */
		break;
	case NAND_CMD_RESET:		/* 0xFF */
		break;
	default:
		break;
	}

	return 0;
}

static int sp_spinand_desc_send(struct sp_spinand_info *info, int need_wait)
{
	int ret;

	if (info->cmd == NAND_CMD_PAGEPROG ) {
		info->cmd = SPINAND_CMD_PROEXECUTE;
#if USE_SP_BCH
		spi_nand_proloadx4(info, 0, (unsigned char *)info->buff.phys, info->mtd->writesize + info->mtd->oobsize);
#else
		spi_nand_proloadx4(info, 0, (unsigned char *)info->buff.phys, info->mtd->writesize);
#endif

		spi_nand_proexecute(info, info->row);
	}

	ret = sp_spinand_wait(info);
	if (ret) {
		pr_info("sp_spinand: timeout, cmd=0x%x\n", info->cmd);
		sp_spinand_reset(info);
	}

	return ret;
}

static int sp_spinand_read_page(struct mtd_info *mtd,
			      struct nand_chip *chip, uint8_t *buf, int oob_required, int page)
{
#if USE_SP_BCH
	struct sp_spinand_info *info = &our_spinfc;
	int ret;

	ret = sp_bch_decode(mtd, info->buff.virt, info->buff.virt + mtd->writesize);
	if (ret)
		printf("sp_spinand: bch decode failed at page=%d\n", page);
#endif

	chip->read_buf(mtd, buf, mtd->writesize);

	if (!oob_required || page < 0)
		return 0;

	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);

	return 0;
}

static int sp_spinand_write_page(struct mtd_info *mtd, struct nand_chip *chip,
							   const uint8_t *buf, int oob_required)
{
#if USE_SP_BCH
	struct sp_spinand_info *info = &our_spinfc;
#endif

	chip->write_buf(mtd, buf, mtd->writesize);
	chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);

#if USE_SP_BCH
	sp_bch_encode(mtd, info->buff.virt, info->buff.virt + mtd->writesize);
#endif

	return 0;
}

static int sp_spinand_verify_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	void *tmp = malloc(mtd->writesize);
	int ret = 0;

	if (!tmp)
		return -ENOMEM;

	sp_spinand_read_page(mtd, mtd->priv, tmp, 0, -1);
	if (memcmp(buf, tmp, len)) {
		printf("sp_spinand: verify failed\n");
		ret = -EFAULT;
	}

	free(tmp);
	return ret;
}

static int sp_spinand_dev_ready(struct mtd_info *mtd)
{
	struct sp_spinand_info *info = &our_spinfc;

	return ((spi_nand_getfeatures(info, 0xC0) & 0x01) == 0);
}

static uint8_t sp_spinand_read_byte(struct mtd_info *mtd)
{
	struct sp_spinand_info *info = &our_spinfc;

	uint8_t ret = 0;

	switch (info->cmd) {
	case NAND_CMD_STATUS:
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

static void sp_spinand_read_buf(struct mtd_info *mtd,
		u_char *buf, int len)
{
	struct sp_spinand_info *info = &our_spinfc;

#ifdef CONFIG_SP_CBDMA
	unsigned long start = (unsigned long)buf;
	unsigned long stop = start + len;

	invalidate_dcache_range(start, stop);
	sp_cbdma_write(info->buff.phys + info->buff.idx,virt_to_phys(buf), len);
	info->buff.idx += len;
#else
	memcpy(buf, info->buff.virt + info->buff.idx, len);
	info->buff.idx += len;
#endif

}

static void sp_spinand_write_buf(struct mtd_info *mtd,
		const u_char *buf, int len)
{
	struct sp_spinand_info *info = &our_spinfc;

#ifdef CONFIG_SP_CBDMA
	unsigned long start = (unsigned long)buf;
	unsigned long stop = start + len;

	flush_dcache_range(start, stop);
	sp_cbdma_read(virt_to_phys((void *)buf), info->buff.phys + info->buff.idx, len);
	info->buff.idx += len;
#else
	memcpy(info->buff.virt + info->buff.idx, buf, len);
	info->buff.idx += len;
#endif

}

static void sp_spinand_select_chip(struct mtd_info *mtd, int chipnr)
{
	struct sp_spinand_info *info = &our_spinfc;

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

static void sp_spinand_cmd_ctrl(struct mtd_info *mtd,
		int cmd, unsigned int ctrl)
{
	return;
}


static void sp_spinand_cmdfunc(struct mtd_info *mtd,
		unsigned cmd, int col, int row)
{
	struct sp_spinand_info *info = &our_spinfc;

	info->cmd = cmd;
	switch (cmd) {
	case NAND_CMD_READ0:		/* 0x00 */
		sp_spinand_desc_prep(info, cmd, col, row);
		sp_spinand_desc_send(info, 1);
		if (readb(info->buff.virt + mtd->writesize + 1) == 0xFF)
			break;
	case NAND_CMD_READOOB:		/* 0x50 */
		sp_spinand_desc_prep(info, cmd, col, row);
		sp_spinand_desc_send(info, 1);
		break;
	case NAND_CMD_SEQIN:		/* 0x80 */
		sp_spinand_desc_prep(info, cmd, col, row);
		break;
	case NAND_CMD_PAGEPROG:		/* 0x10 */
		sp_spinand_desc_send(info, 1);
		break;
	case NAND_CMD_ERASE1:		/* 0x60 */
		sp_spinand_desc_prep(info, cmd, col, row);
		break;
	case NAND_CMD_ERASE2:		/* 0xD0 */
		sp_spinand_desc_send(info, 1);
		break;
	case NAND_CMD_STATUS:		/* 0x70 */
		sp_spinand_desc_prep(info, cmd, -1, -1);
		sp_spinand_desc_send(info, 1);
		break;
	case NAND_CMD_RESET:		/* 0xFF */
		sp_spinand_reset(info);
		mdelay(5);
		break;
	case NAND_CMD_READID:		/* 0x90 */
		info->buff.idx = 0;
		spi_nand_readid(info,0, info->buff.virt);
		break;
	case NAND_CMD_PARAM:    	/* 0xEC */
	case NAND_CMD_GET_FEATURES:	/* 0xEE */
		sp_spinand_desc_prep(info, cmd, col, -1);
		//sp_spinand_desc_send(info, 1);
		break;
	case NAND_CMD_SET_FEATURES:	/* 0xEF */
		sp_spinand_desc_prep(info, cmd, col, -1);
		break;
	default:
		printf("sp_spinand: unknown command=0x%02x.\n", cmd);
		break;
	}
}

static int sp_spinand_fixup(struct sp_spinand_info *info)
{
	struct mtd_info *mtd = info->mtd;
	struct nand_chip *chip = mtd->priv;
	uint32_t nrpg;

	/* column address */
	info->cac = 2;

	/* row address */
	nrpg = lldiv(chip->chipsize, mtd->writesize);

	if (nrpg & 0xFF000000)
		info->rac = 4;
	else if (nrpg & 0xFFFF0000)
		info->rac = 3;
	else
		info->rac = 2;

	return 0;
}

void board_spinand_init(void)
{
	struct sp_spinand_info *info = &our_spinfc;
	unsigned int id;
	struct nand_chip *nand = &info->nand;
	struct mtd_info *mtd = &nand_info[0];

	info->mtd = mtd;
	mtd->priv = nand;
	nand->IO_ADDR_R = nand->IO_ADDR_W = info->regs;
	printf("sp_spinand: regs@0x%p\n", info->regs);

	info->buff.size = CFG_BUFF_MAX;
	info->buff.phys = CONFIG_SRAM_BASE;
	info->buff.virt = (void *)info->buff.phys;
	printk("sp_spinand: buff=0x%p@0x%08x size=%u\n",
			info->buff.virt, info->buff.phys, info->buff.size);

#ifdef CFG_BBT_USE_FLASH
	//	info->nand.bbt_options    = NAND_BBT_USE_FLASH;
#endif

	info->nand.select_chip    = sp_spinand_select_chip;
	info->nand.cmd_ctrl       = sp_spinand_cmd_ctrl;
	info->nand.cmdfunc        = sp_spinand_cmdfunc;
	info->nand.dev_ready      = sp_spinand_dev_ready;

	info->nand.read_byte      = sp_spinand_read_byte;
	info->nand.read_buf       = sp_spinand_read_buf;
	info->nand.write_buf      = sp_spinand_write_buf;
	info->nand.verify_buf     = sp_spinand_verify_buf;

	info->nand.ecc.read_page  = sp_spinand_read_page;
	info->nand.ecc.write_page = sp_spinand_write_page;
	info->nand.ecc.layout     = &info->ecc_layout;
	info->nand.ecc.mode       = NAND_ECC_HW;

	if( sp_spinand_reset(info) < 0 )
		return;

	/* ==== Read ID ==== */
	spi_nand_readid(info, 0, &id);

	if( (id & 0xFF) == 0xEF ) {
		info->id = (id & 0xFF);
		printf("Winbond SPI NAND found\n");
		printf("read WB status-2:0x%02x\n", spi_nand_getfeatures(info, 0xB0));
#if USE_SP_BCH
		printf("Set WB in BUF-1..& ECC-OFF,using S+ BCH");
		spi_nand_setfeatures(info, 0xB0, 0x08);	/* Dis-able ECC, enable BUF-1 */
#else
		printf("Set WB in BUF-1..& ECC-ON");
		spi_nand_setfeatures(info, 0xB0, 0x18);	/* enable ECC & BUF-1 */
#endif
		printf("...done,status-2;0x%x\n", spi_nand_getfeatures(info, 0xB0));
	} else if (((id & 0xFF) == 0xC8) && ((id & 0xFF00) != 0x2100)) {
		info->id = id&0xFF;
		printf("GigaDevice SPI NAND found & enable QuadIO mode,");
#if USE_SP_BCH
		printf("using S+ BCH\n");
		spi_nand_setfeatures(info, 0xB0, 0x01);  /* Dis-able ECC & ebable QuadIO */
#else
		printf("using device internal ECC\n");
		spi_nand_setfeatures(info, 0xB0, 0x11);  /* enable ECC & QuadIO */
#endif
		printf("read fea:0x%02x\n",spi_nand_getfeatures(info,0xB0));
	} else if ((id & 0xFF) == 0xC2) {	/* MXIC1G:C212 */
		info->id = (id & 0xFF);
		printf("MXIC SPI NAND found\n");
		printf("read MXIC status-2:0x%02x\n", spi_nand_getfeatures(info, 0xB0));
#if USE_SP_BCH
		printf("using S+ BCH");
		spi_nand_setfeatures(info, 0xB0, 0x01); /* Dis-able ECC, enable Quad */
#else
		printf("using device internal ECC\n");
		spi_nand_setfeatures(info, 0xB0, 0x11); /* enable ECC & QuadIO */
#endif
		printf("...done,status-2;0x%x\n",spi_nand_getfeatures(info,0xB0));

	} else if ((id & 0xFFFF) == 0x21C8) {	/* ESMT1G:21c8 */
		info->id = id&0xFFFF;
		printf("ESMT SPI NAND found & enable QuadIO mode,");
#if USE_SP_BCH
		printf("using S+ BCH\n");
		spi_nand_setfeatures(info, 0xB0, 0x01);	/* Dis-able ECC & ebable QuadIO */
#else
		printf("using device internal ECC\n");
		spi_nand_setfeatures(info, 0xB0, 0x11);	/* enable ECC & QuadIO */
#endif
		printf("read fea:0x%02x\n",spi_nand_getfeatures(info, 0xB0));

	} else {
		printf("Unknow SPI NAND device\n");
		return;
	}

	printf("%s\n", __FUNCTION__);
	spi_nand_setfeatures(info, 0xA0, 0x0);

	if (nand_scan_ident(mtd, CONFIG_SP_SPINAND_CSNR, sp_nand_ids))
		return;

	if (sp_spinand_fixup(info) < 0)
		return;

	if(sp_bch_init(mtd))
		return;

	if (nand_scan_tail(mtd))
		return;

	nand_register(0);
	return ;
}

