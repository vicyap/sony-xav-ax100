
#ifndef __REG_SYSTEM_H__
#define __REG_SYSTEM_H__

#include <mach/hardware.h>
//#include <mach/typedef.h>


#define SYSTEM_BASE						IO0_ADDRESS(0*32*4 )
#define LOGI_ADDR_MOON_REG			(SYSTEM_BASE + 0*32*4)
#define LOGI_ADDR_MOON1_REG			(SYSTEM_BASE + 1*32*4)
#define LOGI_ADDR_MOON2_REG			(SYSTEM_BASE + 2*32*4)
#define LOGI_ADDR_TOUCH_REG			(SYSTEM_BASE + 94*32*4)

#define LOGI_ADDR_INT_CTRL_G0_REG		(SYSTEM_BASE + 9*32*4)
#define LOGI_ADDR_INT_CTRL_G1_REG		(SYSTEM_BASE + 10*32*4)
#define LOGI_ADDR_STC_REG			(SYSTEM_BASE + 12*32*4)
#define LOGI_ADDR_STC_AV1_REG			(SYSTEM_BASE + 97*32*4)
#define LOGI_ADDR_STC_AV2_REG			(SYSTEM_BASE + 99*32*4)

#define LOGI_ADDR_DPLL0			        (SYSTEM_BASE + 20*32*4 +  0*4)
#define LOGI_ADDR_DPLL1			        (SYSTEM_BASE + 20*32*4 +  8*4)
#define LOGI_ADDR_DPLL2			        (SYSTEM_BASE + 20*32*4 + 16*4)
#define LOGI_ADDR_DPLL3			        (SYSTEM_BASE + 20*32*4 + 24*4)
#define STC_SZ 0x80
/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
	// System

typedef struct moonReg_s {
    volatile unsigned int stamp;            // 0.0
    volatile unsigned int g0_reserved_0[3]; // 0.1
    volatile unsigned int clk_sel[2];       // 0.4
    volatile unsigned int pllsys_cfg_sel;   // 0.6
    volatile unsigned int clken[5];         // 0.7
    volatile unsigned int gclken[5];        // 0.12
    volatile unsigned int reset[5];         // 0.17
    volatile unsigned int g0_reserved_2[6]; // 0.22
    volatile unsigned int hw_cfg;           // 0.28
    volatile unsigned int sfg_cfg_func;     // 0.29
    volatile unsigned int g0_reserved_3[2]; // 0.30
} moonReg_t;

typedef struct moon1Reg_s {
		// Group 1   : Moon 1
	/* volatile UINT8 regOffset[0x000000]; */ /* 0x9c000000 */
	volatile unsigned int sft_cfg_0;
	volatile unsigned int sft_cfg_1;
	volatile unsigned int sft_cfg_2;
	volatile unsigned int sft_cfg_3;
	volatile unsigned int sft_cfg_4;
	volatile unsigned int sft_cfg_5;
	volatile unsigned int sft_cfg_6;
	volatile unsigned int sft_cfg_7;
	volatile unsigned int sft_cfg_8;
	volatile unsigned int sft_cfg_9;
	volatile unsigned int sft_cfg_10;
	volatile unsigned int sft_cfg_11;
	volatile unsigned int sft_cfg_12;
	volatile unsigned int sft_cfg_13;
	volatile unsigned int sft_cfg_14;
	volatile unsigned int sft_cfg_15;
	volatile unsigned int sft_cfg_16;
	volatile unsigned int sft_cfg_17;
	volatile unsigned int sft_cfg_18;
	volatile unsigned int sft_cfg_19;
	volatile unsigned int sft_cfg_20;
	volatile unsigned int sft_cfg_21;
	volatile unsigned int sft_cfg_22;
	volatile unsigned int sft_cfg_23;
	volatile unsigned int sft_cfg_24;
	volatile unsigned int sft_cfg_25;
	volatile unsigned int sft_cfg_26;
	volatile unsigned int sft_cfg_27;
	volatile unsigned int sft_cfg_28;
	volatile unsigned int sft_cfg_29;
	volatile unsigned int sft_cfg_30;
	volatile unsigned int sft_cfg_31;
}moon1Reg_t;

