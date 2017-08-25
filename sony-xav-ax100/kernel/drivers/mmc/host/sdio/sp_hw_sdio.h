#ifndef __SP_HW_SDIO_H__
#define __SP_HW_SDIO_H__
#include <linux/types.h>

#define IO0_BASE			(0xf0800000)    // VA of IO 	/* 0x9C000000 --> 0xFF000000 */
#define 	RGST_OFFSET     IO0_BASE

#ifndef NULL
#define NULL	((void *)0)
#endif

#define SP_SD_SUCCESS			0x00000000
#define SP_SD_FAIL				0x00000001
#define SP_SD_RSP_TIMEOUT		0x00000002
#define SP_SD_CRC_ERROR			0x00000004
#define SP_SD_COUNTER_ERROR		0x00000008
#define SP_SD_DMA_FAIL			0x00000010
#define SP_SD_TIMEOUT			0x00000020
#define SP_SD_CRC_TIMEOUT		0x00000040
#define SP_SD_COMPLETE_INIT		0x80000000

#define OS_TICK					1

enum {
	MEDIA_NONE = 0,
	MEDIA_SMC  = 1,
	MEDIA_RESERVED1 = 2,
	MEDIA_CF   = 3,
	MEDIA_SPI  = 4,
	MEDIA_RESERVED2 = 5,
	MEDIA_SD   = 6,
	MEDIA_MS   = 7,
};

enum SD_HANDLE_STATE {
	SD_WRITE_HANDLE_IDLE = 0,
	SD_WAIT_TRAN_STATE0,
	SD_SEND_WRITE_CMD,
	SD_WRITE_DATA,
	SD_SEND_END_CMD,
	SD_WAIT_TRAN_STATE1,
	SD_WAIT_READY,
};
#define SDIO_MODE               0
#define MMC_MODE     			1
#define SD_MODE      			2

#define SD_1BIT_MODE 			0
#define SD_4BIT_MODE 			1


#define RSP_TYPE_NORSP          0xff
#define RSP_TYPE_R1  			0
#define RSP_TYPE_R1B 			0
#define RSP_TYPE_R2  			1
#define RSP_TYPE_R3  			2
#define RSP_TYPE_R6  			3
#define RSP_TYPE_R7	 			4
#define RSP_ECHO_BYTE			0xaa
#define SD_STATE_IDLE    		0
#define SD_STATE_READY 			1
#define SD_STATE_IDENT   		2
#define SD_STATE_STBY    		3
#define SD_STATE_TRAN			4
#define SD_STATE_DATA			5
#define SD_STATE_RCV			6
#define SD_STATE_PRG			7
#define SD_STATE_DIS			8
#define SD_STATE_TIMEOUT 		9

#define DMA_DRAM      			1
#define DMA_FLASH     			2

#define SP_DMA_DST_SHIFT		4
#define SP_DMA_SRT_SHIFT		0

/**
 *	SD' Bus Width
 */
#define BUS_WIDTH_1BIT	0
#define BUS_WIDTH_4BIT 2
#define BUS_WIDTH_8BIT 4

#define SP_BUS_WIDTH_1BIT 0
#define SP_BUS_WIDTH_4BIT 1
#define SP_BUS_WIDTH_8BIT (1<<6)

#define CLOCK_200K	200000
#define CLOCK_400K	400000
#define CLOCK_50M	50000000
#define CLOCK_30M	30000000
#define CLOCK_25M	25000000
#define CLOCK_15M	15000000
#define CLOCK_5M	5000000

//SP_SD_DRIVESTRENGTH only take 2 bits
//current 0:7mA  1:9.3mA  2:11.6mA  3:14mA
#define SP_SD_DRIVESTRENGTH_0 0
#define SP_SD_DRIVESTRENGTH_1 1
#define SP_SD_DRIVESTRENGTH_2 2
#define SP_SD_DRIVESTRENGTH_3 3

/********************************************************************************
 *               pad voltage setting
 *********************************************************************************/
#define	pad_voltage_33v() 	do{ \
	regs0->pad_ctrl[14] &=  ~0x00000001;\
}while(0)

#define	pad_voltage_18v() 	do{ \
	regs0->pad_ctrl[14] |=  0x00000001;\
}while(0)

/********************************************************************************
 *               DPLL0_CLK setting
 *			200 MHz, 169MHz ~ 243MHz
 *********************************************************************************/
#define PLLH_SET() 			do{ \
	regs0->sft_cfg24   =    0x000022D8; \
	regs0->sft_cfg24  |=   (1<<26);\
	recgs0->sft_cfg24  |=   (1<<8  );\
	regs0->sft_cfg24  &= ~(1<<7 );\
}while(0)

#define DPLL0_SET_159MHz() 	do{ \
	regs0->dpll0_div = 0x1; \
	regs0->dpll0_remain  = 0x8C; \
	regs0->dpll0_denomi  = 0xC8; \
}while(0)

#define DPLL0_SET_200MHz() 	do{ \
	regs0->dpll0_div = 0x1; \
	regs0->dpll0_remain  = 0x8C;\
	regs0->dpll0_denomi  = 0x190;\
}while(0)

