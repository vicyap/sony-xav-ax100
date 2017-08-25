#ifndef __INC_REGMAP_letb123_H
#define __INC_REGMAP_letb123_H

struct moon0_regs {
	unsigned int stamp;
	unsigned int g0_reserved_0[3];
	unsigned int clk_sel[2];
	unsigned int pllsys_cfg_sel;
	unsigned int g0_reserved_1;
	unsigned int clken[4];
	unsigned int gclken[4];
#define RESET0_UA0   BIT(24)
	unsigned int reset[4];
	unsigned int g0_reserved_2[8];
	unsigned int hw_cfg;
	unsigned int g0_reserved_3[3];
};
#define MOON0_REG ((volatile struct moon0_regs *)RF_GRP(0, 0))

struct moon1_regs {
	unsigned int sft_cfg[32];
};
#define MOON1_REG ((volatile struct moon1_regs *)RF_GRP(1, 0))

struct pad_ctl_regs {
	unsigned int pad_ctrl[25];
	unsigned int gpio_first[7];
};
#define PAD_CTL_REG ((volatile struct pad_ctl_regs *)RF_GRP(4, 0))

struct hb_gp_regs {
	unsigned int hb_otp_data0;
	unsigned int hb_otp_data1;
	unsigned int hb_otp_data2;
	unsigned int hb_otp_data3;
	unsigned int hb_otp_data4;
	unsigned int hb_otp_data5;
	unsigned int hb_otp_data6;
	unsigned int hb_otp_data7;
	unsigned int hb_otp_ctl;
	unsigned int hb_otp_data;
	unsigned int g7_reserved[22];
};
#define HB_GP_REG ((volatile struct hb_gp_regs *)RF_GRP(7, 0))

struct iop_regs {
	// Group 8 : IOP
	unsigned int iop_control;
	unsigned int g8_reserved_0;
	unsigned int iop_bp;
	unsigned int iop_regsel;
	unsigned int iop_regout;
	unsigned int iop_memlimit;
	unsigned int g8_reserved_1[2];
	unsigned int iop_data[12];
	unsigned int iop_base_adr_l;
	unsigned int iop_base_adr_h;
	unsigned int memory_bridge_control;
	unsigned int iop_remap_adr_l;
	unsigned int iop_remap_adr_h;
	unsigned int g8_reserved_2[7];
};
#define IOP_REG ((volatile struct iop_regs *)RF_GRP(8, 0))

struct uart_regs {
        unsigned int dr;  /* data register */
        unsigned int lsr; /* line status register */
        unsigned int msr; /* modem status register */
        unsigned int lcr; /* line control register */
        unsigned int mcr; /* modem control register */
        unsigned int div_l;
        unsigned int div_h;
        unsigned int isc; /* interrupt status/control */
        unsigned int txr; /* tx residue */
        unsigned int rxr; /* rx residue */
        unsigned int thr; /* rx threshold */
};
#define UART0_REG    ((volatile struct uart_regs *)RF_GRP(18, 0))
#define UART1_REG    ((volatile struct uart_regs *)RF_GRP(19, 10))
#define UART2_REG    ((volatile struct uart_regs *)RF_GRP(16, 10))
#define UART3_REG    ((volatile struct uart_regs *)RF_GRP(17, 0))
#define UART4_REG    ((volatile struct uart_regs *)RF_GRP(271, 0))
#define UART5_REG    ((volatile struct uart_regs *)RF_GRP(272, 0))
#define UART6_REG    ((volatile struct uart_regs *)RF_GRP(273, 0))

struct stc_regs {
	unsigned int stc_15_0;
	unsigned int stc_31_16;
	unsigned int stc_64;
	unsigned int stc_divisor;
	unsigned int rtc_15_0;
	unsigned int rtc_23_16;
	unsigned int rtc_divisor;
	unsigned int stc_config;
	unsigned int timer0_ctrl;
	unsigned int timer0_cnt;
	unsigned int timer1_ctrl;
	unsigned int timer1_cnt;
	unsigned int timerw_ctrl;
	unsigned int timerw_cnt;
	unsigned int g12_reserved_0[2];
        unsigned int timer2_ctl;     // 12.16
        unsigned int timer2_pres_val;// 12.17
        unsigned int timer2_reload;  // 12.18
        unsigned int timer2_cnt;     // 12.19
	unsigned int timer3_ctrl;
	unsigned int timer3_divisor;
	unsigned int timer3_reload;
	unsigned int timer3_cnt;
	unsigned int stcl_0;
	unsigned int stcl_1;
	unsigned int stcl_2;
	unsigned int atc_0;
	unsigned int atc_1;
	unsigned int atc_2;
};
#define STC_REG     ((volatile struct stc_regs *)RF_GRP(12, 0))

