#ifndef __GPIO_SEL_H__
#define __GPIO_SEL_H__

//#include "xxx_config.h"
#include "platform_define.h" //Kaney add
#include "customize.h" //Kaney add
#include "regmap.h" //Kaney add
#include "gpio_func_sel.h"	// gpio function selection

// pin mux type selection
#if (PLT_PINMUX_TYPE == BD_EMU_TYPE1_ATA)
	#include "gpio_pin_map_8600.h"			// gpio map selection, one for each IC
	#include "bd_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == BD_EMU_TYPE2_CARD_NAND)
	#include "gpio_pin_map_8600.h"			// gpio map selection, one for each IC
	#include "bd_pinmux_type2.h"
#elif (PLT_PINMUX_TYPE == BD_EMU_TYPE3_MARK_VFD)
	#include "gpio_pin_map_8600.h"			// gpio map selection, one for each IC
	#include "bd_pinmux_type3.h"
#elif (PLT_PINMUX_TYPE == DVBT_EVB_TYPE_1)
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvb_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVBT_EVB_TYPE_2)
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvb_pinmux_type2.h"
#elif (PLT_PINMUX_TYPE == DVBS_EVB_TYPE_1)
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvbs_pinmux_type1.h"	
#elif (PLT_PINMUX_TYPE == DVBT_DEMO_TYPE_1)
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvbt_demo_pinmux_type1.h"	
#elif (PLT_PINMUX_TYPE == DVBS_DEMO_TYPE_1)
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvbs_demo_pinmux_type1.h"	
#elif (PLT_PINMUX_TYPE == DVBT_DEMO_TYPE_ATA)
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvb_pinmux_type3.h"
#elif (PLT_PINMUX_TYPE == DVBT_DEMO_TYPE_2)
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvbt_demo_pinmux_type2.h"	
#elif (PLT_PINMUX_TYPE == DVBS_DEMO_TYPE_2)
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvbs_demo_pinmux_type2.h"	
#elif (PLT_PINMUX_TYPE == DVBST_DEMO_TYPE_1)	// 20101227 lijin add for dvb_S+T
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvbst_demo_pinmux_type1.h"	
#elif (PLT_PINMUX_TYPE == DVBST_DEMO_TYPE_2)	// 20101227 lijin add for dvb_S+T
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvbst_demo_pinmux_type2.h"	
#elif (PLT_PINMUX_TYPE == DVBC_DEMO_TYPE_1)	// 20100416 zhangjie add for dvb_c
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvbc_demo_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVBS_SPH10019B)
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvbs_sph10019b_pinmux.h"		
#elif (PLT_PINMUX_TYPE == SBTVD_DEMO_TYPE_1)	// 20100505 haleliu add for SBTVD
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "sbtvd_demo_pinmux_type1.h"	
#elif (PLT_PINMUX_TYPE == DVBS_DEMO_1CI)
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvbs_demo_pinmux_1ci.h"	
#elif (PLT_PINMUX_TYPE == DVBS_SZ10047_PINMUX)
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvbs_sz10047_pinmux.h"	
#elif (PLT_PINMUX_TYPE == DVB2S_SZ10047_DEMO_TYPE_1)
	#include "gpio_pin_map_1500.h"			// gpio map selection, one for each IC
	#include "dvb2S_demo_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVBT_LQFP256_SPH10095B)
	#include "gpio_pin_map_1500_lqfp256.h"			// gpio map selection, one for each IC
	#include "dvbt_lqfp256_sph10095b_pinmux.h"
#elif (PLT_PINMUX_TYPE == DVBT_256PIN_TYPE_1)
	#include "gpio_pin_map_1502.h"			// gpio map selection, one for each IC
	#include "dvbt_256pin_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVBS_216PIN_TYPE_1)
	#include "gpio_pin_map_1502.h"			// gpio map selection, one for each IC
	#include "dvbs_216pin_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVBC_216PIN_TYPE_1)
	#include "gpio_pin_map_1502.h"			// gpio map selection, one for each IC
	#include "dvbc_216pin_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVBC_216PIN_TYPE_2)		//haleliu @20120222 added
	#include "gpio_pin_map_1502.h"			// gpio map selection, one for each IC
	#include "dvbc_216pin_pinmux_type2.h"
#elif (PLT_PINMUX_TYPE == DVBT_216PIN_TYPE_1)
	#include "gpio_pin_map_1502.h"			// gpio map selection, one for each IC
	#include "dvbt_216pin_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVBT_216PIN_SZ11089_PINMUX)
	#include "gpio_pin_map_1502.h"			// gpio map selection, one for each IC
	#include "dvbt_216pin_sz11089_pinmux.h"
#elif (PLT_PINMUX_TYPE == DVBS_216PIN_SZ11089_PINMUX)
	#include "gpio_pin_map_1502.h"			// gpio map selection, one for each IC
	#include "dvbs_216pin_sz11089_pinmux.h"
#elif (PLT_PINMUX_TYPE == BD_EMU_8602_TYPE_1_PBGA)
	#include "gpio_pin_map_8602.h"			// gpio map selection, one for each IC
	#include "bd_pinmux_8602_type1_pbga.h"
#elif (PLT_PINMUX_TYPE == BD_EMU_8602_TYPE_2_BP_CI)
	#include "gpio_pin_map_8602.h"			// gpio map selection, one for each IC
	#include "bd_pinmux_8602_type2_bp_ci.h"