#define DPLL0_ENABLE() 		do{ \
	regs0->dpll0_ctrl  =  0x3; \
}while(0)



/********************************************************************************
 *                pin mux setting : A_MX65 = DPLL0_CLK
 *********************************************************************************/
#define dpll0_clk_pin()	do{ \
	regs0->sft_cfg3 = 0x00000080; \
}while(0)


/********************************************************************************
 *                          SD Pin MUX
 *********************************************************************************/
#define sd_pin(N)		do{ \
	regs0->sft_cfg2 &=  ~( 3<<0 ); \
	regs0->sft_cfg2 |=    ( N<<0 ); \
}while(0)
/******************************************************************************
 *                          MACRO Function Define
 *******************************************************************************/
#define CURRENT_USED_CARD_CTRL  4	/* 0, 1, 2, 3, 4*/  

#if (CURRENT_USED_CARD_CTRL==0)
#define DEV_SDCTRL_BASE_ADRS	((RGST_OFFSET+ 118*32*4)) 
#define letb8388_IRQ_CARD_CTL0_INT  (20 + 32) 
#define  MMC_MODE_CHIOCE  SD_MODE //sd mode

#elif (CURRENT_USED_CARD_CTRL==1)
#define DEV_SDCTRL_BASE_ADRS	((RGST_OFFSET+ 125*32*4)) 
#define letb8388_IRQ_CARD_CTL0_INT  (21 + 32) 
#define  MMC_MODE_CHIOCE  SD_MODE //sd mode

#elif (CURRENT_USED_CARD_CTRL==4)
#define DEV_SDCTRL_BASE_ADRS	((RGST_OFFSET+ 264*32*4)) 
#define letb8388_IRQ_CARD_CTL0_INT  (132 + 32)
#define  MMC_MODE_CHIOCE  SDIO_MODE //sdio mode
#endif


#define card_mediatype(adrs)				(*((volatile unsigned int *)(adrs+ 0*32*4+ 0*4)))
#define card_cpu_page_cnt(adrs)				(*((volatile unsigned int *)(adrs+ 0*32*4+ 2*4)))
#define card_fm_page_cnt(adrs)				(*((volatile unsigned int *)(adrs+ 0*32*4+ 3*4)))
#define card_ring_page_limit(adrs)			(*((volatile unsigned int *)(adrs+ 0*32*4+ 4*4)))

#define sd_vol_control(adrs)				(*((volatile unsigned int *)(adrs+ 1*32*4+11*4)))
#define sd_int_control(adrs)				(*((volatile unsigned int *)(adrs+ 1*32*4+12*4)))
#define sd_page_num(adrs)					(*((volatile unsigned int *)(adrs+ 1*32*4+13*4)))
#define sd_config0(adrs)					(*((volatile unsigned int *)(adrs+ 1*32*4+14*4)))
#define sdio_control(adrs)					(*((volatile unsigned int *)(adrs+ 1*32*4+15*4)))

#define sd_rst(adrs)						(*((volatile unsigned int *)(adrs+ 1*32*4+16*4)))
#define sd_config(adrs)						(*((volatile unsigned int *)(adrs+ 1*32*4+17*4)))
#define sd_ctrl(adrs)						(*((volatile unsigned int *)(adrs+ 1*32*4+18*4)))
#define sd_status0(adrs)					(*((volatile unsigned int *)(adrs+ 1*32*4+19*4)))
#define sd_status1(adrs)					(*((volatile unsigned int *)(adrs+ 1*32*4+20*4)))
#define sd_blocksize(adrs)					(*((volatile unsigned int *)(adrs+ 1*32*4+21*4)))
#define sd_hw_config(adrs)					(*((volatile unsigned int *)(adrs+ 1*32*4+22*4)))
#define sd_timing_config0(adrs)				(*((volatile unsigned int *)(adrs+ 1*32*4+23*4)))
#define sd_timing_config1(adrs)				(*((volatile unsigned int *)(adrs+ 1*32*4+24*4)))
#define sd_piodatatx(adrs)					(*((volatile unsigned int *)(adrs+ 1*32*4+25*4)))
#define sd_piodatarx(adrs)					(*((volatile unsigned int *)(adrs+ 1*32*4+26*4)))
#define sd_cmdbuf0(adrs)					(*((volatile unsigned int *)(adrs+ 1*32*4+27*4)))
#define sd_cmdbuf1(adrs)					(*((volatile unsigned int *)(adrs+ 1*32*4+28*4)))
#define sd_cmdbuf2(adrs)					(*((volatile unsigned int *)(adrs+ 1*32*4+29*4)))
#define sd_cmdbuf3(adrs)					(*((volatile unsigned int *)(adrs+ 1*32*4+30*4)))
#define sd_cmdbuf4(adrs)					(*((volatile unsigned int *)(adrs+ 1*32*4+31*4)))

#define sd_rspbuf0_3(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+ 0*4)))
#define sd_rspbuf4_5(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+ 1*4)))