typedef struct moon2Reg_s {
        // Group 2   : Moon 2
    volatile unsigned int aadc_gpio_ctl_0;  // 2.0
    volatile unsigned int aadc_gpio_ctl_1;  // 2.1
    volatile unsigned int uphy0_otp_ctl;    // 2.2
    volatile unsigned int usbc_ctl;         // 2.3
    volatile unsigned int pllsp_ctl_0;      // 2.4
    volatile unsigned int pllsp_ctl_1;      // 2.5
    volatile unsigned int pllsp_ctl_2;      // 2.6
    volatile unsigned int pllsp_ctl_3;      // 2.7
    volatile unsigned int srv_gpio_ctl;     // 2.8
    volatile unsigned int rsv_2_9;
    volatile unsigned int disp_prob_sel;    // 2.10
    volatile unsigned int adac_gpio_ctl;    // 2.11
    volatile unsigned int gc520_ctl;        // 2.12
    volatile unsigned int svr_ctl;          // 2.13
    volatile unsigned int rsv_2_14;
    volatile unsigned int rsv_2_15;
    volatile unsigned int rsv_2_16;
    volatile unsigned int rsv_2_17;
    volatile unsigned int rsv_2_18;
    volatile unsigned int rsv_2_19;
    volatile unsigned int rsv_2_20;
    volatile unsigned int rsv_2_21;
    volatile unsigned int rsv_2_22;
    volatile unsigned int rsv_2_23;
    volatile unsigned int rsv_2_24;
    volatile unsigned int rsv_2_25;
    volatile unsigned int rsv_2_26;
    volatile unsigned int rsv_2_27;
    volatile unsigned int rsv_2_28;
    volatile unsigned int rsv_2_29;
    volatile unsigned int rsv_2_30;
    volatile unsigned int rsv_2_31;
}moon2Reg_t;

typedef struct int0Reg_s {
	// Group 9   : INT 0
	/* volatile UINT8 regOffset[0x000000]; */ /* 0x9c000000 */
	volatile unsigned int intr_l1_flag;
	volatile unsigned int intr_l1_polarity;
	volatile unsigned int intr_l1_edge;
	volatile unsigned int risc0_intr_lx_sel;
	volatile unsigned int risc0_intr_l0_flag;
	volatile unsigned int risc0_intr_l0_polarity;
	volatile unsigned int risc0_intr_l0_edge;
	volatile unsigned int risc0_intr0_mask;
	volatile unsigned int risc0_intr1_mask;
	volatile unsigned int risc0_intr2_mask;
	volatile unsigned int risc0_intr3_mask;
	volatile unsigned int risc0_intr4_mask;
	volatile unsigned int risc0_intr5_mask;
	volatile unsigned int risc1_intr0_mask;
	volatile unsigned int risc1_intr1_mask;
	volatile unsigned int risc1_intr2_mask;
	volatile unsigned int risc1_intr3_mask;
	volatile unsigned int risc1_intr4_mask;
	volatile unsigned int risc1_intr5_mask;
	volatile unsigned int risc0_intr0_masked;
	volatile unsigned int risc0_intr1_masked;
	volatile unsigned int risc0_intr2_masked;
	volatile unsigned int risc0_intr3_masked;
	volatile unsigned int risc0_intr4_masked;
	volatile unsigned int risc0_intr5_masked;
	volatile unsigned int risc1_intr0_masked;
	volatile unsigned int risc1_intr1_masked;
	volatile unsigned int risc1_intr2_masked;
	volatile unsigned int risc1_intr3_masked;
	volatile unsigned int risc1_intr4_masked;
	volatile unsigned int risc1_intr5_masked;
	volatile unsigned int risc1_intr_lx_sel;
}int0Reg_t;