struct dpll_regs {
	unsigned int dpll0_ctrl;
	unsigned int dpll0_remainder;
	unsigned int dpll0_denominator;
	unsigned int dpll0_divider;
	unsigned int g20_reserved_0[4];
	unsigned int dpll1_ctrl;
	unsigned int dpll1_remainder;
	unsigned int dpll1_denominator;
	unsigned int dpll1_divider;
	unsigned int g20_reserved_1[4];
	unsigned int dpll2_ctrl;
	unsigned int dpll2_remainder;
	unsigned int dpll2_denominator;
	unsigned int dpll2_divider;
	unsigned int g20_reserved_2[4];
	unsigned int dpll3_ctrl;
	unsigned int dpll3_remainder;
	unsigned int dpll3_denominator;
	unsigned int dpll3_divider;
	unsigned int dpll3_sprd_num;
	unsigned int g20_reserved_3[3];
};
#define DPLL_REG     ((volatile struct dpll_regs *)RF_GRP(20, 0))

struct spi_ctrl_regs {
	unsigned int spi_ctrl;
	unsigned int spi_wait;
	unsigned int spi_cust_cmd;
	unsigned int spi_addr_l;
	unsigned int spi_addr_h;
	unsigned int spi_data_l;
	unsigned int spi_data_h;
	unsigned int spi_status;
	unsigned int spi_cfg[9];
	unsigned int spi_cust_cmd_2;
	unsigned int spi_data_64;
	unsigned int spi_buf_addr;
	unsigned int spi_status_2;
	unsigned int spi_status_3;
	unsigned int spi_mode_status;
	unsigned int spi_err_status;
};
#define SPI_CTRL_REG ((volatile struct spi_ctrl_regs *)RF_GRP(22, 0))

struct card_ctl_regs {
        unsigned int card_mediatype;     // 0
        unsigned int reserved;           // 1
        unsigned int card_cpu_page_cnt;  // 2
        unsigned int card_ctl_page_cnt;  // 3
        unsigned int sdram_sector0_sz;   // 4
        unsigned int ring_buf_on;        // 5
        unsigned int card_gclk_disable;  // 6
        unsigned int sdram_sector1_addr; // 7
        unsigned int sdram_sector1_sz;   // 8
        unsigned int sdram_sector2_addr; // 9
        unsigned int sdram_sector2_sz;   // 10
        unsigned int sdram_sector3_addr; // 11
        unsigned int sdram_sector3_sz;   // 12
        unsigned int sdram_sector4_addr; // 13
        unsigned int sdram_sector4_sz;   // 14
        unsigned int sdram_sector5_addr; // 15
        unsigned int sdram_sector5_sz;   // 16
        unsigned int sdram_sector6_addr; // 17
        unsigned int sdram_sector6_sz;   // 18
        unsigned int sdram_sector7_addr; // 19
        unsigned int sdram_sector7_sz;   // 20
        unsigned int sdram_sector_cnt;   // 21
        unsigned int reserved2[10];      // 22
};
#define CARD0_CTL_REG ((volatile struct card_ctl_regs *)RF_GRP(118, 0))
#define CARD1_CTL_REG ((volatile struct card_ctl_regs *)RF_GRP(125, 0))

struct card_sd_regs {
        unsigned int reserved[16];       // 0
        unsigned int sd_rst;             // 16
        unsigned int sd_config;          // 17
        unsigned int sd_ctrl;            // 18
        unsigned int sd_status;          // 19
        unsigned int sd_state;           // 20
        unsigned int sd_blocksize;       // 21
        unsigned int sd_hwdma_config;    // 22
        unsigned int sd_timing_config0;  // 23
        unsigned int sd_timing_config1;  // 24
        unsigned int sd_piodatatx;       // 25
        unsigned int sd_piodatarx;       // 26
        unsigned int sd_cmdbuf[5];       // 27
};
#define CARD0_SD_REG ((volatile struct card_sd_regs *)RF_GRP(119, 0))