#define sd_crc7buf(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+ 6*4)))
#define sd_crc16buf0lsb(adrs)				(*((volatile unsigned int *)(adrs+ 2*32*4+ 7*4)))
#define sd_hw_state(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+ 8*4)))
#define sd_crc16buf1lsb(adrs)				(*((volatile unsigned int *)(adrs+ 2*32*4+ 9*4)))
#define sd_hw_cmd13_rca(adrs)				(*((volatile unsigned int *)(adrs+ 2*32*4+10*4)))
#define sd_crc16buf2lsb(adrs)				(*((volatile unsigned int *)(adrs+ 2*32*4+11*4)))
#define sd_tx_dummy_num(adrs)				(*((volatile unsigned int *)(adrs+ 2*32*4+12*4)))
#define sd_crc16buf3lsb(adrs)				(*((volatile unsigned int *)(adrs+ 2*32*4+13*4)))
#define sd_clk_dly(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+14*4)))

#define ms_piodmarst(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+16*4)))
#define ms_cmd(adrs)						(*((volatile unsigned int *)(adrs+ 2*32*4+17*4)))
#define g120_reserved_1(adrs)				(*((volatile unsigned int *)(adrs+ 2*32*4+18*4)))
#define ms_hw_state(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+19*4)))
#define ms_modespeed(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+20*4)))
#define ms_timout(adrs)						(*((volatile unsigned int *)(adrs+ 2*32*4+21*4)))
#define ms_state1(adrs)						(*((volatile unsigned int *)(adrs+ 2*32*4+22*4)))
#define ms_state2(adrs)						(*((volatile unsigned int *)(adrs+ 2*32*4+23*4)))
#define ms_rddata0(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+24*4)))
#define ms_rddata1(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+25*4)))
#define ms_rddata2(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+26*4)))
#define ms_rddata3(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+27*4)))
#define ms_crcbuflsb(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+28*4)))
#define ms_crcbufmsb(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+29*4)))
#define ms_crcerror(adrs)					(*((volatile unsigned int *)(adrs+ 2*32*4+30*4)))
#define ms_piordy(adrs)						(*((volatile unsigned int *)(adrs+ 2*32*4+31*4)))

#define ms_wd_data0(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+ 0*4)))
#define ms_wd_data1(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+ 1*4)))
#define ms_wd_data2(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+ 2*4)))
#define ms_wd_data3(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+ 3*4)))
#define ms_wd_data4(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+ 4*4)))
#define ms_wd_data5(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+ 5*4)))
#define ms_wd_data6(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+ 6*4)))
#define ms_wd_data7(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+ 7*4)))
#define ms_wd_data8(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+ 8*4)))
#define ms_wd_data9(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+ 9*4)))
#define ms_wd_data10(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+10*4)))
#define ms_wd_data11(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+11*4)))
#define ms_wd_data12(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+12*4)))
#define ms_wd_data13(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+13*4)))
#define ms_wd_data14(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+14*4)))
#define ms_wd_data15(adrs)					(*((volatile unsigned int *)(adrs+ 3*32*4+15*4)))

#define dma_data(adrs)						(*((volatile unsigned int *)(adrs+ 4*32*4+ 0*4)))
#define dma_srcdst(adrs)					(*((volatile unsigned int *)(adrs+ 4*32*4+ 1*4)))
#define dma_size(adrs)						(*((volatile unsigned int *)(adrs+ 4*32*4+ 2*4)))
#define dma_hw_stop_rst(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+ 3*4)))
#define dma_ctrl(adrs)						(*((volatile unsigned int *)(adrs+ 4*32*4+ 4*4)))
#define dma_base_addrl(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+ 5*4)))
#define dma_base_addrh(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+ 6*4)))
#define dma_hw_en(adrs)						(*((volatile unsigned int *)(adrs+ 4*32*4+ 7*4)))
#define dma_hw_page_addrl(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+ 8*4)))
#define dma_hw_page_addrh(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+ 9*4)))
#define dma_hw_page_addrl1(adrs)			(*((volatile unsigned int *)(adrs+ 4*32*4+ 0*4)))
#define dma_hw_page_addrh1(adrs)			(*((volatile unsigned int *)(adrs+ 4*32*4+11*4)))
#define dma_hw_page_addrl2(adrs)			(*((volatile unsigned int *)(adrs+ 4*32*4+12*4)))
#define dma_hw_page_addrh2(adrs)			(*((volatile unsigned int *)(adrs+ 4*32*4+13*4)))
#define dma_hw_page_addrl3(adrs)			(*((volatile unsigned int *)(adrs+ 4*32*4+14*4)))
#define dma_hw_page_addrh3(adrs)			(*((volatile unsigned int *)(adrs+ 4*32*4+15*4)))
#define dma_hw_page_num0(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+16*4)))
#define dma_hw_page_num1(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+17*4)))
#define dma_hw_page_num2(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+18*4)))
#define dma_hw_page_num3(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+19*4)))
#define dma_hw_block_num(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+20*4)))
#define dma_start(adrs)						(*((volatile unsigned int *)(adrs+ 4*32*4+21*4)))
#define dma_hw_page_cnt(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+22*4)))
#define dma_cmp(adrs)						(*((volatile unsigned int *)(adrs+ 4*32*4+23*4)))
#define dma_cmpen(adrs)						(*((volatile unsigned int *)(adrs+ 4*32*4+24*4)))
#define g136_reserved_0(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+25*4)))
#define dma_hw_wait_numl(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+26*4)))
#define dma_hw_wait_numh(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+27*4)))
#define dma_hw_delay_num(adrs)				(*((volatile unsigned int *)(adrs+ 4*32*4+28*4)))