typedef struct int1Reg_s {
	// Group 10  : INT 1
	/* volatile UINT8 regOffset[0x000000]; */ /* 0x9c000000 */
	volatile unsigned int dispatch_a2b;
	volatile unsigned int dispatch_b2a;
	volatile unsigned int intr_src_l0;
	volatile unsigned int intr_src_l1;
	volatile unsigned int risc_ipti;
	volatile unsigned int intr_ctrl_id;
	volatile unsigned int dispatch_c2a;
	volatile unsigned int dispatch_c2b;
	volatile unsigned int intl0_test_mode;
	volatile unsigned int intl1_test_mode;
	volatile unsigned int dsp_intr_lx_sel;
	volatile unsigned int dsp_intr_mask;
	volatile unsigned int dsp_intr_masked;
	volatile unsigned int sd0_msi_flag;
	volatile unsigned int sd0_msi_mask;
	volatile unsigned int sd0_msi_masked;
	volatile unsigned int sd1_msi_flag;
	volatile unsigned int sd1_msi_mask;
	volatile unsigned int sd1_msi_masked;
	volatile unsigned int risc1_intr_l0_flag;
	volatile unsigned int risc1_intr_l0_polarity;
	volatile unsigned int risc1_intr_l0_edge;
	volatile unsigned int g10_reserved_0[10];
}int1Reg_t;


typedef struct stcReg_s {
	// Group 12  : STC
	/* volatile UINT8 regOffset[0x000000]; */ /* 0x9c000000 */
	volatile unsigned int stc_15_0;
	volatile unsigned int stc_31_16;
	volatile unsigned int stc_32;
	volatile unsigned int stc_divisor;
	volatile unsigned int rtc_15_0;
	volatile unsigned int rtc_23_16;
	volatile unsigned int rtc_divisor;
	volatile unsigned int stc_config;
	volatile unsigned int timer0_ctrl;
	volatile unsigned int timer0_cnt;
	volatile unsigned int timer1_ctrl;
	volatile unsigned int timer1_cnt;
	volatile unsigned int timerw_ctrl;
	volatile unsigned int timerw_cnt;
	volatile unsigned int g12_reserved_0[2];
	volatile unsigned int timer2_ctrl;
	volatile unsigned int timer2_divisor;
	volatile unsigned int timer2_reload;
	volatile unsigned int timer2_cnt;
	volatile unsigned int timer3_ctrl;
	volatile unsigned int timer3_divisor;
	volatile unsigned int timer3_reload;
	volatile unsigned int timer3_cnt;
	volatile unsigned int stcl_0;
	volatile unsigned int stcl_1;
	volatile unsigned int stcl_2;
	volatile unsigned int atc_0;
	volatile unsigned int atc_1;
	volatile unsigned int atc_2;
	volatile unsigned int g12_reserved_1[2];
}stcReg_t;

typedef struct stc_avReg_s {
	// Group 96, 97, 99: STC_AV0 - STC_AV2
	volatile unsigned int stc_15_0;
	volatile unsigned int stc_31_16;
	volatile unsigned int stc_64;
	volatile unsigned int stc_divisor;
	volatile unsigned int rtc_15_0;
	volatile unsigned int rtc_23_16;
	volatile unsigned int rtc_divisor;
	volatile unsigned int stc_config;
	volatile unsigned int timer0_ctrl;
	volatile unsigned int timer0_cnt;
	volatile unsigned int timer1_ctrl;
	volatile unsigned int timer1_cnt;
	volatile unsigned int rsv_12;
	volatile unsigned int rsv_13;
	volatile unsigned int stc_47_32;
	volatile unsigned int stc_63_48;
	volatile unsigned int timer2_ctrl;
	volatile unsigned int timer2_divisor;
	volatile unsigned int timer2_reload;
	volatile unsigned int timer2_cnt;
	volatile unsigned int timer3_ctrl;
	volatile unsigned int timer3_divisor;
	volatile unsigned int timer3_reload;
	volatile unsigned int timer3_cnt;
	volatile unsigned int stcl_0;
	volatile unsigned int stcl_1;
	volatile unsigned int stcl_2;
	volatile unsigned int atc_0;
	volatile unsigned int atc_1;
	volatile unsigned int atc_2;
	volatile unsigned int rsv_30;
	volatile unsigned int rsv_31;
}stc_avReg_t;

