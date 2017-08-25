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

#include "sp_bch.h"
#include "sp_spinand.h"
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

unsigned long long SPITotal_ISP_Write_Byte = 0;
struct proc_dir_entry *proc_entry_SPINAND;
#define NAND_RW_DBG 1
#if NAND_RW_DBG
int spinand_addr_prompt = 0;
#endif
#endif

#ifdef CONFIG_MTD_SP_NAND_HZ
#define CFG_NAND_HZ		CONFIG_MTD_SP_NAND_HZ
#else
#define CFG_NAND_HZ		304000000 /* 304 MHz */
#endif

#define CFG_BUFF_MAX	(18 << 10)
#define CFG_DESC_MAX	4

#ifndef setbits_le32
#define setbits_le32(reg, val)	writel(readl(reg) | (val), reg)
#endif

#ifndef clrbits_le32
#define clrbits_le32(reg, val)	writel(readl(reg) & (~(val)), reg)
#endif

#define CFG45_DefaultValue	0x15
#define CFG67_DefaultValue	0x95
#define CFG_CMD_TIMEOUT_MS	10

#define DEVICE_STS_AUTO_CHK	1
#define USE_SP_BCH			1   /* 1:Using S+ BCH, 0:using Device internal ECC */
#define BIT(n)          (1 << (n))

struct sp_spinand_info
{
	struct device *dev;
	struct device *dma;
	struct mtd_info mtd;
	struct nand_chip nand;
	wait_queue_head_t wq;
	void __iomem *regs;
	int irq;
	int cs;

	/*
	 * local buffer for NAND <--> SRAM DMA operation
	 *
	 * There is a hardware limitation in letb123:
	 * The NAND & BCH controller are attached to CBUS
	 * which could not access DRAM modules, and thus
	 * zero-copy is not possible.
	 */
	struct {
		uint32_t idx;
		uint32_t size;
		void *virt;
		dma_addr_t phys;
		void *tmp_virt;
		void *tmp_virt_forMXIC2G;
	} buff;

	struct nand_ecclayout ecc_layout;
	int busy;
	int cmd;		/* current command code */
	int cac;		/* col address cycles */
	int rac;		/* row address cycles */
	int id;
	int row;
	/* NAND flash IDs */
	struct nand_flash_dev *ids;
	/* Sunplus Boot Header */
	void *hdr;

#ifdef CONFIG_MTD_SP_NAND_BKER_COUNTER
	uint16_t *blk_erased;
#endif

	struct mutex lock;
	struct semaphore spnand_sem;
};

// #define gbootRom_boot_mode	 (*(volatile unsigned char *)(SRAM0_BASE + 0x9904))
/* Wrapper for do_div(). Doesn't modify dividend and returns
 * the result, not reminder.
 */
static inline uint64_t lldiv(uint64_t dividend, uint32_t divisor)
{
	uint64_t result = dividend;
	do_div(result, divisor);
	return result;
}

static irqreturn_t sp_spinand_irq(int irq, void *dev)
{
	return IRQ_HANDLED;
}

/* check SPI NAND Device status by ctrl_status_registers */
void wait_spi_idle(struct sp_spinand_info *info)
{
	struct sp_spinand_regs *regs = info->regs;

	while (readl(&regs->ctrl)& SPI_DEVICE_IDLE) { /* ctrl bit-15, wait spi_ctrl idle */
		/* wait */;
	}
}

void spi_nand_readid(struct sp_spinand_info *info, uint32_t addr, uint32_t *data)
{
	struct sp_spinand_regs *regs = info->regs;

	writel(addr, &regs->addr_low);
	writel(addr >> 16, &regs->addr_high);
	writel(0x9F8D, &regs->cust_cmd);

	wait_spi_idle(info);
	*data = readl(&regs->data_low) | (readl(&regs->data_high) << 16);
}

int spi_nand_getfeatures(struct sp_spinand_info *info, uint32_t addr)
{
	struct sp_spinand_regs *regs = info->regs;

	/* cfg-4 & cfg-6 RGST bus( for Data_64 port usage ) */
	writel(CFG45_DefaultValue, &regs->cfg4); //0x15
	writel(CFG67_DefaultValue, &regs->cfg6); //0x95
	writel(addr, &regs->addr_low);
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
}

static int sp_spinand_reset(struct sp_spinand_info *info)
{
	struct sp_spinand_regs *regs = info->regs;
	int ret=-1;

	while (readl(&regs->ctrl) & SPI_DEVICE_IDLE) {
		/* wait */ ;
	}
	writel(0x211C, &regs->ctrl); /* Flash A, auto_wel, clk/8 */
	writel(readl(&regs->wait) | SPI_NAND_ENABLE, &regs->wait);
	writel(1, &regs->cfg3);
	writel(CFG45_DefaultValue, &regs->cfg4);
	writel(CFG67_DefaultValue, &regs->cfg6);

	/* ==== Flash reset ==== */
	writel(0xFF80, &regs->cust_cmd);
	writel(0x0583, &regs->cust_cmd);
	wait_spi_idle(info);
	ret=spi_nand_getfeatures(info, 0xC0);

	return ret;
}

