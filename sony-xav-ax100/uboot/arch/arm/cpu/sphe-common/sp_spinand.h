/*
 * (C) Copyright 2015
 * Sunplus Technology
 * Mason Yang <mason.yang@sunplus.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __SP_SPINAND_H
#define __SP_SPINAND_H

struct sp_spinand_regs {

	uint32_t ctrl;   /* SPI NAND Control register, Group 87 */
	uint32_t wait;   /* bit 11 for enable SPI NAND function, setup,hold and disactive cycles */
	uint32_t cust_cmd; /* customized command setup & enable, write-to/read-from flash, address & data byte count */

	uint32_t addr_low;  /* spi address low register,bit15~0 */
	uint32_t addr_high;  /* spi address low register,bit31~16 */
	uint32_t data_low;	 /* spi data low register,bit15~0 */
	uint32_t data_high;  /* spi data high register,bit31~16 */

	uint32_t status;	   /*  spi nand device status; status feed back from spi device */

	/*  spi mode1/mode3, 1,2,4 bit data, customer defined command,number of dummy cycle...  */     
	uint32_t cfg0;   
	uint32_t cfg1;  /* cfg-1:user defined read command & enable,i.,. read cache quad IO */
	uint32_t cfg2;  /* cfg-2:user defined write command & enable */
	uint32_t cfg3;  /* cfg-3: bit-0:Auto read status register enable   */ 

	uint32_t cfg4;  /* cfg-4 & cfg-6 is for RGST bus setting */
	uint32_t cfg5;  /* cfg-5 & cfg-7 is for C-bus setting */
	uint32_t cfg6;	/* cfg-6 & cfg-4 is for RGST bus setting */
	uint32_t cfg7;  /* cfg-7 & cfg-5 is for C-bus setting */

	uint32_t cfg8;

	uint32_t cust_cmd_2;  /* The 2nd customized command,to enable data_64, byte_cnt for spi_cust_cmd ??? */
	uint32_t data_64;     /* to Read / to Write buffer,for 8388 is 4K byte size */
	uint32_t rbuf_addr;   /* spi read buffer address register */
	uint32_t status_2;    /* the 2nd status register */
	uint32_t status_3;
	uint32_t mode_status;
	uint32_t err_status;
	/**** new add for spi nand ctrl  *****/ 
	uint32_t threshold; /* Issue interrupt after reading threshold data */
	uint32_t finish;    /* Issue interrupt after transmission complete */
	uint32_t isr;       /* interrupt information and clear */

	uint32_t wbuf_addr; /* spi write buffer address register */

};


// ctrl
#define SPI_DEVICE_IDLE  BIT(15)


// wait
#define SPI_NAND_ENABLE  BIT(11)


// spi_cust_cmd
#define SPI_CUSTCMD_SHIFT          8
#define SPI_CUSTCMD_FN_SHIFT       7 
#define SPI_CUSTCMD_RW_SHIFT       5 
#define CUSTCMD_BYTECNT_DATA_SHIFT 2
#define CUSTCMD_BYTECNT_ADDR_SHIFT 0

#define SPI_CUSTCMD_ENABLE       (1<<SPI_CUSTCMD_FN_SHIFT)
#define SPI_CUSTCMD_READ         (0<<SPI_CUSTCMD_RW_SHIFT) 
#define SPI_CUSTCMD_WRITE        (1<<SPI_CUSTCMD_RW_SHIFT)

#define SPINAND_CUSTCMD_NO_DATA	(0<<CUSTCMD_BYTECNT_DATA_SHIFT)
#define SPINAND_CUSTCMD_1_DATA	(1<<CUSTCMD_BYTECNT_DATA_SHIFT)
#define SPINAND_CUSTCMD_2_DATA	(2<<CUSTCMD_BYTECNT_DATA_SHIFT)
#define SPINAND_CUSTCMD_3_DATA	(3<<CUSTCMD_BYTECNT_DATA_SHIFT)
#define SPINAND_CUSTCMD_4_DATA	(4<<CUSTCMD_BYTECNT_DATA_SHIFT)

#define SPINAND_CUSTCMD_NO_ADDR	(0<<CUSTCMD_BYTECNT_ADDR_SHIFT)
#define SPINAND_CUSTCMD_1_ADDR	(1<<CUSTCMD_BYTECNT_ADDR_SHIFT)
#define SPINAND_CUSTCMD_2_ADDR	(2<<CUSTCMD_BYTECNT_ADDR_SHIFT)
#define SPINAND_CUSTCMD_3_ADDR	(3<<CUSTCMD_BYTECNT_ADDR_SHIFT)


// CMD list
#define SPINAND_CMD_GETFEATURES   0x0F
#define SPINAND_CMD_SETFEATURES   0x1F
#define SPINAND_CMD_BLKERASE      0xD8
#define SPINAND_CMD_PAGE2CACHE    0x13
#define SPINAND_CMD_RDCACHEQUADIO 0xEB

#define SPINAND_CMD_PROLOADx4     0x32
#define SPINAND_CMD_PROEXECUTE    0x10



// =============================
#define CSR_EN			BIT(0)
#define CSR_MODE_PIO	BIT(1)
#define CSR_MODE_DESC	0
#define CSR_MODE_PIO1	(BIT(1))
#define CSR_MODE_PIO2	(BIT(1) | BIT(2))
#define CSR_BURST4		0
#define CSR_BURST8		BIT(3)
#define CSR_DESC_FETCH	BIT(4)
#define CSR_NAND_EDO	BIT(5)
#define CSR_DUMMY_RD	BIT(6)
#define CSR_RESET		BIT(8)
#define EDO_TYPE01		BIT(9) // Mason for 8388
#define EDO_TYPE10		BIT(10)
#define EDO_TYPE11		(BIT(9) | BIT(10))