#define SET_HW_DMA_BASE_ADDR(DRAMAddr)  do{ \
	dma_base_addrl(DEV_SDCTRL_BASE_ADRS) = (UINT32) DRAMAddr & 0xffff; \
	dma_base_addrh(DEV_SDCTRL_BASE_ADRS) = ((UINT32)DRAMAddr >> 16) & 0xffff; \
}while(0)

#define DMA_BASE_ADDRL_GET()  dma_base_addrl(DEV_SDCTRL_BASE_ADRS)
#define DMA_BASE_ADDRH_GET()  dma_base_addrh(DEV_SDCTRL_BASE_ADRS)

#define SDIO_CONTROL_GET()  sdio_control(DEV_SDCTRL_BASE_ADRS)
#define SDIO_CONTROL_SET(x)  sdio_control(DEV_SDCTRL_BASE_ADRS) = (x)

#define SD_WAIT_RSP_TIME_SET(x)	\
	do{ \
		sd_timing_config0(DEV_SDCTRL_BASE_ADRS) = sd_timing_config0(DEV_SDCTRL_BASE_ADRS) & 0xF800; \
		sd_timing_config0(DEV_SDCTRL_BASE_ADRS) = sd_timing_config0(DEV_SDCTRL_BASE_ADRS) | ((x) & 0x7FF); \
	}while(0)

#define SD_WAIT_RSP_TIME_GET()	(sd_timing_config0(DEV_SDCTRL_BASE_ADRS) & 0x7FF)

#define SD_HIGHSPEED_EN_SET(x) \
	do{ \
		sd_timing_config0(DEV_SDCTRL_BASE_ADRS) = sd_timing_config0(DEV_SDCTRL_BASE_ADRS) & 0xF7FF; \
		sd_timing_config0(DEV_SDCTRL_BASE_ADRS) = sd_timing_config0(DEV_SDCTRL_BASE_ADRS) | (((x) & 0x1)<<11); \
	}while(0)

#define SD_HIGHSPEED_EN_GET() 	((sd_timing_config0(DEV_SDCTRL_BASE_ADRS) >> 11) & 1)

#define SD_WT_CLK_DELAY_TIME_SET(x)	\
	do{ \
		sd_timing_config0(DEV_SDCTRL_BASE_ADRS) = sd_timing_config0(DEV_SDCTRL_BASE_ADRS) & 0x0FFF; \
		sd_timing_config0(DEV_SDCTRL_BASE_ADRS) = sd_timing_config0(DEV_SDCTRL_BASE_ADRS) | (((x) & 0x7)<<12); \
	}while(0)

#define SD_WT_CLK_DELAY_TIME_GET()	((sd_timing_config0(DEV_SDCTRL_BASE_ADRS) >> 12) & 0x7)

#define SD_RD_CLK_DELAY_TIME_GET()	((sd_timing_config1(DEV_SDCTRL_BASE_ADRS)>>13) & 0x7)
#define HISGH_2BIT_FREQ_GET()		((sd_timing_config1(DEV_SDCTRL_BASE_ADRS)>>11) & 0x03)
#define HISGH_2BIT_FREQ_SET(x)		sd_timing_config1(DEV_SDCTRL_BASE_ADRS) = (sd_timing_config1(DEV_SDCTRL_BASE_ADRS) | ((0x03&(x))<<11))

#define SD_RD_CLK_DELAY_TIME_SET(x)	\
	do{ \
		sd_timing_config1(DEV_SDCTRL_BASE_ADRS) = sd_timing_config1(DEV_SDCTRL_BASE_ADRS) & 0x1FFF; \
		sd_timing_config1(DEV_SDCTRL_BASE_ADRS) = sd_timing_config1(DEV_SDCTRL_BASE_ADRS) | (((x) & 0x7)<<13); \
	}while(0)
#define SD_WR_CLK_DELAY_TIME_SET(x)	\
	do{ \
		sd_timing_config0(DEV_SDCTRL_BASE_ADRS) = sd_timing_config0(DEV_SDCTRL_BASE_ADRS) & 0x1FFF; \
		sd_timing_config0(DEV_SDCTRL_BASE_ADRS) = sd_timing_config0(DEV_SDCTRL_BASE_ADRS) | (((x) & 0x7)<<12); \
	}while(0)


#define SD_WAIT_CRC_TIME_SET(x)	\
	do{ \
		sd_timing_config1(DEV_SDCTRL_BASE_ADRS) = sd_timing_config1(DEV_SDCTRL_BASE_ADRS) & 0xF800; \
		sd_timing_config1(DEV_SDCTRL_BASE_ADRS) = sd_timing_config1(DEV_SDCTRL_BASE_ADRS) | ((x) & 0x7FF); \
	}while(0)