void spi_nand_pageread2cache(struct sp_spinand_info *info, uint32_t addr)
{
	struct sp_spinand_regs *regs = info->regs;

	// RGST bus
	writel(0x05, &regs->cfg4);
	writel(0x05, &regs->cfg6);

	addr &= 0x00FFFFFF;		/* 3 byte addr cnt */
	writel(addr, &regs->addr_low);
	writel(addr >> 16, &regs->addr_high);

	writel(0x1383, &regs->cust_cmd);  /* 3 byte addr count -> 1Gbit:1 dummy byte address + 2 byte(page/block) address */
	wait_spi_idle(info);

	writel(0x0583, &regs->cust_cmd);
	wait_spi_idle(info);

}

/* ==== Read From cache x4 ==== */
void spi_nand_readcacheQuadIO_byMapping(struct sp_spinand_info *info, uint32_t addr, unsigned int size, uint32_t *pbuf)
{
	struct sp_spinand_regs *regs = info->regs;
	 
	unsigned int cfg1 = 0, cfg5 = 0, cfg7 = 0;

	if (info->id == 0x21C8) {	/* ESMT */
#if USE_SP_BCH
		spi_nand_setfeatures(info, 0xB0, 0x01);	/* en-able QuadIO,ECC-off */
#else
		spi_nand_setfeatures(info, 0xB0, 0x11);	/* en-able QuadIO,ECC-on */
#endif
		cfg5 = 0x0435;
		cfg7 = 0x05;
		cfg1 = 0x6B10;
	} else if (info->id == 0xC8) {	/* GD */
#if USE_SP_BCH
		spi_nand_setfeatures(info, 0xB0, 0x01);	/* en-able QuadIO,ECC-off */
#else
		spi_nand_setfeatures(info, 0xB0, 0x11);	/* en-able QuadIO,ECC-on */
#endif
		cfg5 = 0x013D;
		cfg7 = 0x01;
		cfg1 = 0xEB10;
	} else if (info->id == 0xEF) {	/* WB */
#if USE_SP_BCH
		spi_nand_setfeatures(info, 0xB0, 0x08);	/* ECC-off ,BUF-1 */
#else
		spi_nand_setfeatures(info, 0xB0, 0x18);	/* ECC on & BUF-1 mode on */
#endif
		cfg5 = 0x023D;
		cfg7 = 0x01;
		cfg1 = 0xEB10;
	} else if (info->id == 0xC2) {	/* MXIC */
#if USE_SP_BCH
		spi_nand_setfeatures(info,0xB0,0x01);	/* en-able QuadIO,ECC-off */
#else
		spi_nand_setfeatures(info,0xB0,0x11);	/* en-able QuadIO,ECC-on */
#endif
		cfg5 = 0x0435;
		cfg7 = 0x05;
		cfg1 = 0x6B10;
	} else {
		printk("Error Device ID\n");
		return ;
	}

	writel(cfg5, &regs->cfg5);
	writel(cfg7, &regs->cfg7);
	writel(cfg1, &regs->cfg1);
#if 0
	for ( i = addr; i < size; i += 4)
		*(unsigned int*)pbuf++ = *(unsigned int *)((info->buff.tmp_virt) + i);
	//memcpy(pbuf, (info->buff.tmp_virt), size);
#endif
#if 1
	if ((info->row & (0x40)) && ((info->id & 0xFF) == 0xC2))
		memcpy(pbuf, (info->buff.tmp_virt_forMXIC2G), size);
	else
		memcpy(pbuf, (info->buff.tmp_virt), size);
#endif

	wait_spi_idle(info);

}


void spi_nand_wren(struct sp_spinand_info *info)
{
	struct sp_spinand_regs *regs = info->regs;

	writel(CFG45_DefaultValue, &regs->cfg4);	/* 0x15 1bit mode(cmd , data, addr ) */
	writel(CFG67_DefaultValue, &regs->cfg6);	/* 0x95  DQ1 input&output enable,addr and cmd DQ0 output enable */
	writel(0x0680, &regs->cust_cmd);
}

void spi_nand_wrdi(struct sp_spinand_info *info)
{
	struct sp_spinand_regs *regs = info->regs;

	writel(CFG45_DefaultValue, &regs->cfg4);
	writel(CFG67_DefaultValue, &regs->cfg6);
	writel(0x0480, &regs->cust_cmd);
}

int spi_nand_blkerase(struct sp_spinand_info *info, uint32_t addr)
{
	struct sp_spinand_regs *regs = info->regs;
	unsigned short cmd_data=0;
	int ret;

	writel(CFG45_DefaultValue, &regs->cfg4);
	writel(CFG67_DefaultValue, &regs->cfg6);

	cmd_data = (SPINAND_CMD_BLKERASE << SPI_CUSTCMD_SHIFT) | SPI_CUSTCMD_ENABLE | SPI_CUSTCMD_WRITE | SPINAND_CUSTCMD_3_ADDR;

	addr &= 0x00FFFFFF;			/* for 1Gbit:1 dummy byte address + 2 byte(page/block) address */
	writel(addr, &regs->addr_low);
	writel(addr >> 16, &regs->addr_high);
	/* 0xD8A3;  --> cmd:0xD8;Block Erase, 0xA bit 7 & 5 is 1, bit-5 =1 means writing to Flash */
	writel(cmd_data, &regs->cust_cmd);

	writel(0x0583, &regs->cust_cmd);
	wait_spi_idle(info);

	ret = spi_nand_getfeatures(info, 0xC0);

	return (ret & 0x04); // bit-2 erase fail
}