#define ACTR_CLE(t)		(((t) >> 1) & 0xf)
#define ACTR_ALE(t)		(((t) & 0xf) << 4)
#define ACTR_ACT(t)		(((t) & 0xf) << 8)
#define ACTR_REC(t)		(((t) & 0xf) << 12)
#define ACTR_WAIT(t)	(((t) & 0xff) << 16)
#define ACTR_RDST(t)	(((t) & 0xff) << 24)

#define PIOCR0_CS(n)		(1 << ((n) & 3))
#define PIOCR0_CS_MASK		0x0f
#define PIOCR0_CLE			BIT(4)
#define PIOCR0_ALE			BIT(5)
#define PIOCR0_WE			BIT(6)
#define PIOCR0_RE			BIT(7)
#define PIOCR0_OE			BIT(8)
#define PIOCR0_WP			BIT(9)
#define PIOCR1_WR(n)		((n) & 0xff)
#define PIOCR2_RD(n)		((n) & 0xff)
#define PIOCR3_CMD(n)		((n) & 0xff)
#define PIOCR4_ADDR_RAW(n)	(n)
#define PIOCR5_ADDR_COL(n)	((n) & 0xffff)
#define PIOCR5_ADDR_CYC(n)	((((n) - 1) & 7) << 16)
#define PIOCR6_DMA_ADDR(n)	(n)
#define PIOCR7_DMA_SIZE(n)	(((n) - 1) & 0xffff)
#define PIOCR7_DMA_RD		0
#define PIOCR7_DMA_WR		BIT(16)

#define ISR_DESC_DONE		BIT(0)
#define ISR_DESC_END		BIT(1)
#define ISR_DESC_ERR		BIT(2)
#define ISR_DESC_INV		BIT(3)
#define ISR_DESC_INVCMD		BIT(4)
#define ISR_DESC			0x2 //0x1f
#define ISR_BUS_BUSY		BIT(5)
#define ISR_FL_ERROR		BIT(6)  /* NAND flash reports error */
#define ISR_HW_BUSY			BIT(7)  /* NAND controller busy */
#define ISR_CMD				BIT(8)  /* CMD complete (PIO MODE2) */
#define ISR_ADDR			BIT(9)  /* ADDR complete (PIO MODE2) */
#define ISR_WR				BIT(10) /* Write complete (PIO MODE2) */
#define ISR_RD				BIT(11) /* Read complete (PIO MODE2) */
#define ISR_RB0				BIT(12)
#define ISR_RB1				BIT(13)
#define ISR_RB2				BIT(14)
#define ISR_RB3				BIT(15)
#define ISR_DESC_ADDR(x)	(((x) >> 16) & 0xffff)

#define RBTR_WB(t)			((t) & 0xff)
#define RBTR_IRQ_INVDESC	BIT(8)

/*
 * Descriptor Mode
 */

struct sp_spinand_desc {
	uint32_t cmd1:8;
	uint32_t cmd0:8;
	uint32_t ctrl:8;
	uint32_t mfs:4;
	uint32_t cmd:4; /* i.e. SP_NAND_CMD_XXX */

	uint32_t oob_ssz:16; /* oob sector size */
	uint32_t buf_ssz:16; /* data sector size */

	uint32_t isr:16;
	uint32_t ier:16;

	uint32_t addr4:8;
	uint32_t addr5:8;
	uint32_t sect_nr:7; /* sector number */
	uint32_t type:1;
	uint32_t addr_len:3;
	uint32_t ff_check:1;
	uint32_t oob_en:1;
	uint32_t cs_ctrl:1;
	uint32_t end:1;
	uint32_t owner:1;

	uint32_t addr0:8;
	uint32_t addr1:8;
	uint32_t addr2:8;
	uint32_t addr3:8;

	uint32_t buf_dma:32;

	uint32_t oob_dma:32;

	uint32_t desc_sz:14;  /* descriptor size */
	uint32_t rand_en:1;   /* enable scrambler/randomizer */
	uint32_t ac_timing_select:1;
	uint32_t rsvd:16;
} __attribute__ ((__packed__));

#define SP_NAND_CMD_READ	0x1
#define SP_NAND_CMD_WRITE	0x2  /* page write */
#define SP_NAND_CMD_ERASE	0x3  /* block erase */
#define SP_NAND_CMD_RDST	0x4  /* read status */
#define SP_NAND_CMD_SUPER	0xd  /* super manual */
#define SP_NAND_CMD_CACHE_RD 0xe /* cache read */
extern const struct nand_flash_dev sp_nand_ids[];

////////////////////

struct sp_spinand_info {
        void __iomem *regs;
        struct mtd_info *mtd;
        struct nand_chip nand;

#if USE_DESCRIPTOR_MODE
        /* descriptor pool */
        struct {
                uint32_t size;
                void *virt;
                dma_addr_t phys;
        } desc;
#endif
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
        } buff;


        struct nand_ecclayout ecc_layout;
        int cmd; /* current command code */
        int cac; /* col address cycles */
        int rac; /* row address cycles */
        int cs;
        int id;  // Vendor ID for 6B/EB Winbond or GigaDevice
        int row; // row address for pagewrite
};

#endif /* __SP_SPINAND_H */