#elif (PLT_PINMUX_TYPE == SBTVD_216PIN_TYPE_1)
	#include "gpio_pin_map_1502.h"			// gpio map selection, one for each IC
	#include "sbtvd_216pin_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVB2S_216pin_SZ10089_DEMO_TYPE_1)
	#include "gpio_pin_map_1502.h"			// gpio map selection, one for each IC
	#include "dvb2S_216pin_sz11089_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVBST_216pin_SZ10089_TYPE_1)
	#include "gpio_pin_map_1502.h"			// gpio map selection, one for each IC
	#include "dvbst_216pin_sz11089_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == SBTVD_DVBS_216PIN_TYPE_1)
	#include "gpio_pin_map_1502.h"			// gpio map selection, one for each IC
	#include "sbtvd_dvbs_216pin_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVBS_256PIN_SPH11105)
	#include "gpio_pin_map_1502.h"			// gpio map selection, one for each IC
	#include "dvbs_256pin_sph11105_pinmux.h"
#elif (PLT_PINMUX_TYPE == DVBC_256PIN_TYPE_1)
	#include "gpio_pin_map_1502.h"			// gpio map selection, one for each IC
	#include "dvbc_256pin_pinmux_type1.h"	
#elif (PLT_PINMUX_TYPE == MKV_8203L_TYPE_1_216PIN)
	#include "gpio_pin_map_8602.h"	        //gpio map selection, one for each IC
	#include "mkv_pinmux_8203L_type1_216pin.h"
#elif (PLT_PINMUX_TYPE == DVBT_128PIN_TYPE_1)
	#include "gpio_pin_map_1503.h"	        //gpio map selection, one for each IC
	#include "dvbt_128pin_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVBC_128PIN_TYPE_1)
	#include "gpio_pin_map_1007b.h"	        //gpio map selection, one for each IC
	#include "dvbc_sd_128pin_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVB2T_128PIN_TYPE_1)
	#include "gpio_pin_map_1505.h"	        //gpio map selection, one for each IC
	#include "dvb2t_128pin_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVBS_1506_216PIN_TYPE_1)
	#include "gpio_pin_map_1506.h"	        //gpio map selection, one for each IC
	#include "dvbs_1506_216pin_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVBS_1506_144PIN_TYPE_1)
	#include "gpio_pin_map_1506.h"	        //gpio map selection, one for each IC
	#include "dvbs_1506_144pin_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVBS_1506_128PIN_TYPE_1)
	#include "gpio_pin_map_1506.h"	        //gpio map selection, one for each IC
	#include "dvbs_1506_128pin_pinmux_type1.h"
#elif (PLT_PINMUX_TYPE == DVBS_1506_216PIN_TYPE_2)
	#include "gpio_pin_map_1506.h"	        //gpio map selection, one for each IC
	#include "dvbs_1506_216pin_pinmux_type2.h"
#elif (PLT_PINMUX_TYPE == DVBS_1506_144PIN_TYPE_2)
	#include "gpio_pin_map_1506.h"	        //gpio map selection, one for each IC
	#include "dvbs_1506_144pin_pinmux_type2.h"
#else
	#error "no such pinmux type"
#endif	// PLT_PINMUX_TYPE1
#include "pin_allocator.h"	// pin allocator API

#if 1 //Kaney mark: refer to <mach/gpio_drv.h>
// GPIO macro
#define GPIO_F_SET(a,d) do { if (d) {HWREG_W(gpio_first[a/32], HWREG_R(gpio_first[a/32])|(1<<(a%32))); \
                            } else { HWREG_W(gpio_first[a/32], HWREG_R(gpio_first[a/32])& ~(1<<(a%32))); }} while (0)

#define GPIO_M_SET(a,d) do { if (d) {HWREG_W(gpio_master[a/32], HWREG_R(gpio_master[a/32])|(1<<(a%32))); \
                            } else  {HWREG_W(gpio_master[a/32], HWREG_R(gpio_master[a/32])&~(1<<(a%32))); }} while (0)

#if ((IC_VER==QCE491)||(IC_VER == QAF536)||(IC_VER == QAF561)) //Rex_110816 modify: for 1502 GPIO[15] read data fail
#define GPIO_E_SET(a,d) do {\
							if(a==15) \
							{\
								if(d==0){\
								HWREG_W(pad_ctrl[0], (HWREG_R(pad_ctrl[0])|(1<<22))); \
								HWREG_W(pad_ctrl[0], (HWREG_R(pad_ctrl[0])|0xf)); \
								}\
								else\
									HWREG_W(pad_ctrl[0], (HWREG_R(pad_ctrl[0])&~(1<<22)));\
							} \
							if (d) {\
								HWREG_W(gpio_oe[a/32], HWREG_R(gpio_oe[a/32])|(1<<(a%32))); \
                            			} else  {\
                            				HWREG_W(gpio_oe[a/32], HWREG_R(gpio_oe[a/32])&~(1<<(a%32))); \
                            			}\
                            		} while (0)
#else
#define GPIO_E_SET(a,d) do { if (d) {HWREG_W(gpio_oe[a/32], HWREG_R(gpio_oe[a/32])|(1<<(a%32))); \
                            } else  {HWREG_W(gpio_oe[a/32], HWREG_R(gpio_oe[a/32])&~(1<<(a%32))); }} while (0)
#endif

#define GPIO_O_SET(a,d) do { if (d) {HWREG_W(gpio_out[a/32], HWREG_R(gpio_out[a/32])|(1<<(a%32))); \
                            } else  {HWREG_W(gpio_out[a/32], HWREG_R(gpio_out[a/32])&~(1<<(a%32))); }} while (0)

#define GPIO_I_GET(a)   ((HWREG_R(gpio_in[a/32]) >> (a%32)) & 0x01)
#endif

#endif	// __GPIO_SEL_H__