void spi_nand_progloadx4(struct sp_spinand_info *info, uint32_t addr, uint8_t *pbuf, unsigned int size)
{
	struct sp_spinand_regs *regs = info->regs;
	uint16_t *pdata = (uint16_t *)pbuf;
	unsigned int len = 0;
	int ret,i;

	writel(0x211C, &regs->ctrl);		/* SPI_CLK/8 */

	/* RGST bus cfg */
	writel(0x35, &regs->cfg4);		/* 0x35 -> 4-bit data, 1-bit cmd & addr */
	writel(0x05, &regs->cfg6);

#if 1
	if (((info->row) & (0x40)) && (info->id & 0xff) == 0xc2) { //MXIC-2G
		addr = (addr | BIT(12));
	}
#endif
	writel(addr & 0xFFFF, &regs->addr_low);
	writel(addr >> 16, &regs->addr_high);

	len = ((size & 0x01FF) | (((size & 0x0E00) << 2)) | (1 << 10));	/* bit-10 using Data_64 port */
	writel(len, &regs->cust_cmd_2);

	writel(0x32A2, &regs->cust_cmd);	/* 0x32 is program loadx4,0xa-> bit7:cmd enable */

	for (i = 0 ; i < size / 2 ; i++)
		writel(pdata[i], &regs->data_64);

	wait_spi_idle(info);
	writel(0, &regs->cust_cmd_2);		/* SW have to clear cust_cmd_2 bit-10 function */
	writel(0x0583, &regs->cust_cmd);	/* HW will send get feature,cmd=0x0f,addr=0xc0 */
	wait_spi_idle(info);
	ret = spi_nand_getfeatures(info, 0xC0);

}

int spi_nand_progexecute(struct sp_spinand_info *info, uint32_t addr)
{
	struct sp_spinand_regs *regs = info->regs;
	int ret;

	writel(0x05, &regs->cfg4);
	writel(0x05, &regs->cfg6);

	addr &= 0x00FFFFFF;				/* 1Gbit:1 dummy byte address + 2 byte(page/block) address,using 3 byte address */
	writel(addr, &regs->addr_low);
	writel(addr >> 16, &regs->addr_high);
	writel(0x10A3, &regs->cust_cmd);
	wait_spi_idle(info);

	writel(0x0583, &regs->cust_cmd);		/* HW will send get feature,cmd=0x0f,addr=0xc0, */
	wait_spi_idle(info);
	ret = spi_nand_getfeatures(info, 0xC0);

#if !USE_SP_BCH
	printk("Prog Execute is done ...");
	if (ret & 0x08)
		printk("but fail\n");
	else
		printk("ok\n");
#endif

	return ret;
}


static int sp_spinand_wait(struct sp_spinand_info *info)
{
	struct sp_spinand_regs *regs = info->regs;

	while (readl(&regs->ctrl) & SPI_DEVICE_IDLE) {
		/* wait */
	}
	return 0;
}

static int sp_spinand_desc_prep(struct sp_spinand_info *info,
	uint8_t cmd, int col, int row)
{
	mutex_lock(&info->lock);
	switch (cmd) {
	case NAND_CMD_READOOB:		/* 0x50 */
		col += info->mtd.writesize;
		/* fall through */

	case NAND_CMD_READ0:		/* 0x00 */
		info->cmd = SPINAND_CMD_PAGE2CACHE;
		info->buff.idx = col;
		info->row = row;
		spi_nand_pageread2cache(info, row & 0xFFFF);
		spi_nand_readcacheQuadIO_byMapping(info, 0,
						  ((info->mtd.writesize) + (info->mtd.oobsize)),
						  (unsigned int*)info->buff.virt);
		break;

	case NAND_CMD_SEQIN:		/* 0x80 */
		info->cmd = SPINAND_CMD_PROLOADx4;
		info->buff.idx = col;
		spi_nand_setfeatures(info, 0xA0, 0x0);
		spi_nand_wren(info);
		info->row = row & 0xFFFF;
		break;

	case NAND_CMD_ERASE1:		/* 0x60 */
		info->cmd = SPINAND_CMD_BLKERASE;
		spi_nand_wren(info);	/* for MXIC,ESMT */
		spi_nand_setfeatures(info, 0xA0, 0x0);
		spi_nand_wren(info);
		spi_nand_blkerase(info, row & 0xFFC0); /* bit-5~0 is page address */
		break;

	case NAND_CMD_STATUS:		/* 0x70 */
		info->cmd = SPINAND_CMD_GETFEATURES;
		info->buff.idx = 0;
		if (spi_nand_getfeatures(info, 0xA0) & 0x38) {
			*(unsigned int*)info->buff.virt = 0x0; /* protected */
		} else {
			*(unsigned int*)info->buff.virt = 0x80; /* not protectd. comply w/ raw NAND */
		}
		break;

	case NAND_CMD_READID:		/* 0x90 */
	case NAND_CMD_PARAM:		/* 0xEC */
	case NAND_CMD_GET_FEATURES: 	/* 0xEE */
	case NAND_CMD_SET_FEATURES: 	/* 0xEF */
	case NAND_CMD_RESET:		/* 0xFF */
		break;
	default:
		BUG();
		break;
	}

	return 0;
}
EXPORT_SYMBOL(sp_spinand_desc_prep);

