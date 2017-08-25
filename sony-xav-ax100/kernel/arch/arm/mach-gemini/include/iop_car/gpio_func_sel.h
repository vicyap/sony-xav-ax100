#ifndef __8600_GPIO_FUNC_SEL_H
#define __8600_GPIO_FUNC_SEL_H

//********************************************************************
//							function select area
//********************************************************************

#define BD_EMU_TYPE1_ATA  1
#define	BD_EMU_TYPE2_CARD_NAND 2
#define	DTV_TYPE_1 3
#define	DTV_TYPE_2 4
#define	DTV_TYPE_DVBS_TEST 5
#define DVBT_EVB_TYPE_1 6
#define DVBT_EVB_TYPE_2 7
#define DVBS_EVB_TYPE_1 8
#define DVBT_DEMO_TYPE_1 9
#define DVBS_DEMO_TYPE_1 10
#define DVBT_DEMO_TYPE_ATA 11
#define	BD_EMU_TYPE3_MARK_VFD 12
#define DVBT_DEMO_TYPE_2 13
#define DVBS_DEMO_TYPE_2 14
#define DVBC_DEMO_TYPE_1 15
#define DVBS_SPH10019B 16
#define SBTVD_DEMO_TYPE_1 17 //for SBTVD
#define DVBS_DEMO_1CI	18
#define DVBS_SZ10047_PINMUX 19
#define DVBT_LQFP256_SPH10095B 20//for LQFP PCB SPH10095B
#define DVBST_DEMO_TYPE_1    21 //lijin 20101227 added for DVBST
#define DVBST_DEMO_TYPE_2    22 //lijin 20110330 added for DVBST
#define DVBT_256PIN_TYPE_1	 23 //yukai added for mantis 140545
#define DVBS_216PIN_TYPE_1	 24 //yukai added for mantis 140545
#define DVBT_216PIN_TYPE_1	 25 //yukai added for mantis 140545
#define DVB2S_SZ10047_DEMO_TYPE_1     26 //lijin 20110808 added for Dual DVBS
//#define BD_EMU_8602_VERIFY_TYPE_1 27//(Titan removed, DVB team can use this number.)
#define DVBC_216PIN_TYPE_1	 28
#define DVBT_216PIN_SZ11089_PINMUX  29  //lizhan 20111110 added for sz-11089 (SUNPLUS_letb1502&1512_DEMO_BOARD_V1.0)
#define DVBS_216PIN_SZ11089_PINMUX  30  //lizhan 20111110 added for sz-11089 (SUNPLUS_letb1502&1512_DEMO_BOARD_V1.0)
#define SBTVD_216PIN_TYPE_1  31  //haleliu @20111206 added for sz-11089 (SUNPLUS_letb1502&1512_DEMO_BOARD_V1.0)
#define DVB2S_216pin_SZ10089_DEMO_TYPE_1   32  //lijin 20111207 added for 1502 Dual DVBS (SUNPLUS_letb1502&1512_DEMO_BOARD_V1.0)
#define DVBST_216pin_SZ10089_TYPE_1   33  //lijin 20111227 added for 1502 DVBS+T (SUNPLUS_letb1502&1512_DEMO_BOARD_V1.0)
#define BD_EMU_8602_TYPE_1_PBGA    34//TitanHo added for 8602 PBGA type. 20120112
#define BD_EMU_8602_TYPE_2_BP_CI   35//TitanHo added for 8602 BD player+CI type.. 20120112
#define BD_EMU_8602_TYPE_3_RESERVE 36//Reserve for future use.(you can use this if need.) 20120112
#define SBTVD_DVBS_216PIN_TYPE_1   37  //yanan 20120107 added for 1502 SBTVD+DVBS (SUNPLUS_letb1502&1512_DEMO_BOARD_V1.0)
#define DVBS_256PIN_SPH11105		38
#define DVBC_216PIN_TYPE_2	 	   39	//haleliu @20120222 added for dvbc new board
#define DVBC_256PIN_TYPE_1	       40
#define MKV_8203L_TYPE_1_216PIN    41  //jeffrey.c added for 8203L
#define MKV_8203L_TYPE_2_XXXPIN    42
#define MKV_8203L_TYPE_3_XXXPIN    43
#define DVBT_128PIN_TYPE_1	       44 //suntree
#define DVBC_128PIN_TYPE_1		45 //yukai added for dvbc on 1007B
#define DVB2T_128PIN_TYPE_1		   46 //weijian added for DVBT2 on 1505
#define DVBS_1506_216PIN_TYPE_1		   47 //suntree added for dvbs on 1506
#define DVBS_1506_144PIN_TYPE_1		   48 //suntree added for dvbs on 1506
#define DVBS_1506_128PIN_TYPE_1        49 //glenn added for dvbs on 1506
#define DVBS_1506_216PIN_TYPE_2		   50
#define DVBS_1506_144PIN_TYPE_2		   51