#define SD_PAGE_NUM_GET(x)                 sd_page_num(DEV_SDCTRL_BASE_ADRS)
#define SD_PAGE_NUM_SET(x)                 sd_page_num(DEV_SDCTRL_BASE_ADRS) = (x) & 0xFFFF
#define SD_BLOCKSIZE_SET(x)				sd_blocksize(DEV_SDCTRL_BASE_ADRS) = ((x) & 0x7FF)
#define SD_BLOCKSIZE_GET()				(sd_blocksize(DEV_SDCTRL_BASE_ADRS) & 0x7FF)

#define SD_HCSD_EN_GET(x) 				(sd_hw_config(DEV_SDCTRL_BASE_ADRS)&0x01)
#define SD_HCSD_EN_SET(x) \
	do{ \
		sd_hw_config(DEV_SDCTRL_BASE_ADRS) = sd_hw_config(DEV_SDCTRL_BASE_ADRS) & 0xFFFE; \
		sd_hw_config(DEV_SDCTRL_BASE_ADRS) = sd_hw_config(DEV_SDCTRL_BASE_ADRS) | ((x) & 0x1); \
	}while(0)

#define DMA_TYPE_SINGLEBLOCK_CMD	1
#define DMA_TYPE_MULTIBLOCK_CMD		2

#define SD_AUTODMA_TYPE_SET(x)		\
	do{ \
		sd_hw_config(DEV_SDCTRL_BASE_ADRS) = sd_hw_config(DEV_SDCTRL_BASE_ADRS) & 0xFFF9; \
		sd_hw_config(DEV_SDCTRL_BASE_ADRS) = sd_hw_config(DEV_SDCTRL_BASE_ADRS) | (((x) & 0x3)<<1); \
	}while(0)

#define SD_HWDMA_TYPE_GET()		((sd_hw_config(DEV_SDCTRL_BASE_ADRS)>>1) & 0x03)

#define SD_HW_CMD13_EN(x) \
	do{ \
		sd_hw_config(DEV_SDCTRL_BASE_ADRS) = sd_hw_config(DEV_SDCTRL_BASE_ADRS) & 0xFFF7; \
		sd_hw_config(DEV_SDCTRL_BASE_ADRS) = sd_hw_config(DEV_SDCTRL_BASE_ADRS) | (((x)&0x1)<<3); \
	}while(0)

#define SD_HW_CMD13_EN_GET() 	((sd_hw_config(DEV_SDCTRL_BASE_ADRS)>>3)&0x01)

#define SD_HW_DMA_CMD13_RSP_CNT_SET(x) 		do{ \
	sd_hw_config(DEV_SDCTRL_BASE_ADRS) &= ~(0x7FF<<5); \
	sd_hw_config(DEV_SDCTRL_BASE_ADRS) = sd_hw_config(DEV_SDCTRL_BASE_ADRS) | (((x)&0x7ff)<<5); \
}while(0)
#define SD_HW_DMA_CMD13_RSP_CNT_GET() 	((sd_hw_config(DEV_SDCTRL_BASE_ADRS)>>5)&0x7ff)

#define DUMMY_CLOCK_NUMBER_SET(x)		sd_tx_dummy_num(DEV_SDCTRL_BASE_ADRS)=(x&0x1ff)
#define DUMMY_CLOCK_NUMBER_GET(x)		sd_tx_dummy_num(DEV_SDCTRL_BASE_ADRS)

#define SD_CMD_BUF0_GET()				(sd_cmdbuf0(DEV_SDCTRL_BASE_ADRS) & 0xff)
#define SD_CMD_BUF1_GET()				(sd_cmdbuf1(DEV_SDCTRL_BASE_ADRS) & 0xff)
#define SD_CMD_BUF2_GET()				(sd_cmdbuf2(DEV_SDCTRL_BASE_ADRS) & 0xff)
#define SD_CMD_BUF3_GET()				(sd_cmdbuf3(DEV_SDCTRL_BASE_ADRS) & 0xff)
#define SD_CMD_BUF4_GET()				(sd_cmdbuf4(DEV_SDCTRL_BASE_ADRS) & 0xff)

#define SD_CMD_BUF0_SET(x)				sd_cmdbuf0(DEV_SDCTRL_BASE_ADRS) = (x)
#define SD_CMD_BUF1_SET(x)				sd_cmdbuf1(DEV_SDCTRL_BASE_ADRS) = (x)
#define SD_CMD_BUF2_SET(x)				sd_cmdbuf2(DEV_SDCTRL_BASE_ADRS) = (x)
#define SD_CMD_BUF3_SET(x)				sd_cmdbuf3(DEV_SDCTRL_BASE_ADRS) = (x)
#define SD_CMD_BUF4_SET(x)				sd_cmdbuf4(DEV_SDCTRL_BASE_ADRS) = (x)