static int sp_spinand_desc_send(struct sp_spinand_info *info)
{
	int ret;

	if (info->cmd == NAND_CMD_PAGEPROG ) {
		info->cmd = SPINAND_CMD_PROEXECUTE;

#if USE_SP_BCH
		spi_nand_progloadx4(info, 0, (unsigned char *)info->buff.virt, info->mtd.writesize + info->mtd.oobsize);
#else
		spi_nand_progloadx4(info, 0, (unsigned char *)info->buff.virt, info->mtd.writesize);
#endif
		spi_nand_progexecute(info, info->row);
	}

	ret = sp_spinand_wait(info);
	mutex_unlock(&info->lock);
	if (ret) {
		pr_info("sp_spinand: timeout, cmd=0x%x\n", info->cmd);
		sp_spinand_reset(info);
	}

	return ret;
}


static int sp_spinand_read_page(struct mtd_info *mtd,
	struct nand_chip *chip, uint8_t *buf, int page)
{

#if USE_SP_BCH
	struct sp_spinand_info *info = container_of(mtd, struct sp_spinand_info, mtd);
	int ret,i;

	down(&info->spnand_sem);

	ret = sp_bch_decode(mtd, info->buff.phys, info->buff.phys + mtd->writesize);
	
	if (ret)
		printk("sp_spinand: bch decode failed at page=%d\n", page);
#endif

	chip->read_buf(mtd, buf, mtd->writesize);


#if NAND_RW_DATASIZE_FOR_ISP
	SPITotal_ISP_Write_Byte += mtd->writesize;
#endif

	if (page < 0)
		up(&info->spnand_sem);
	else {
		chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);
		up(&info->spnand_sem);
	}
	return 0;
}


static void sp_spinand_write_page(struct mtd_info *mtd,
	struct nand_chip *chip, const uint8_t *buf)
{

#if USE_SP_BCH
	struct sp_spinand_info *info = container_of(mtd, struct sp_spinand_info, mtd);
#endif

	down(&info->spnand_sem);
	chip->write_buf(mtd, buf, mtd->writesize);
	chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);

#if USE_SP_BCH
	sp_bch_encode(mtd, info->buff.phys, info->buff.phys + mtd->writesize);
#endif

#if NAND_RW_DATASIZE_FOR_ISP
	SPITotal_ISP_Write_Byte += mtd->writesize;
#endif
	up(&info->spnand_sem);
}

static int sp_spinand_verify_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	int ret = 0;
	void *tmp = kmalloc(mtd->writesize, GFP_KERNEL);

	if (!tmp)
		return -ENOMEM;

	sp_spinand_read_page(mtd, mtd->priv, tmp, -1);
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
static int sp_spinand_dev_ready(struct mtd_info *mtd)
{
	//struct sp_spinand_info *info = container_of(mtd, struct sp_spinand_info, mtd);
	//struct sp_spinand_regs *regs = info->regs;

	//return (spi_nand_getfeatures(info,0xc0)&0x01) == 0;

	return 1;
}

static uint8_t sp_spinand_read_byte(struct mtd_info *mtd)
{
	struct sp_spinand_info *info = container_of(mtd, struct sp_spinand_info, mtd);
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
	struct sp_spinand_info *info = container_of(mtd, struct sp_spinand_info, mtd);
	/* refer nand_read_buf() callback function at every nand driver,
	 * they all use pio mode NOT to use dma mode.
	 *
	 * we do an experiment to try to use dma mode when buf is vaild.
	 * it will increade performance but it may have side effect.
	 * if yes then we will use completely pio mode at this function. */

	if (!virt_addr_valid(buf) || !virt_addr_valid(buf + len - 1)) {
		memcpy(buf, info->buff.virt + info->buff.idx, len);
		info->buff.idx += len;
		printk("\n%s:%d", __FUNCTION__, __LINE__);
		return;
	}

#ifdef CONFIG_MTD_SP_NAND_CBDMA
		dma_addr_t src = (dma_addr_t)info->buff.phys + info->buff.idx;

		dma_addr_t dst = dma_map_single(info->dev, buf, len, DMA_FROM_DEVICE);

		sp_cbdma_write(info->dma, src, dst, len); //sram -> DRAM

		dma_unmap_single(info->dev, dst, len, DMA_FROM_DEVICE);
#else
		memcpy(buf, info->buff.virt + info->buff.idx, len);
#endif

	info->buff.idx += len;

}