#if (PLT_PINMUX_TYPE == BD_EMU_TYPE1_ATA)
#include "gpio_func_sel_bd_emu_type1_ata.h"
#elif (PLT_PINMUX_TYPE == BD_EMU_TYPE2_CARD_NAND)
#include "gpio_func_sel_bd_emu_type2_card_nand.h"
#elif (PLT_PINMUX_TYPE == BD_EMU_TYPE3_MARK_VFD)
#include "gpio_func_sel_bd_emu_type3_mark_vfd.h"
#elif (PLT_PINMUX_TYPE == DVBT_EVB_TYPE_1 || PLT_PINMUX_TYPE == DVBT_EVB_TYPE_2 || PLT_PINMUX_TYPE == DVBS_EVB_TYPE_1 \
 || PLT_PINMUX_TYPE == DVBT_DEMO_TYPE_1 || PLT_PINMUX_TYPE == DVBT_DEMO_TYPE_2 || PLT_PINMUX_TYPE == DVBS_DEMO_TYPE_1 \
 || PLT_PINMUX_TYPE == DVBS_DEMO_TYPE_2|| PLT_PINMUX_TYPE == DVBC_DEMO_TYPE_1|| PLT_PINMUX_TYPE == DVBS_SPH10019B || PLT_PINMUX_TYPE == SBTVD_DEMO_TYPE_1	\
 || PLT_PINMUX_TYPE == DVBS_DEMO_1CI || PLT_PINMUX_TYPE == DVBS_SZ10047_PINMUX || PLT_PINMUX_TYPE == DVBT_LQFP256_SPH10095B || PLT_PINMUX_TYPE == DVBS_256PIN_SPH11105	\
 || PLT_PINMUX_TYPE == DVBT_256PIN_TYPE_1)
#include "gpio_func_sel_dvb_evb_type1.h"
#elif (PLT_PINMUX_TYPE == DVBT_DEMO_TYPE_ATA )
#include "gpio_func_sel_dvb_evb_type_ata.h"
#elif (PLT_PINMUX_TYPE == DVBST_DEMO_TYPE_1 ||PLT_PINMUX_TYPE == DVBST_DEMO_TYPE_2 || PLT_PINMUX_TYPE == DVB2S_SZ10047_DEMO_TYPE_1 \
 || PLT_PINMUX_TYPE == DVB2S_216pin_SZ10089_DEMO_TYPE_1  || PLT_PINMUX_TYPE == DVBST_216pin_SZ10089_TYPE_1)    //lijin 20110808 add for Dual DVBS NIM     //lijin 20110808 add for Dual DVBS NIM     //20101227lijin added for DVB_S&T
#include "gpio_func_sel_dvbst_evb_type.h"
#elif (PLT_PINMUX_TYPE == SBTVD_DVBS_216PIN_TYPE_1)//yanan added 20120130 for SBTVD+DVBS
#include "gpio_func_sel_sbtvd_dvbs_evb_type.h"
#elif ((PLT_PINMUX_TYPE==DVBS_216PIN_TYPE_1)||(PLT_PINMUX_TYPE==DVBS_216PIN_SZ11089_PINMUX))
#include "gpio_func_sel_216pin_dvbs_type1.h"
#elif ((PLT_PINMUX_TYPE==DVBT_216PIN_TYPE_1)||(PLT_PINMUX_TYPE==DVBT_216PIN_SZ11089_PINMUX)||(PLT_PINMUX_TYPE==SBTVD_216PIN_TYPE_1))
#include "gpio_func_sel_216pin_dvbt_type1.h"
#elif (PLT_PINMUX_TYPE==DVBC_216PIN_TYPE_1 || PLT_PINMUX_TYPE==DVBC_216PIN_TYPE_2)
#include "gpio_func_sel_216pin_dvbc_type1.h"
#elif (PLT_PINMUX_TYPE==DVBC_256PIN_TYPE_1)
#include "gpio_func_sel_256pin_dvbc_type1.h"
#elif (PLT_PINMUX_TYPE==BD_EMU_8602_TYPE_1_PBGA)
#include "gpio_func_sel_bd_emu_8602_type1_pbga.h"
#elif (PLT_PINMUX_TYPE==BD_EMU_8602_TYPE_2_BP_CI)
#include "gpio_func_sel_bd_emu_8602_type2_bp_ci.h"
#elif (PLT_PINMUX_TYPE==MKV_8203L_TYPE_1_216PIN)        //jeffrey.c added for 8203L
#include "gpio_func_sel_mkv_8203L_type1_216pin.h"
#elif (PLT_PINMUX_TYPE==DVBT_128PIN_TYPE_1)        
#include "gpio_func_sel_128pin_dvbt_type1.h"
#elif (PLT_PINMUX_TYPE==DVBC_128PIN_TYPE_1)        
#include "gpio_func_sel_128pin_dvbc_sd_type1.h"
#elif (PLT_PINMUX_TYPE==DVB2T_128PIN_TYPE_1)
#include "gpio_func_sel_128pin_dvb2t_type1.h"
#elif (PLT_PINMUX_TYPE==DVBS_1506_216PIN_TYPE_1)        //suntree added for 1506
#include "gpio_func_sel_1506_216pin_dvbs_type1.h"
#elif (PLT_PINMUX_TYPE==DVBS_1506_144PIN_TYPE_1)        //suntree added for 1506
#include "gpio_func_sel_1506_144pin_dvbs_type1.h"
#elif (PLT_PINMUX_TYPE==DVBS_1506_128PIN_TYPE_1)
#include "gpio_func_sel_1506_128pin_dvbs_type1.h"
#elif (PLT_PINMUX_TYPE==DVBS_1506_216PIN_TYPE_2)
#include "gpio_func_sel_1506_216pin_dvbs_type2.h"
#elif (PLT_PINMUX_TYPE==DVBS_1506_144PIN_TYPE_2)
#include "gpio_func_sel_1506_144pin_dvbs_type2.h"
#else
#error "No such board config"
#endif	// PLT_PINMUX_TYPE1

#endif//__8600_GPIO_FUNC_SEL_H