#define SD_RSP_BUF0_3_GET					sd_rspbuf0_3(DEV_SDCTRL_BASE_ADRS)
#define SD_RSP_BUF4_5_GET					sd_rspbuf4_5(DEV_SDCTRL_BASE_ADRS)


#define SD_CRC7_BUF_GET()				((sd_crc7buf(DEV_SDCTRL_BASE_ADRS) >> 1) & 0x7f)
#define SD_CRC16_BUF0_GET()				sd_crc16buf0lsb(DEV_SDCTRL_BASE_ADRS)
#define SD_CRC16_BUF1_GET()				sd_crc16buf1lsb(DEV_SDCTRL_BASE_ADRS)
#define SD_CRC16_BUF2_GET()				sd_crc16buf2lsb(DEV_SDCTRL_BASE_ADRS)
#define SD_CRC16_BUF3_GET()				sd_crc16buf3lsb(DEV_SDCTRL_BASE_ADRS)

#define SD_HW_STATE_GET()				sd_hw_state(DEV_SDCTRL_BASE_ADRS)
#define SD_HW_DMA_ERROR		0x40
#define SD_HW_DMA_DONE		0x80

#define SD_HW_CMD13_RCA_SET(x)			sd_hw_cmd13_rca(DEV_SDCTRL_BASE_ADRS) = (x)

#define CLK_DELAY_GET()					((sd_clk_dly(DEV_SDCTRL_BASE_ADRS)>>2)&0x07)

#define CLK_DELAY_SET(x) 				do{\
	sd_clk_dly(DEV_SDCTRL_BASE_ADRS) &= ~0x1C;\
	sd_clk_dly(DEV_SDCTRL_BASE_ADRS) |= (((x)&0x07)<<2);\
}while(0)

#define SD_CRC16_FLAG_GET()				(sd_status0(DEV_SDCTRL_BASE_ADRS) & (0x1 << 11)) >> 11

#define CARD_MEDIA_TYPE_SET(x)			card_mediatype(DEV_SDCTRL_BASE_ADRS) = (x)

#define DMA_STATE_RESET()			do{ \
	dma_ctrl(DEV_SDCTRL_BASE_ADRS) = 1; \
	dma_ctrl(DEV_SDCTRL_BASE_ADRS) = 0; \
	dma_hw_stop_rst(DEV_SDCTRL_BASE_ADRS) = 0x02; \
	dma_hw_page_num0(DEV_SDCTRL_BASE_ADRS) = 0; \
	dma_hw_page_num1(DEV_SDCTRL_BASE_ADRS) = 0; \
	dma_hw_page_num2(DEV_SDCTRL_BASE_ADRS) = 0; \
	dma_hw_page_num3(DEV_SDCTRL_BASE_ADRS) = 0; \
	dma_hw_block_num(DEV_SDCTRL_BASE_ADRS) = 0; \
}while(0)

#define DMA_SRC_DST_SET(x)				dma_srcdst(DEV_SDCTRL_BASE_ADRS) = (x)
#define DMA_SRC_DST_GET()				(dma_srcdst(DEV_SDCTRL_BASE_ADRS))

#define AUTO_DMA_HW_EN_SET(x)   	do{ \
	dma_hw_en(DEV_SDCTRL_BASE_ADRS) &= (~0x01); \
	dma_hw_en(DEV_SDCTRL_BASE_ADRS) |= ((x)&0x01); \
}while(0)

#define DMA_HW_EN_SET_HW_DMA()			dma_hw_en(DEV_SDCTRL_BASE_ADRS) |= (0x01)
#define DMA_HW_EN_SET_NORMAL_DMA()		dma_hw_en(DEV_SDCTRL_BASE_ADRS) &= (~0x01)
#define IS_NORMAL_DMA()					((dma_hw_en(DEV_SDCTRL_BASE_ADRS) & 0x01) == 0)
#define AUTO_DMA_HW_EN_GET()			((dma_hw_en(DEV_SDCTRL_BASE_ADRS)) & 1)

#define HW_DMA_BLOCK_MODE_EN_GET()		((dma_hw_en(DEV_SDCTRL_BASE_ADRS)>>1) & 0x01)
#define AUTO_DMA_BLOCK_MODE_EN_SET(x)	do{ \
	dma_hw_en(DEV_SDCTRL_BASE_ADRS) &= (~0x02); \
	dma_hw_en(DEV_SDCTRL_BASE_ADRS) |= (((x)&0x01)<<1); \
}while(0)
#define DMA_BLOCK_MODE_PAGE_LEVEL	0
#define DMA_BLOCK_MODE_BLOCK_LEVEL	1

#define DMA_HW_STOP_RST_SET(x)			dma_hw_stop_rst(DEV_SDCTRL_BASE_ADRS) = (x)
#define DMA_HW_STOP_RST_GET()			dma_hw_stop_rst(DEV_SDCTRL_BASE_ADRS)
#define DMA_HW_STOP_RST_SET_RESET_CH() do{ \
	DMA_HW_STOP_RST_SET(0x04); \
	while (DMA_HW_STOP_RST_GET() & 0x4); \
}while(0)