static void sp_spinand_write_buf(struct mtd_info *mtd,
				    const u_char *buf, int len)
{
	struct sp_spinand_info *info = container_of(mtd, struct sp_spinand_info, mtd);

	if (!virt_addr_valid(buf) || !virt_addr_valid(buf + len - 1)) {
		memcpy(info->buff.virt + info->buff.idx, buf, len);
		info->buff.idx += len;

		if (info->cmd == NAND_CMD_SET_FEATURES)
			sp_spinand_desc_send(info);

		return;
	}

#ifdef CONFIG_MTD_SP_NAND_CBDMA
		dma_addr_t src = dma_map_single(info->dev, (void *)buf, len, DMA_TO_DEVICE);
		dma_addr_t dst = (dma_addr_t)info->buff.phys + info->buff.idx;

		sp_cbdma_read(info->dma, src, dst, len); //DRAM -> sram

		dma_unmap_single(info->dev, src, len, DMA_TO_DEVICE);
#else
		memcpy(info->buff.virt + info->buff.idx, buf, len);
#endif

	info->buff.idx += len;
	if (info->cmd == NAND_CMD_SET_FEATURES)
	{
		printk("%d\t",__LINE__);
		sp_spinand_desc_send(info);
	}
}

static void sp_spinand_cmdfunc(struct mtd_info *mtd,
				       unsigned cmd, int col, int row)
{
	struct sp_spinand_info *info = container_of(mtd, struct sp_spinand_info, mtd);
	struct sp_spinand_regs *regs = info->regs;
	uint32_t val;

	info->cmd = cmd;

	switch (cmd) {
	case NAND_CMD_READ0:	/* 0x00 */
		sp_spinand_desc_prep(info, cmd, col, row);
		sp_spinand_desc_send(info);
		if (readb(info->buff.virt + mtd->writesize + 1) == 0xFF)
			break;

	case NAND_CMD_READOOB:	/* 0x50 */
		sp_spinand_desc_prep(info, cmd, col, row);
		sp_spinand_desc_send(info);
		break;

	case NAND_CMD_SEQIN:	/* 0x80 */
		sp_spinand_desc_prep(info, cmd, col, row);
		break;

	case NAND_CMD_PAGEPROG:	/* 0x10 */
		sp_spinand_desc_send(info);
		break;

	case NAND_CMD_ERASE1:	/* 0x60 */
		sp_spinand_desc_prep(info, cmd, col, row);
		break;

	case NAND_CMD_ERASE2:	/* 0xD0 */
		sp_spinand_desc_send(info);
		break;

	case NAND_CMD_STATUS:	/* 0x70 */
		sp_spinand_desc_prep(info, cmd, -1, -1);
		sp_spinand_desc_send(info);
		break;

	case NAND_CMD_RESET:	/* 0xFF */
		sp_spinand_reset(info);
		mdelay(5);
		break;

	case NAND_CMD_READID:	/* 0x90 */
		info->buff.idx=0;
		spi_nand_readid(info, 0, info->buff.virt);
		break;

	case NAND_CMD_PARAM:	/* 0xEC */
	case NAND_CMD_GET_FEATURES:	/* 0xEE */
		sp_spinand_desc_prep(info, cmd, col, -1);
		sp_spinand_desc_send(info);
		//mdelay(5);
		break;

	case NAND_CMD_SET_FEATURES:	/* 0xEF */
		sp_spinand_desc_prep(info, cmd, col, -1);
		sp_spinand_desc_send(info);
		//mdelay(5);
		break;

	default:
		break;
	}
}

/**
 * nand_select_chip - control CE line
 * @mtd:	MTD device structure
 * @chipnr:	chipnumber to select, -1 for deselect
 */