struct card_sdms_regs {
        unsigned int sd_rspbuf[6];       // 0
        unsigned int sd_crc7buf;         // 6
        unsigned int sd_crc16buf0;       // 7
        unsigned int sd_hw_state;        // 8
        unsigned int sd_crc16buf1;       // 9
        unsigned int sd_hw_cmd13_rca;    // 10
        unsigned int sd_crc16buf2;       // 11
        unsigned int sd_tx_dummy_num;    // 12
        unsigned int sd_crc16buf3;       // 13
        unsigned int sd_clk_dly;         // 14
        unsigned int sd_crc16flag;       // 15
        unsigned int ms_piodmarst;       // 16
        unsigned int ms_cmd;             // 17
        unsigned int reserved18;         // 18
        unsigned int ms_hw_state;        // 19
        unsigned int ms_modespeed;       // 20
        unsigned int ms_timeout;         // 21
        unsigned int ms_state;           // 22
        unsigned int ms_status;          // 23
        unsigned int ms_rddata[4];       // 24
        unsigned int ms_crcbuf[2];       // 28
        unsigned int ms_crc_error;       // 30
        unsigned int ms_piordy;          // 31
};
#define CARD0_SDMS_REG ((volatile struct card_sdms_regs *)RF_GRP(120, 0))

struct card_ms_regs {
        unsigned int ms_wd_data[16];     // 0
        unsigned int reserved16[16];     // 16
};
#define CARD0_MS_REG ((volatile struct card_ms_regs *)RF_GRP(121, 0))

struct card_dma_regs {
        unsigned int dma_data;             // 0
        unsigned int dma_srcdst;           // 1
        unsigned int dma_size;             // 2
        unsigned int dma_hw_stop_rst;      // 3
        unsigned int dma_ctrl;             // 4
        unsigned int dma_base_addr[2];     // 5
        unsigned int dma_hw_en;            // 7
        unsigned int dma_hw_page_addr0[2]; // 8
        unsigned int dma_hw_page_addr1[2]; // 10
        unsigned int dma_hw_page_addr2[2]; // 12
        unsigned int dma_hw_page_addr3[2]; // 14
        unsigned int dma_hw_page_num[4];   // 16
        unsigned int dma_hw_block_num;     // 20
        unsigned int dma_start;            // 21
        unsigned int dma_hw_page_cnt;      // 22
        unsigned int dma_cmp;              // 23
        unsigned int dma_int_en;           // 24
        unsigned int reserved25;           // 25
        unsigned int dma_hw_wait_num[2];   // 26
        unsigned int dma_hw_delay_num;     // 28
        unsigned int reserved29[3];        // 29
};
#define CARD0_DMA_REG ((volatile struct card_dma_regs *)RF_GRP(122, 0))

struct ehci_regs {
	unsigned int ehci_len_rev;
	unsigned int ehci_sparams;
	unsigned int ehci_cparams;
	unsigned int ehci_portroute;
	unsigned int g143_reserved_0[4];
	unsigned int ehci_usbcmd;
	unsigned int ehci_usbsts;
	unsigned int ehci_usbintr;
	unsigned int ehci_frameidx;
	unsigned int ehci_ctrl_ds_segment;
	unsigned int ehci_prd_listbase;
	unsigned int ehci_async_listaddr;
	unsigned int g143_reserved_1[9];
	unsigned int ehci_config;
	unsigned int ehci_portsc;
	unsigned int g143_reserved_2[1];
	unsigned int ehci_version_ctrl;
	unsigned int ehci_general_ctrl;
	unsigned int ehci_usb_debug;
	unsigned int ehci_sys_debug;
	unsigned int ehci_sleep_cnt;
};
#define EHCI0_REG ((volatile struct ehci_regs *)RF_GRP(143, 0))
#define EHCI1_REG ((volatile struct ehci_regs *)RF_GRP(145, 0))
#define EHCI2_REG ((volatile struct ehci_regs *)RF_GRP(147, 0))

/* NAND */
#define NAND_S330_BASE           0x9C82B000
#define BCH_S338_BASE_ADDRESS    0x9C82C000

/* SPI NAND */
#define CONFIG_SP_SPINAND_BASE   0

#endif /* __INC_REGMAP_letb123_H */