#define DMA_CMP_INTR_EN_SET(x)			dma_cmpen(DEV_SDCTRL_BASE_ADRS) = (x)
#define AUTO_DMA_BLOCK0_PARA_SET(pageIdx, nrPages)  do{ \
	dma_hw_page_addrl(DEV_SDCTRL_BASE_ADRS) = (pageIdx) & 0x0000ffff;	 \
	dma_hw_page_addrh(DEV_SDCTRL_BASE_ADRS) = ((pageIdx) >> 16) & 0x0000ffff; \
	dma_hw_page_num0(DEV_SDCTRL_BASE_ADRS) = (nrPages) - 1; \
}while(0)

#define AUTO_DMA_BLOCK1_PARA_SET(pageIdx, nrPages)  do{ \
	dma_hw_page_addrl1(DEV_SDCTRL_BASE_ADRS) = (pageIdx) & 0x0000ffff;	 \
	dma_hw_page_addrh1(DEV_SDCTRL_BASE_ADRS) = ((pageIdx) >> 16) & 0x0000ffff; \
	dma_hw_page_num1(DEV_SDCTRL_BASE_ADRS) = (nrPages) - 1; \
}while(0)

#define AUTO_DMA_BLOCK2_PARA_SET(pageIdx, nrPages)  do{ \
	dma_hw_page_addrl2(DEV_SDCTRL_BASE_ADRS) = 	(pageIdx) & 0x0000ffff;	 \
	dma_hw_page_addrh2(DEV_SDCTRL_BASE_ADRS) = 	((pageIdx) >> 16) & 0x0000ffff; \
	dma_hw_page_num2(DEV_SDCTRL_BASE_ADRS) = 		(nrPages) - 1; \
}while(0)

#define AUTO_DMA_BLOCK3_PARA_SET(pageIdx, nrPages)  do{ \
	dma_hw_page_addrl3(DEV_SDCTRL_BASE_ADRS) = (pageIdx) & 0x0000ffff;	 \
	dma_hw_page_addrh3(DEV_SDCTRL_BASE_ADRS) = ((pageIdx) >> 16) & 0x0000ffff; \
	dma_hw_page_num3(DEV_SDCTRL_BASE_ADRS) = (nrPages) - 1; \
}while(0)

#define AUTO_DMA_BLOCK_NUM_SET(x)   	dma_hw_block_num(DEV_SDCTRL_BASE_ADRS) = (x) - 1

#define GL_Card0_CLK_ON()				//francis temp remove for FPGA test GL_moduleClockOn(CARD)
#define GL_Card0_CLK_OFF()				//francis temp remove for FPGA test GL_moduleClockOff(CARD)	//zhaiyong modified

#define hwSdCardCrcState()				((((sd_status1(DEV_SDCTRL_BASE_ADRS)) & 0x70) >> 4) == 0x02)
#define hwSdDat0Check()					(((sd_status0(DEV_SDCTRL_BASE_ADRS)) & 0x20) >> 5)
#define hwSdRxCRC()						sd_ctrl(DEV_SDCTRL_BASE_ADRS) = 0x10
#define hwSdTimeOut()					(((sd_status0(DEV_SDCTRL_BASE_ADRS)) >> 7) & 0x01)

#define SD_CONFIG_SET(x)				sd_config(DEV_SDCTRL_BASE_ADRS) = (x)
#define SD_CONFIG_GET()					sd_config(DEV_SDCTRL_BASE_ADRS)

#define SD_DIV_2_EN()					sd_timing_config0(DEV_SDCTRL_BASE_ADRS) = sd_timing_config0(DEV_SDCTRL_BASE_ADRS) | (1<<15)
#define SD_DIV_2_DIS()					sd_timing_config0(DEV_SDCTRL_BASE_ADRS) = sd_timing_config0(DEV_SDCTRL_BASE_ADRS) & ~(1<<15)
#define SD_DIV_2_GET()					sd_timing_config0(DEV_SDCTRL_BASE_ADRS)

#define SD_UTRL_HSPEED_EN()				do{\
	sd_timing_config1(DEV_SDCTRL_BASE_ADRS) = sd_timing_config1(DEV_SDCTRL_BASE_ADRS) & 0x7FFF; \
	sd_timing_config1(DEV_SDCTRL_BASE_ADRS) = sd_timing_config1(DEV_SDCTRL_BASE_ADRS) | 0x8000; \
}while(0)
#define SD_UTRL_HSPEED_DIS()			do{\
	sd_timing_config1(DEV_SDCTRL_BASE_ADRS) = sd_timing_config1(DEV_SDCTRL_BASE_ADRS) & 0x7FFF; \
}while(0)
#define SD_UTRL_HSPEED_GET()			sd_timing_config1(DEV_SDCTRL_BASE_ADRS)

#define SD_INT_CONTROL_SET(x)              sd_int_control(DEV_SDCTRL_BASE_ADRS) = (x)
#define SD_INT_CONTROL_GET()               sd_int_control(DEV_SDCTRL_BASE_ADRS)