//modified
static void sp_spinand_select_chip(struct mtd_info *mtd, int chipnr)
{
	struct sp_spinand_info *info = container_of(mtd, struct sp_spinand_info, mtd);

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

#define LP_OPTIONS  (0x00000004|0x00000008|0x00000010)
#define NAND_LP(name, id, ext_id, pgsz, bksz, size) \
         { name, id, pgsz, size, bksz, LP_OPTIONS, ext_id }

struct nand_flash_dev spinand_flash_ids2[] = {
/* GigaDevice SPI NAND  */
    NAND_LP("GD5F1GQ4UAYIG", 0xC8D1, 0x0, (2048 + 64), 		SZ_128K, 128),
    NAND_LP("GD5F2GQ4UAYIG", 0xC8D2, 0x0, (2048 + 64),		SZ_128K, 256),

/* MXIC  */
    NAND_LP("MX30LF1G18AC", 0xC2F1, 0x80950200, (2048 + 64), 	SZ_128K, 128),

/* Winbond SPI NAND  */
    NAND_LP("W25N01GVxx1G", 0xEFAA, 0, (2048 + 64),  		SZ_128K, 128),
    NAND_LP("W25N02GVxx2G", 0xEFAB, 0, (2048 + 64),  		SZ_128K, 256),

/* MXIC SPI NAND  */
    NAND_LP("MX35LF1GE4AB", 0xC212, 0, (2048 + 64),  		SZ_128K, 128),
    NAND_LP("MX35LF2GE4AB", 0xC222, 0, (2048 + 64),  		SZ_128K, 256),

/* ESMT SPI NAND  */
    NAND_LP("ESMTF50L1G41A", 0xC821, 0, (2048 + 64), 		SZ_128K, 128)
};

static int sp_spinand_loadids(struct sp_spinand_info *info, const char *fn)
{
	int num, num_spinand_flash_ids;
	struct nand_flash_dev *ptr;

	num_spinand_flash_ids = 0;
	for (ptr = nand_flash_ids ; ptr->name ; ptr++) {
		num_spinand_flash_ids++;
	}

	num = ARRAY_SIZE(spinand_flash_ids2) + num_spinand_flash_ids;

	info->ids = kzalloc(num * sizeof(struct nand_flash_dev), GFP_KERNEL);
	if (!info->ids) {
		printk("sp_spinand: out of memory\n");
		info->ids = nand_flash_ids;
		printk("sp_spinand: %s\n", __func__);
		printk("\nsp_spinand: %s:%d\t%s return -ENOMEM", __FILE__, __LINE__, __FUNCTION__);
		return -ENOMEM;
	}

	memcpy( info->ids, spinand_flash_ids2, sizeof(spinand_flash_ids2));
	memcpy( &(info->ids[ARRAY_SIZE(spinand_flash_ids2)]), nand_flash_ids, num_spinand_flash_ids*sizeof(struct nand_flash_dev));

	return 0;
}


static int sp_spinand_fixup(struct sp_spinand_info *info)
{
	struct mtd_info *mtd = &info->mtd;
	struct nand_chip *chip = mtd->priv;
	uint64_t nrpg;

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

static void sp_spinand_cmd_ctrl(struct mtd_info *mtd,
	int cmd, unsigned int ctrl)
{
	return;
}

static ssize_t sp_spinand_sysfs_get_info(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	printk("\n%s:%d\t%s", __FILE__, __LINE__, __FUNCTION__);
	struct sp_spinand_info *info = dev_get_drvdata(dev);
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

static ssize_t sp_spinand_sysfs_get_erased(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct sp_spinand_info *info = dev_get_drvdata(dev);
	struct mtd_partition *part;
	uint64_t offset, length, ertot;
	ssize_t ret = 0;
	int i, b, start, end, eravg, ermax, ermax_bk, ermin, ermin_bk;

	for (i = 1 ; i < info->nr_parts ; ++i) {
		part = info->parts + i;

		offset = part->offset;
		length = part->size ? part->size : info->nand.chipsize - offset;

		start = (int)lldiv(offset, info->mtd.erasesize);
		end = (int)lldiv(offset + length - 1, info->mtd.erasesize);

		ertot = 0;
		ermax = -1;
		ermax_bk = start;
		ermin = 0x01FFFF;
		ermin_bk = start;
		for (b = start ; b <= end ; ++b) {
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
#endif	/* CONFIG_MTD_SP_NAND_BKER_COUNTER */

static struct device_attribute sp_spinand_attrs[] = {
	__ATTR(info, S_IRUGO, sp_spinand_sysfs_get_info, NULL),
#ifdef CONFIG_MTD_SP_NAND_BKER_COUNTER
	__ATTR(erased, S_IRUGO, sp_spinand_sysfs_get_erased, NULL),
#endif
};

static int sp_spinand_probe(struct platform_device *pdev)
{
	int i, ret = 0;
	struct resource *res;
	struct sp_spinand_info *info;
	struct mtd_part_parser_data data;

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

	sema_init(&info->spnand_sem, 1);
	mutex_init(&info->lock);

#ifdef CONFIG_MTD_SP_NAND_CBDMA
	info->dma = sp_cbdma_getbyname(CONFIG_MTD_SP_NAND_CBDMA_NAME);
	if (!info->dma) {
		printk("sp_spinand: dma not found!\n");
		kfree(info);
		return -ENXIO;
	}
	printk("sp_spinand: cbus dma enabled\n");
#endif

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		kfree(info);
		printk("sp_spinand: %s:[%d]\n", __FILE__, __LINE__);
		return -ENXIO;
	}

	if (!request_mem_region(res->start, resource_size(res), "sp_spinand")) {
		kfree(info);
		printk("sp_spinand: %s:[%d]\n", __FILE__, __LINE__);
		return -EBUSY;
	}

	platform_set_drvdata(pdev, info);
	info->hdr       = pdev->dev.platform_data;
	info->dev       = &pdev->dev;
	info->mtd.priv  = &info->nand;
	info->mtd.name  = dev_name(info->dev);
	info->mtd.owner = THIS_MODULE;
	info->buff.tmp_virt = ioremap(0x94000000, 2112);
	info->buff.tmp_virt_forMXIC2G = ioremap(0x94001000, 2112); /* for MXIC2G */
	init_waitqueue_head(&info->wq);

	info->regs = ioremap(res->start, resource_size(res));
	if (!info->regs) {
		ret = -ENOMEM;
		printk("sp_spinand: %s:[%d]\n", __FILE__, __LINE__);
		goto err1;
	}

	if (sp_spinand_reset(info) < 0) {
		ret = -ENXIO;
		printk("sp_spinand: %s:[%d]\n", __FILE__, __LINE__);
		goto err1;
	}

	info->irq = platform_get_irq(pdev, 0);
	info->buff.size = CFG_BUFF_MAX;
	info->buff.phys = CONFIG_MTD_SP_NAND_SRAM_BASE;
	info->buff.virt = ioremap(info->buff.phys, info->buff.size);
	if (!info->buff.virt) {
		ret = -ENOMEM;
		printk("sp_spinand: %s:[%d]\n",__FILE__,__LINE__);
		goto err1;
	}

	printk("sp_spinand: buff=0x%p@0x%08x size=%u\n",
		info->buff.virt, info->buff.phys, info->buff.size);

	info->nand.select_chip    = sp_spinand_select_chip;
	info->nand.cmd_ctrl       = sp_spinand_cmd_ctrl;
	info->nand.cmdfunc        = sp_spinand_cmdfunc;
	info->nand.dev_ready      = sp_spinand_dev_ready;
	info->nand.chip_delay     = 0;

	info->nand.read_byte      = sp_spinand_read_byte;
	info->nand.read_buf       = sp_spinand_read_buf;
	info->nand.write_buf      = sp_spinand_write_buf;
	info->nand.verify_buf     = sp_spinand_verify_buf;

	info->nand.ecc.read_page  = sp_spinand_read_page;
	info->nand.ecc.write_page = sp_spinand_write_page;
	info->nand.ecc.layout     = &info->ecc_layout;
	info->nand.ecc.mode       = NAND_ECC_HW;

	/* ==== Read ID ==== */
	unsigned int id;
	spi_nand_readid(info, 0, &id);

	if ((id & 0xFF) == 0xEF) {
		info->id = (id & 0xFF);
		printk("Winbond SPI NAND found\n");
		printk("read WB status-2:0x%02x\n", spi_nand_getfeatures(info, 0xB0));
#if USE_SP_BCH
		printk("Set WB in BUF-1..& ECC-OFF,using S+ BCH");
		spi_nand_setfeatures(info, 0xB0, 0x08);		/* Dis-able ECC, enable BUF-1 */
#else
		printk("Set WB in BUF-1..& ECC-ON");
		spi_nand_setfeatures(info, 0xB0, 0x18);		/* enable ECC & BUF-1 */
#endif
		printk("...done,status-2;0x%x\n", spi_nand_getfeatures(info, 0xB0));

	} else if (((id & 0xFF) == 0xC8) && ((id & 0xFF00) != 0x2100)) {
		info->id = (id & 0xFF);
		printk("GigaDevice SPI NAND found & enable QuadIO mode,");
#if USE_SP_BCH
		printk("using S+ BCH\n");
		spi_nand_setfeatures(info, 0xB0, 0x01);		/* Dis-able ECC & ebable QuadIO */
#else
		printk("using device internal ECC\n");
		spi_nand_setfeatures(info, 0xB0, 0x11);		/* enable ECC & QuadIO */
#endif
		printk("read fea:0x%02x\n",spi_nand_getfeatures(info, 0xB0));

	} else if ((id & 0xFF) == 0xC2) {
		info->id = (id & 0xFF);
		printk("MXIC SPI NAND found\n");
		printk("read MXIC status-2:0x%02x\n", spi_nand_getfeatures(info, 0xB0));
#if USE_SP_BCH
		printk("using S+ BCH");
		spi_nand_setfeatures(info, 0xB0, 0x01);		/* Dis-able ECC, enable Quad */
#else
		printk("using device internal ECC\n");
		spi_nand_setfeatures(info, 0xB0, 0x11);		/* enable ECC & QuadIO */
#endif
		printk("...done,status-2;0x%x\n", spi_nand_getfeatures(info, 0xB0));

	} else if ((id & 0xFFFF) == 0x21C8 ) {
		info->id = (id & 0xFFFF);
		printk("ESMT SPI NAND found & enable QuadIO mode,");
#if USE_SP_BCH
		printk("using S+ BCH\n");
		spi_nand_setfeatures(info, 0xB0, 0x01);  /* Dis-able ECC & ebable QuadIO */
#else
		printk("using device internal ECC\n");
		spi_nand_setfeatures(info, 0xB0, 0x11);  /* enable ECC & QuadIO */
#endif
		printk("read fea:0x%02x\n", spi_nand_getfeatures(info, 0xB0));

	} else {
	  printk("Unknow SPI NAND device\n");
	  return;
	}

	spi_nand_setfeatures(info, 0xA0, 0x0);
	sp_spinand_loadids(info, "name");
	/**
	 * scan for NAND devices attached to the controller
	 * this is the first stage in a two step process to register
	 * with the nand subsystem
	 */

	if (nand_scan_ident(&info->mtd, CONFIG_MTD_SP_NAND_CSNR, info->ids)) {
		ret = -ENXIO;
		printk("sp_spinand: %s:[%d]\n",__FILE__,__LINE__);
		goto err1;
	}

	if (sp_spinand_fixup(info) < 0) {
		ret = -ENXIO;
		printk("sp_spinand: %s:[%d]\n", __FILE__, __LINE__);
		goto err1;
	}

	printk("\n sp_spinand: page=%u+%u, block=%uKB, chip=%lluMB (adrc=%u+%u)\n",
		info->mtd.writesize, info->mtd.oobsize, info->mtd.erasesize / SZ_1K,
		lldiv(info->nand.chipsize, SZ_1M), info->cac, info->rac);

	if (sp_bch_init(&info->mtd) < 0) {
		ret = -ENXIO;
		printk("sp_spinand: %s:[%d]\n", __FILE__, __LINE__);
		goto err1;
	}

	if (nand_scan_tail(&info->mtd)) {
		ret = -ENXIO;
		printk("sp_spinand: %s:[%d]\n",__FILE__,__LINE__);
		goto err1;
	}

#ifdef CONFIG_MTD_SP_NAND_BKER_COUNTER
	i = (int)lldiv(info->nand.chipsize, info->mtd.erasesize);
	info->blk_erased = kzalloc(2 * i, GFP_KERNEL);
	if (!info->blk_erased) {
		ret = -ENOMEM;
		printk("sp_spinand: %s:[%d]\n", __FILE__, __LINE__);
		goto err1;
	}
#endif

	/* create sysfs */
	for (i = 0; i < ARRAY_SIZE(sp_spinand_attrs); ++i) {
		ret = device_create_file(info->dev, sp_spinand_attrs + i);
		if (ret) {
			printk("sp_spinand: %s:[%d]\n",__FILE__,__LINE__);
			goto err1;
		}
	}

	data.origin = (unsigned long)info->hdr;
	ret = mtd_device_parse_register(&info->mtd, part_types, &data, NULL, 0);

	return ret;

err1:
	release_mem_region(res->start, resource_size(res));

	if (info->regs)
		iounmap(info->regs);

	if (info->buff.virt)
		iounmap(info->buff.virt);

#ifdef CONFIG_PM_WARP
        if (info->irq)
                free_irq(info->irq, info);
#endif

	kfree(info);
	return ret;
}

static int sp_spinand_remove(struct platform_device *pdev)
{
	struct sp_spinand_info *info = platform_get_drvdata(pdev);
	struct resource *res;

	nand_release(&info->mtd);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res)
		release_mem_region(res->start, resource_size(res));

	if (info->regs)
		iounmap(info->regs);

	if (info->buff.virt)
		iounmap(info->buff.virt);

	kfree(info);

	platform_set_drvdata(pdev, NULL);

	return 0;
}

#ifdef CONFIG_PM
static int sp_spinand_suspend(struct platform_device *pdev, pm_message_t mesg)
{
	return 0;
}

static int sp_spinand_resume(struct platform_device *pdev)
{
	struct sp_spinand_info *info = platform_get_drvdata(pdev);
	sp_spinand_reset(info);
	return 0;
}
#else
#define sp_spinand_suspend	NULL
#define sp_spinand_resume	NULL
#endif

/* work with "modprobe sp_spinand" from hotplugging or coldplugging */
MODULE_ALIAS("platform:sp_spinand");

static struct platform_driver sp_spinand_driver = {
	.probe		= sp_spinand_probe,
	.remove		= sp_spinand_remove,
	.suspend	= sp_spinand_suspend,
	.resume		= sp_spinand_resume,
	.driver		= {
		.name	= "sp_spinand",
		.owner	= THIS_MODULE,
	},
};

#if NAND_RW_DATASIZE_FOR_ISP
int SPINAND_proc_read( char *page, char **start, off_t off,
                   int count, int *eof, void *data )
{
	int len;

	if (off > 0) {
		*eof = 1;
		return 0;
	}

	len = sprintf(page, "%lld", SPITotal_ISP_Write_Byte);

	// printk("len:%d,%s,Nand Write %lld Bytes\n",len,page,TotalISP_Write_Byte);
	return len;
}

ssize_t SPINAND_proc_write(struct file *filp, const char __user *buff,
                        unsigned long len, void *data )
{
	unsigned char temp[80];
	memset(temp, 80, 0);

	if (copy_from_user(temp, buff, len)) {
		return -EFAULT;
	}
	temp[len-1] = 0;

	if (strncmp(temp, "clear", 5) == 0) {
		SPITotal_ISP_Write_Byte = 0;
	}

#if NAND_RW_DBG
	if (strncmp(temp, "addr_on", 7) == 0) {
		spinand_addr_prompt = 1;
	} else if (strncmp(temp, "addr_off", 8) == 0) {
		spinand_addr_prompt = 0;
	}

#endif
  return len;
}
#endif

static int __init sp_spinand_init(void)
{
#if NAND_RW_DATASIZE_FOR_ISP
	proc_entry_SPINAND = create_proc_entry("spinand_rw_cnt", 0644, NULL);
	if (proc_entry_SPINAND == NULL) {
		printk("create proc for SPINAND is failed\n");
		return -ENOMEM;
	} else {
		proc_entry_SPINAND->read_proc = SPINAND_proc_read;
		proc_entry_SPINAND->write_proc = SPINAND_proc_write;
	}
#endif

	return platform_driver_register(&sp_spinand_driver);
}

static void __exit sp_spinand_exit(void)
{
	platform_driver_unregister(&sp_spinand_driver);
}

module_init(sp_spinand_init);
module_exit(sp_spinand_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TEST");
MODULE_DESCRIPTION("Sunplus SPINAND flash controller");

