#ifndef __SP_SPINAND_H
#define __SP_SPINAND_H

struct sp_spinand_regs
{
	uint32_t ctrl;		/* SPI NAND Control register, Group 87 */
	uint32_t wait;		/* bit 11 for enable SPI NAND function, setup,hold and disactive cycles */
	uint32_t cust_cmd;	/* customized command setup & enable, write-to/read-from flash, address & data byte count */

	uint32_t addr_low;	/* spi address low register,bit15~0 */
	uint32_t addr_high;	/* spi address low register,bit31~16 */
	uint32_t data_low;	/* spi data low register,bit15~0 */
	uint32_t data_high;	/* spi data high register,bit31~16 */

	uint32_t status;	/*  spi nand device status; status feed back from spi device */

	/*  spi mode1/mode3, 1,2,4 bit data, customer defined command,number of dummy cycle...  */
	uint32_t cfg0;
	uint32_t cfg1;		/* cfg-1:user defined read command & enable,i.,. read cache quad IO */
	uint32_t cfg2;		/* cfg-2:user defined write command & enable */
	uint32_t cfg3;		/* cfg-3: bit-0:Auto read status register enable   */
	uint32_t cfg4;		/* cfg-4 & cfg-6 is for RGST bus setting */
	uint32_t cfg5;		/* cfg-5 & cfg-7 is for C-bus setting */
	uint32_t cfg6;		/* cfg-6 & cfg-4 is for RGST bus setting */
	uint32_t cfg7;		/* cfg-7 & cfg-5 is for C-bus setting */
	uint32_t cfg8;

	uint32_t cust_cmd_2;	/* The 2nd customized command,to enable data_64, byte_cnt for spi_cust_cmd ??? */
	uint32_t data_64;	/* to Read / to Write buffer,for 8388 is 4K byte size */
	uint32_t rbuf_addr;	/* spi read buffer address register */
	uint32_t status_2;	/* the 2nd status register */
	uint32_t status_3;
	uint32_t mode_status;
	uint32_t err_status;

	uint32_t threshold;	/* Issue interrupt after reading threshold data */
	uint32_t finish;	/* Issue interrupt after transmission complete */
	uint32_t isr;		/* interrupt information and clear */
	uint32_t wbuf_addr;	/* spi write buffer address register */

};

// ctrl
#define SPI_DEVICE_IDLE	BIT(15)

// wait
#define SPI_NAND_ENABLE	BIT(11)

// spi_cust_cmd
#define SPI_CUSTCMD_SHIFT		8
#define SPI_CUSTCMD_FN_SHIFT		7
#define SPI_CUSTCMD_RW_SHIFT		5
#define CUSTCMD_BYTECNT_DATA_SHIFT	2
#define CUSTCMD_BYTECNT_ADDR_SHIFT	0

#define SPI_CUSTCMD_ENABLE		(1 << SPI_CUSTCMD_FN_SHIFT)
#define SPI_CUSTCMD_READ		(0 << SPI_CUSTCMD_RW_SHIFT)
#define SPI_CUSTCMD_WRITE		(1 << SPI_CUSTCMD_RW_SHIFT)

#define SPINAND_CUSTCMD_NO_DATA		(0 << CUSTCMD_BYTECNT_DATA_SHIFT)
#define SPINAND_CUSTCMD_1_DATA		(1 << CUSTCMD_BYTECNT_DATA_SHIFT)
#define SPINAND_CUSTCMD_2_DATA		(2 << CUSTCMD_BYTECNT_DATA_SHIFT)
#define SPINAND_CUSTCMD_3_DATA		(3 << CUSTCMD_BYTECNT_DATA_SHIFT)
#define SPINAND_CUSTCMD_4_DATA		(4 << CUSTCMD_BYTECNT_DATA_SHIFT)