#define SD_SET_CONFIG0(x)                   sd_config0(DEV_SDCTRL_BASE_ADRS) = (x)
#define SD_GET_CONFIG0()                   sd_config0(DEV_SDCTRL_BASE_ADRS)

#define SDIO_RST()						do{\
	sd_rst(DEV_SDCTRL_BASE_ADRS) = 0x07;\
}while(0)						

#define SD_RST()						sd_rst(DEV_SDCTRL_BASE_ADRS) = 0x03
#define SD_CRCRST()						sd_rst(DEV_SDCTRL_BASE_ADRS) = 0x02
#define SD_CTRL_SET(x)					sd_ctrl(DEV_SDCTRL_BASE_ADRS) = (x)
#define SD_CONTROLLER_TRIGGER_COMMAND			0x01
#define SD_CONTROLLER_TRIGGER_RESPONSE			0x02
#define SD_CONTROLLER_TRIGGER_PIO_TXBLOCKDATA	0x04
#define SD_CONTROLLER_TRIGGER_PIO_RXBLOCKDATA	0x08
#define SD_CONTROLLER_TRIGGER_RXCRC				0x10
#define SD_CONTROLLER_TRIGGER_TX_DUMMY_CLOCK	0x02


#define SD_RST_seq() 				do {\
	SDIO_RST(); \
	DMA_HW_STOP_RST_SET(0x01); \
	dma_ctrl(DEV_SDCTRL_BASE_ADRS) = 0; \
	dma_ctrl(DEV_SDCTRL_BASE_ADRS) = 1; \
	dma_ctrl(DEV_SDCTRL_BASE_ADRS) = 0; \
}while(0)

#define SD_STATUS0_GET()				sd_status0(DEV_SDCTRL_BASE_ADRS)
#define SD_STATUS1_GET()				sd_status1(DEV_SDCTRL_BASE_ADRS)

#define SD_PIO_RX_DATA_GET()			sd_piodatarx(DEV_SDCTRL_BASE_ADRS)
#define SD_PIO_TX_DATA_SET(x)			sd_piodatatx(DEV_SDCTRL_BASE_ADRS) = ((x) & (0x0FFFF))

//DMA and card controler register

#define DMA_SIZE_SET(x)					dma_size(DEV_SDCTRL_BASE_ADRS) = (x)
#define DMA_SIZE_GET()					dma_size(DEV_SDCTRL_BASE_ADRS)

#define DMA_START()						dma_start(DEV_SDCTRL_BASE_ADRS) = 1

#define DMA_CMP_INTR_CLEAR()			dma_cmp(DEV_SDCTRL_BASE_ADRS) = 0
#define DMA_CMP_INTR_GET()				dma_cmp(DEV_SDCTRL_BASE_ADRS)


#define BIT_N(n)  (0x01<<(n))
#define SET_BIT_N(val, n) (val) |= BIT_N(n)
#define CLR_BIT_N(val, n) (val) &= ~(BIT_N(n))
#define GET_BIT_N(val, n)  ((((val) & BIT_N(n)) >> (n)) & 0x01)


/******************************************************************************
 *                          D A T A    T Y P E S
 ******************************************************************************/
//for card acceleration feature
typedef struct access_parameter_info_t {
	unsigned int pageIdx[4];
	unsigned int nrPages[4];
	unsigned int BlockNum;
	unsigned int hw_block_mode_en;
	unsigned int hw_cmd_mode;	// 1:hardware send cmd17, 2:hw send cmd18

	unsigned char *pdram;
} sd_access_parameter_info_t;


typedef enum  sdio_cmd_type_t
{
	SDIO_CMD_READ,
	SDIO_CMD_WRITE,
	SDIO_CMD_OLY_CMD,
	SDIO_CMD_STREAM,
}sdio_cmd_type_t;

/******************************************************************************
 *                          F U N C T I O N    D E C L A R A T I O N S
 ******************************************************************************/
void Xil_Out32(unsigned int OutAddress, unsigned int Value);
unsigned int Xil_In32(unsigned int Addr);
extern  bool is_sd_cmp_int(void);
extern  bool is_sdio_int(void);
extern  bool is_dma_int(void);
extern bool sd_rsp_buff_full(void);


extern void config_send_buf(u32 arg, u32 cmd);
extern 	void config0_for_send_cmd(u32 cmd, u32 rsp_type, sdio_cmd_type_t cmd_type);
extern void config0_for_dma(u32 wr);
extern 	void reset_dut_sdio(void);
extern 	u32 hw_sd_check_rsp_timeout(void);
extern void disable_sd_cmp_int(void);
extern void enable_sd_cmp_int(void);
extern 	void hw_start_transation(void);
extern void clear_sd_cmp_int(void);
extern	bool is_nomal_dma_err_timeout(void);
extern void sw_rst_channel(void);
extern u32 get_rst_channel_stat(void);
extern void enable_sdio_int(void);
extern void disable_sdio_int(void);
extern void sdio_int_clr(void);
extern void  set_sd_tx_dummy_num(u32 rounds);
extern u32 hw_prepare_xfer_data_pio(u32 wr);

#endif //#ifndef __HW_SDIO_H__