typedef struct touchReg_s {
//Group 94
	volatile unsigned int rg_cfg00;
	volatile unsigned int rg_cfg01;
	volatile unsigned int rg_cfg02;
	volatile unsigned int rg_cfg03;
	volatile unsigned int rg_cfg04;
	volatile unsigned int rg_cfg05;
	volatile unsigned int rg_cfg06;
	volatile unsigned int rg_cfg07;
	volatile unsigned int rg_cfg08;
	volatile unsigned int rg_cfg09;
	volatile unsigned int rg_cfg0A;
	volatile unsigned int rg_cfg0B;
	volatile unsigned int rg_cfg0C;
	volatile unsigned int rg_cfg0D;
	volatile unsigned int rg_cfg0E;
	volatile unsigned int rg_cfg0F;

	volatile unsigned int rg_cfg10;
	volatile unsigned int rg_cfg11;
	volatile unsigned int rg_cfg12;
	volatile unsigned int rg_cfg13;
	volatile unsigned int rg_cfg14;
	volatile unsigned int rg_cfg15;
	volatile unsigned int rg_cfg16;
	volatile unsigned int rg_cfg17;
	volatile unsigned int rg_cfg18;
	volatile unsigned int rg_cfg19;
	volatile unsigned int rg_cfg1A;
	volatile unsigned int rg_cfg1B;
	volatile unsigned int rg_cfg1C;
	volatile unsigned int rg_cfg1D;
	volatile unsigned int rg_cfg1E;
	volatile unsigned int rg_cfg1F;
	
}touchReg_t;



#define SP_STC_TIMER0CTL_SRC_STC	(1 << 14)
#define SP_STC_TIMER0CTL_SRC_RTC	(2 << 14)
#define SP_STC_TIMER0CTL_RTP 		(1 << 13)
#define SP_STC_TIMER0CTL_GO 		(1 << 11)
#define SP_STC_TIMER0CTL_TM0_RELOAD_MASK (0x7ff <<0)

#define SP_STC_AV_TIMER01_CTL_SRC_SYS		(0 << 14)
#define SP_STC_AV_TIMER01_CTL_SRC_STC		(1 << 14)
#define SP_STC_AV_TIMER01_CTL_SRC_RTC		(2 << 14)
#define SP_STC_AV_TIMER01_CTL_RTP 		(1 << 13)
#define SP_STC_AV_TIMER01_CTL_GO 		(1 << 11)
#define SP_STC_AV_TIMER01_CTL_RELOAD_MASK 	(0x7ff <<0)

#define SP_STC_AV_TIMER23_CTL_SRC_SYS 		(0 << 2)
#define SP_STC_AV_TIMER23_CTL_SRC_STC 		(1 << 2)
#define SP_STC_AV_TIMER23_CTL_RPT 		(1 << 1)
#define SP_STC_AV_TIMER23_CTL_GO 		(1 << 0)


/* Group 20.0, 20.8, 20.16, 20.24 */
typedef struct dpllReg_s {
    volatile unsigned int dpll_ctrl;
    volatile unsigned int dpll_remainder;
    volatile unsigned int dpll_denominator;
    volatile unsigned int dpll_divider;
    volatile unsigned int dpll_sprd_num;
    volatile unsigned int dpll_reserved[3];
}dpllReg_t;

#endif /* __REG_SYSTEM_H__ */

