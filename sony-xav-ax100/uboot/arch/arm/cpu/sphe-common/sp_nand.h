/*
 * (C) Copyright 2014
 * Sunplus Technology
 * Kuo-Jung Su <dante.su@sunplus.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __SP_NAND_H
#define __SP_NAND_H

struct sp_nand_regs {
	uint32_t csr;   /* Control status register */
	uint32_t dbar;  /* Descriptor base address register */
	uint32_t actr0; /* AC timing register 0 */
	uint32_t rber;  /* R/B enable register */

	uint32_t piocr[8];

	/* Toggle NAND support */
	uint32_t rsvd0[2];
	uint32_t tole_cnt_bit0;	/*bit0 tolerance cnt*/
	uint32_t tole_cnt;

	uint32_t ier;
	uint32_t isr;
	uint32_t rbcr;     /* R/B control register */
	uint32_t rsvd1;

	uint32_t rand_cr;  /* Randomizer control register */
	uint32_t prbs_cr;  /* PRBS coefficient control register */
	uint32_t seed_cr;  /* Seed control register */
	uint32_t pren[8];  /* Predefined number */

	uint32_t pio_page; /* PIO1 page address */
	uint32_t pio_seed; /* PIO1 load seed trigger */
	uint32_t smc_wc;   /* Super manual command wait control */
	uint32_t rdst;     /* Status of repeat STS_RD command */
	uint32_t actr1;    /* AC timing register 1 */
};

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
#define CSR_EDO_TYPE_SET(n) ((n&0x3)<<9)

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

struct sp_nand_desc {
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

extern const struct nand_flash_dev sp_nand_ids[];

/////////////////////

// FIXME:something wrong in nand dump,nand read command, #define SUPPORT_DOUBLE_BUFFER_PREFETCH
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
enum Working_PingPong_Buf {
	USE_PP_BUF_0 = 0,
	USE_PP_BUF_1,
};

enum PingPong_Buffer_Status {
	PP_BUF_IDLE = 0,
	PP_BUF_CMD_SEND,
	PP_BUF_DATA_RDY,
	PP_BUF_DECODE_RDY,
	PP_BUF_DMA_RDY,
};
#endif

struct sp_nand_info {
        void __iomem *regs;
        struct mtd_info *mtd;
        struct nand_chip nand;

        /* descriptor pool */
        struct {
                uint32_t size;
                void *virt;
                dma_addr_t phys;
        } desc;

        /*
         * local buffer for NAND <--> SRAM DMA operation
         *
         * There is a hardware limitation in letb123:
         * The NAND & BCH controller are attached to CBUS
         * which could not access DRAM modules, and thus
         * zero-copy is not possible.
         */
#ifdef SUPPORT_DOUBLE_BUFFER_PREFETCH
        struct {
                uint32_t idx;
                uint32_t size;
                void *virt;
                dma_addr_t phys;
        } buff0;

        struct {
                uint32_t idx;
                uint32_t size;
                void *virt;
                dma_addr_t phys;
        } buff1;

        int nand_working_buff_flag;
        int prefetch_row;
#else
        struct {
                uint32_t idx;
                uint32_t size;
                void *virt;
                dma_addr_t phys;
        } buff;
#ifdef CONFIG_SP_NAND_PING_PONG_BUFFER
        int buff_flag;  /* buf-0 or buf-1 for upload data to HAL(MTD & FTL) layer */
        int buff_page;
        int buff_state;	/* 0:idle; 1:cmd_send; 2;data_rdy; 3:decode_rdy; 4:dma_rdy */
        struct {
                uint32_t idx;
                uint32_t size;
                void *virt;
                dma_addr_t phys;
        } buff1;
        int buff1_page;
        int buff1_state;  /* 0:idle; 1:cmd_send; 2;data_rdy; 3:decode_rdy; 4:dma_rdy */
#endif
#endif

        struct nand_ecclayout ecc_layout;
        int cmd; /* current command code */
        int cac; /* col address cycles */
        int rac; /* row address cycles */
        int cs;
#ifdef CONFIG_SP_ERASECNT_TABLE
        uint32_t *EraseCntTable;
#endif
};
#endif /* __SP_NAND_H */