#define SPINAND_CUSTCMD_NO_ADDR		(0 << CUSTCMD_BYTECNT_ADDR_SHIFT)
#define SPINAND_CUSTCMD_1_ADDR		(1 << CUSTCMD_BYTECNT_ADDR_SHIFT)
#define SPINAND_CUSTCMD_2_ADDR		(2 << CUSTCMD_BYTECNT_ADDR_SHIFT)
#define SPINAND_CUSTCMD_3_ADDR		(3 << CUSTCMD_BYTECNT_ADDR_SHIFT)

// CMD list
#define SPINAND_CMD_GETFEATURES		0x0F
#define SPINAND_CMD_SETFEATURES		0x1F
#define SPINAND_CMD_BLKERASE		0xD8
#define SPINAND_CMD_PAGE2CACHE		0x13
#define SPINAND_CMD_RDCACHEQUADIO	0xEB

#define SPINAND_CMD_PROLOADx4		0x32
#define SPINAND_CMD_PROEXECUTE		0x10

// =============================
#define CSR_EN				BIT(0)
#define CSR_MODE_PIO			BIT(1)
#define CSR_MODE_DESC			0
#define CSR_MODE_PIO1			(BIT(1))
#define CSR_MODE_PIO2			(BIT(1) | BIT(2))
#define CSR_BURST4			0
#define CSR_BURST8			BIT(3)
#define CSR_DESC_FETCH			BIT(4)
#define CSR_NAND_EDO			BIT(5)
#define CSR_DUMMY_RD			BIT(6)
#define CSR_RESET			BIT(8)

#define ACTR_CLE(t)			(((t) >> 1) & 0xF)
#define ACTR_ALE(t)			(((t) & 0xF) << 4)
#define ACTR_ACT(t)			(((t) & 0xF) << 8)
#define ACTR_REC(t)			(((t) & 0xF) << 12)
#define ACTR_WAIT(t)			(((t) & 0xFF) << 16)
#define ACTR_RDST(t)			(((t) & 0xFF) << 24)

#define PIOCR0_CS(n)			(1 << ((n) & 3))
#define PIOCR0_CS_MASK			0x0F
#define PIOCR0_CLE			BIT(4)
#define PIOCR0_ALE			BIT(5)
#define PIOCR0_WE			BIT(6)
#define PIOCR0_RE			BIT(7)
#define PIOCR0_OE			BIT(8)
#define PIOCR0_WP			BIT(9)
#define PIOCR1_WR(n)			((n) & 0xFF)
#define PIOCR2_RD(n)			((n) & 0xFF)
#define PIOCR3_CMD(n)			((n) & 0xFF)
#define PIOCR4_ADDR_RAW(n)		(n)
#define PIOCR5_ADDR_COL(n)		((n) & 0xFFFF)
#define PIOCR5_ADDR_CYC(n)		((((n) - 1) & 7) << 16)
#define PIOCR6_DMA_ADDR(n)		(n)
#define PIOCR7_DMA_SIZE(n)		(((n) - 1) & 0xFFFF)
#define PIOCR7_DMA_RD			0
#define PIOCR7_DMA_WR			BIT(16)


#define ISR_BUS_BUSY			BIT(5)
#define ISR_FL_ERROR			BIT(6)  /* NAND flash reports error */
#define ISR_HW_BUSY			BIT(7)  /* NAND controller busy */
#define ISR_CMD				BIT(8)  /* CMD complete (PIO MODE2) */
#define ISR_ADDR			BIT(9)  /* ADDR complete (PIO MODE2) */
#define ISR_WR				BIT(10) /* Write complete (PIO MODE2) */
#define ISR_RD				BIT(11) /* Read complete (PIO MODE2) */
#define ISR_RB0				BIT(12)
#define ISR_RB1				BIT(13)
#define ISR_RB2				BIT(14)
#define ISR_RB3				BIT(15)
#define ISR_DESC_ADDR(x)		(((x) >> 16) & 0xFFFF)

#define RBTR_WB(t)			((t) & 0xFF)
#define RBTR_IRQ_INVDESC		BIT(8)

#endif /* __SP_SPINAND_H */
