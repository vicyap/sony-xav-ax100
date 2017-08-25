#ifndef __HAL_SPHE8388_TCON_H__
#define __HAL_SPHE8388_TCON_H__

#ifndef UBOOT_TCON
	#include "gsl/gl_api.h"
	#include "gsl/gl_malloc.h"
#else
	#include "tcon_uboot.h"
#endif

//debug
//#define TCON_DEBUG
#ifdef TCON_DEBUG
	#define STATIC_ASSERT(b) extern int _static_assert[ b ? 1 : -1 ]
#else
	#define STATIC_ASSERT(b)
#endif

typedef unsigned char		BOOL;

#ifndef ENABLE
	#define ENABLE			1
#endif

#ifndef DISABLE
	#define DISABLE			0
#endif

#ifndef TRUE
	#define TRUE			1
#endif

#ifndef FALSE
	#define FALSE			0
#endif

#ifndef NULL
	#define NULL			(void *)0
#endif

#ifndef __UNUSED
	#define __UNUSED		__attribute__((unused))
#endif

//Gamma table length
#define GAMMA_LEN			512

//SPI table length
#define SPI_HEADER_LEN		20
#define SPI_LEN				510		//2byte reserved for table_len

#define FALLING		0
#define RISING		1
#define FLOATING	2

//GPIO number
#define GPIO_LVDS_EVEN_P3	154
#define GPIO_LVDS_EVEN_N3	153
#define GPIO_LVDS_EVEN_PCLK	152
#define GPIO_LVDS_EVEN_NCLK	151
#define GPIO_LVDS_EVEN_P2	150
#define GPIO_LVDS_EVEN_N2	149
#define GPIO_LVDS_EVEN_P1	148
#define GPIO_LVDS_EVEN_N1	147
#define GPIO_LVDS_EVEN_P0	146
#define GPIO_LVDS_EVEN_N0	145

#define GPIO_LVDS_ODD_P3	144
#define GPIO_LVDS_ODD_N3	143

#define GPIO_DE		GPIO_LVDS_EVEN_N3		//in LVDS pad
#define GPIO_HS		GPIO_LVDS_EVEN_P3		//in LVDS pad
#define GPIO_VS		24
#define GPIO_CLK	25
#define GPIO_R0		27
#define GPIO_R1		28
#define GPIO_G0		29
#define GPIO_G1		30
#define GPIO_B0		31
#define GPIO_B1		32

enum
{
	eDECREASING = 0,
	eINCREASING,
};

typedef enum
{
	ePANEL_SUCCESS,				/*!< successful outcome	*/
	ePANEL_ERR_FAILURE,			/*!< operation failed	*/
	ePANEL_ERR_INVALID_PARAM,	/*!< invalid parameter	*/
	ePANEL_ERR_MAX				/*!< Max error number	*/
} PANEL_Status_t;

typedef enum
{
	eFMT_NTSC,
	eFMT_PAL,
	eFMT_MAX,
} TCON_FMT_t;

typedef enum
{
	eCLK_REF,		//spec timing.
	eNTSC_59_94,
	ePAL_50,
	eCLK_MAX,
} TCON_CLK_t;

enum
{
	ePANEL_ALL_GAMMA	= 0,
	ePANEL_R_GAMMA		= 1,
	ePANEL_G_GAMMA		= 2,
	ePANEL_B_GAMMA		= 3,
};

typedef enum
{
	eTCON_PWM1,
	eTCON_PWM2,
	eTCON_PWM3,
	eTCON_PWM4,
	eTCON_PWM5,
	eTCON_PWM6,
	eTCON_PWM7,
	eTCON_PWM8,
	eTCON_PWM_DISABLE = 0xff,
} TCON_PWM_t;

typedef enum
{
	eTCON_PWM_DD0,
	eTCON_PWM_DD1,
	eTCON_PWM_DD2,
	eTCON_PWM_DD3,
} TCON_PWM_DD_t;

typedef enum
{
	ePLL_CP_0,
	ePLL_CP_1,
	ePLL_CP_2,
	ePLL_CP_3,
	ePLL_CP_4,
	ePLL_CP_5,
	ePLL_CP_6,
	ePLL_CP_7,
	ePLL_CP_8,
	ePLL_CP_9,
	ePLL_CP_10,
	ePLL_CP_11,
	ePLL_CP_12,
	ePLL_CP_13,
	ePLL_CP_14,
	ePLL_CP_15,
} PLL_CP_t;

typedef enum
{
	eLVDSTX_ICP_20uA,
	eLVDSTX_ICP_35uA,
	eLVDSTX_ICP_50uA,
	eLVDSTX_ICP_65uA,
} LVDSTX_ICP_t;

typedef enum
{
	eLVDSTX_IDRIVER_888mV,
	eLVDSTX_IDRIVER_801mV,
	eLVDSTX_IDRIVER_711mV,
	eLVDSTX_IDRIVER_622mV,
	eLVDSTX_IDRIVER_532mV,
	eLVDSTX_IDRIVER_442mV,
	eLVDSTX_IDRIVER_353mV,
	eLVDSTX_IDRIVER_265mV,
} LVDSTX_IDRIVER_t;

typedef enum
{
	eTTL_DS_0_5mA = 0,
	eTTL_DS_1_0mA,
	eTTL_DS_2_0mA,
	eTTL_DS_4_0mA,
	eTTL_DS_8_0mA = 7,
	eTTL_DS_MAX = 7,
} LVDSTX_DS_t;

typedef enum
{
	eGPIO_DS_,

} GPIO_DS_t;

typedef enum
{
	eGPIO_MODE_FUNC,
	eGPIO_MODE_OUTPUT,
	eGPIO_MODE_INPUT,
} PANEL_GPIO_Mode_t;

typedef enum
{
	eDeviceID = 1,
	eHiZ,
	eConst,
	eRW,
	eAddr,
	eData,
	eCS,
	eHEADER_ID_MAX,
} PANEL_SPI_HEADER_ID_t;

typedef struct TCON_ManFck_s
{
	UINT8 bEnable	:4;		// 0=Disable, 1=XX Hz, 2=G1.26 & G1.27
	UINT8 bMethod	:4;		// 0=Integer, 1=Fractional, 2=Integer+MR
	UINT8 bMRatio;			// when bMethod == 2.	0~7
	UINT8 bMClk;			// when bMethod == 2.	0~7
	UINT8 bMClkX2;			// when bMethod == 2.	0~1
	UINT32 dwDclk1;
	UINT32 dwDclk2;
} TCON_ManFck_t;

typedef struct TCON_PAT_GAMMA_s
{
	UINT8 bPAT_Gamma_en;							//0:Disable, 1:Enable
	UINT8 bPAT_Gamma_sel;							//0:Merge 1:RGB
	UINT8 bPAT_Gamma_reverse[2];
	UINT16 bPAT_Gamma_table[3][GAMMA_LEN];
} TCON_PAT_GAMMA_t;

typedef struct stSPI_Format_s
{
	UINT8 header_id;
	UINT8 offset;
	UINT8 len;
	UINT8 increasing	:4;
	UINT8 argv			:4;
} PACKED stSPI_Format_t;

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
typedef struct stTCON_API_s
{
	//extra driver
	void (*SetPinMux_PWM1_Sel)				/* Hal_TCON_SET_PinMux_PWM1_Sel_EX			*/	(UINT8 Param);
	void (*GetPinMux_PWM1_Sel)				/* Hal_TCON_GET_PinMux_PWM1_Sel_EX			*/	(UINT8 *Param);
	void (*SetPinMux_PWM2_Sel)				/* Hal_TCON_SET_PinMux_PWM2_Sel_EX			*/	(UINT8 Param);
	void (*GetPinMux_PWM2_Sel)				/* Hal_TCON_GET_PinMux_PWM2_Sel_EX			*/	(UINT8 *Param);
	void (*SetPinMux_PWM3_Sel)				/* Hal_TCON_SET_PinMux_PWM3_Sel_EX			*/	(UINT8 Param);
	void (*GetPinMux_PWM3_Sel)				/* Hal_TCON_GET_PinMux_PWM3_Sel_EX			*/	(UINT8 *Param);
	void (*SetPinMux_PWM4_Sel)				/* Hal_TCON_SET_PinMux_PWM4_Sel_EX			*/	(UINT8 Param);
	void (*GetPinMux_PWM4_Sel)				/* Hal_TCON_GET_PinMux_PWM4_Sel_EX			*/	(UINT8 *Param);
	void (*SetPinMux_PWM5_Sel)				/* Hal_TCON_SET_PinMux_PWM5_Sel_EX			*/	(UINT8 Param);
	void (*GetPinMux_PWM5_Sel)				/* Hal_TCON_GET_PinMux_PWM5_Sel_EX			*/	(UINT8 *Param);
	void (*SetPinMux_PWM6_Sel)				/* Hal_TCON_SET_PinMux_PWM6_Sel_EX			*/	(UINT8 Param);
	void (*GetPinMux_PWM6_Sel)				/* Hal_TCON_GET_PinMux_PWM6_Sel_EX			*/	(UINT8 *Param);
	void (*SetPinMux_PWM7_Sel)				/* Hal_TCON_SET_PinMux_PWM7_Sel_EX			*/	(UINT8 Param);
	void (*GetPinMux_PWM7_Sel)				/* Hal_TCON_GET_PinMux_PWM7_Sel_EX			*/	(UINT8 *Param);
	void (*SetPinMux_PWM8_Sel)				/* Hal_TCON_SET_PinMux_PWM8_Sel_EX			*/	(UINT8 Param);
	void (*GetPinMux_PWM8_Sel)				/* Hal_TCON_GET_PinMux_PWM8_Sel_EX			*/	(UINT8 *Param);
	void (*SetPinMux_PCLK_Sel)				/* Hal_TCON_SET_PinMux_PCLK_Sel_EX			*/	(UINT8 Param);
	void (*GetPinMux_PCLK_Sel)				/* Hal_TCON_GET_PinMux_PCLK_Sel_EX			*/	(UINT8 *Param);
	void (*SetPinMux_RGB888_Sel)			/* Hal_TCON_SET_PinMux_RGB888_Sel_EX		*/	(UINT8 Param);
	void (*GetPinMux_RGB888_Sel)			/* Hal_TCON_GET_PinMux_RGB888_Sel_EX		*/	(UINT8 *Param);
	void (*SetPinMux_RGB666_Sel)			/* Hal_TCON_SET_PinMux_RGB666_Sel_EX		*/	(UINT8 Param);
	void (*GetPinMux_RGB666_Sel)			/* Hal_TCON_GET_PinMux_RGB666_Sel_EX		*/	(UINT8 *Param);
	void (*SetPinMux_TTL_Sel)				/* Hal_TCON_SET_PinMux_TTL_Sel_EX			*/	(UINT8 Param);
	void (*GetPinMux_TTL_Sel)				/* Hal_TCON_GET_PinMux_TTL_Sel_EX			*/	(UINT8 *Param);
	void (*SetPinMux_STP_Sel)				/* Hal_TCON_SET_PinMux_STP_Sel_EX			*/	(UINT8 Param);
	void (*GetPinMux_STP_Sel)				/* Hal_TCON_GET_PinMux_STP_Sel_EX			*/	(UINT8 *Param);
	void (*SetLVDSTX_CLK_SEL)				/* Hal_TCON_SET_LVDSTX_CLK_SEL_EX			*/	(UINT8 Param);
	void (*GetLVDSTX_CLK_SEL)				/* Hal_TCON_GET_LVDSTX_CLK_SEL_EX			*/	(UINT8 *Param);
	void (*SetLVDSTX_CPH1_Pol)				/* Hal_TCON_SET_LVDSTX_CPH1_Pol_EX			*/	(UINT8 Param);
	void (*GetLVDSTX_CPH1_Pol)				/* Hal_TCON_GET_LVDSTX_CPH1_Pol_EX			*/	(UINT8 *Param);
	void (*SetLVDSTX_Enable0)				/* Hal_TCON_SET_LVDSTX_Enable0_EX			*/	(UINT8 Param);
	void (*GetLVDSTX_Enable0)				/* Hal_TCON_GET_LVDSTX_Enable0_EX			*/	(UINT8 *Param);
	void (*SetLVDSTX_Enable1)				/* Hal_TCON_SET_LVDSTX_Enable1_EX			*/	(UINT8 Param);
	void (*GetLVDSTX_Enable1)				/* Hal_TCON_GET_LVDSTX_Enable1_EX			*/	(UINT8 *Param);
	void (*SetLVDSTX_RisingEdge)			/* Hal_TCON_SET_LVDSTX_RisingEdge_EX		*/	(UINT8 Param);
	void (*GetLVDSTX_RisingEdge)			/* Hal_TCON_GET_LVDSTX_RisingEdge_EX		*/	(UINT8 *Param);
	void (*SetLVDSTX_ChargePump)			/* Hal_TCON_SET_LVDSTX_ChargePump_EX		*/	(UINT8 Param);
	void (*GetLVDSTX_ChargePump)			/* Hal_TCON_GET_LVDSTX_ChargePump_EX		*/	(UINT8 *Param);
	void (*SetLVDSTX_iDriver)				/* Hal_TCON_SET_LVDSTX_iDriver_EX			*/	(UINT8 Param);
	void (*GetLVDSTX_iDriver)				/* Hal_TCON_GET_LVDSTX_iDriver_EX			*/	(UINT8 *Param);
	void (*SetTTL_Driver_1)					/* Hal_TCON_SET_TTL_Driver_1_EX				*/	(UINT8 Param);
	void (*GetTTL_Driver_1)					/* Hal_TCON_GET_TTL_Driver_1_EX				*/	(UINT8 *Param);
	void (*SetTTL_Driver_2)					/* Hal_TCON_SET_TTL_Driver_2_EX				*/	(UINT8 Param);
	void (*GetTTL_Driver_2)					/* Hal_TCON_GET_TTL_Driver_2_EX				*/	(UINT8 *Param);
	void (*SetTTL_Driver_3)					/* Hal_TCON_SET_TTL_Driver_3_EX				*/	(UINT8 Param);
	void (*GetTTL_Driver_3)					/* Hal_TCON_GET_TTL_Driver_3_EX				*/	(UINT8 *Param);
	void (*SetTTL_Driver_4)					/* Hal_TCON_SET_TTL_Driver_4_EX				*/	(UINT8 Param);
	void (*GetTTL_Driver_4)					/* Hal_TCON_GET_TTL_Driver_4_EX				*/	(UINT8 *Param);
	void (*SetLVDS_GPO_Mode)				/* Hal_TCON_SET_LVDS_GPO_Mode_EX			*/	(UINT8 Param);
	void (*GetLVDS_GPO_Mode)				/* Hal_TCON_GET_LVDS_GPO_Mode_EX			*/	(UINT8 *Param);
	void (*SetLVDS_GPO_Enable)				/* Hal_TCON_SET_LVDS_GPO_Enable_EX			*/	(UINT8 Param);
	void (*GetLVDS_GPO_Enable)				/* Hal_TCON_GET_LVDS_GPO_Enable_EX			*/	(UINT8 *Param);
	void (*SetLVDS_GPIO_OE)					/* Hal_TCON_SET_LVDS_GPIO_OE_EX				*/	(UINT8 Param);
	void (*GetLVDS_GPIO_OE)					/* Hal_TCON_GET_LVDS_GPIO_OE_EX				*/	(UINT8 *Param);
	void (*SetLVDS_GPIO_OEA)				/* Hal_TCON_SET_LVDS_GPIO_OEA_EX			*/	(UINT8 Param);
	void (*GetLVDS_GPIO_OEA)				/* Hal_TCON_GET_LVDS_GPIO_OEA_EX			*/	(UINT8 *Param);
	void (*SetUpdateGamma)					/* Hal_TCON_SET_UpdateGamma_EX				*/	(UINT8 RGB_SEL, UINT16 addr, UINT16 data, UINT8 debug);
	void (*GetUpdateGamma)					/* Hal_TCON_GET_UpdateGamma_EX				*/	(UINT8 RGB_SEL, UINT16 addr, UINT16 *data, UINT8 debug);
	void (*Set_Gamma_tbl)					/* Hal_TCON_SET_Gamma_tbl_EX				*/	(UINT16 *table, UINT8 RGB_SEL, UINT8 debug);
	void (*Get_Gamma_tbl)					/* Hal_TCON_GET_Gamma_tbl_EX				*/	(UINT16 *table, UINT8 RGB_SEL, UINT8 debug);
	void (*Set_PAT_Gamma)					/* Hal_TCON_SET_PATGamma_EX					*/	(char *raw_ptr, UINT32 size);
	void (*Get_PAT_Gamma)					/* Hal_TCON_GET_PATGamma_EX					*/	(char *raw_ptr, UINT32 *size);
	BOOL (*SetTCON_PLL_cfg)					/* Hal_TCON_SET_PLLDIS_cfg_EX				*/	(void);
	void (*GetTCON_PLL_cfg)					/* Hal_TCON_GET_PLLDIS_cfg_EX				*/	(UINT32 *freq, UINT32 *plldis_0, UINT32 *plldis_1);
	void (*GetTCON_NTSC_HVTotal)			/* Hal_TCON_GET_NTSC_HVTotal_EX				*/	(UINT16 *htotal, UINT16 *vtotal);
	void (*GetTCON_PAL_HVTotal)				/* Hal_TCON_GET_PAL_HVTotal_EX				*/	(UINT16 *htotal, UINT16 *vtotal);
	void (*SetTCON_HV_Area)					/* Hal_TCON_SET_HV_Area_EX					*/	(UINT16 htotal, UINT16 vtotal, UINT16 hactive, UINT16 vactive);
	void (*GetTCON_HV_Area)					/* Hal_TCON_GET_HV_Area_EX					*/	(UINT16 *htotal, UINT16 *vtotal, UINT16 *hactive, UINT16 *vactive);
	void (*SetTCON_H_Timing)				/* Hal_TCON_SET_H_Timing_EX					*/	(UINT16 sync_width, UINT16 bPorch);
	void (*GetTCON_H_Timing)				/* Hal_TCON_GET_H_Timing_EX					*/	(UINT16 *sync_width, UINT16 *bPorch);
	void (*SetTCON_V_Timing)				/* Hal_TCON_SET_V_Timing_EX					*/	(UINT16 sync_width, UINT16 bPorch);
	void (*GetTCON_V_Timing)				/* Hal_TCON_GET_V_Timing_EX					*/	(UINT16 *sync_width, UINT16 *bPorch);
	void (*SetPANEL_FPS)					/* Hal_TCON_SET_PANEL_FPS_EX				*/	(TCON_CLK_t fps);
	void (*GetPANEL_FPS)					/* Hal_TCON_GET_PANEL_FPS_EX				*/	(TCON_CLK_t *fps);
	void (*SetPANEL_Factor)					/* Hal_TCON_SET_PANEL_Factor_EX				*/	(UINT8 Param);
	void (*GetPANEL_Factor)					/* Hal_TCON_GET_PANEL_Factor_EX				*/	(UINT8 *Param);
	void (*SetPanelInterface_Vars)			/* Hal_TCON_SET_Panel_Interface_Vars_EX		*/	(UINT8 Param);
	void (*GetPanelInterface_Vars)			/* Hal_TCON_GET_Panel_Interface_Vars_EX		*/	(UINT8 *Param);
	void (*SetSyncMode_Vars)				/* Hal_TCON_SET_SyncMode_Vars_EX			*/	(UINT8 Param);
	void (*GetSyncMode_Vars)				/* Hal_TCON_GET_SyncMode_Vars_EX			*/	(UINT8 *Param);
	void (*SetManualFckoutEn_Vars)			/* Hal_TCON_SET_ManualFckoutEn_Vars_EX		*/	(UINT8 fmt, UINT8 Param, UINT32 dclk1, UINT32 dclk2);
	void (*GetManualFckoutEn_Vars)			/* Hal_TCON_GET_ManualFckoutEn_Vars_EX		*/	(UINT8 fmt, UINT8 *Param, UINT32 *dclk1, UINT32 *dclk2);
	void (*SetManualFckMethod_Vars)			/* Hal_TCON_SET_ManualFckMethod_Vars_EX		*/	(UINT8 fmt, UINT8 method, UINT8 mr, UINT8 mclk, UINT8 mclkx2);
	void (*GetManualFckMethod_Vars)			/* Hal_TCON_GET_ManualFckMethod_Vars_EX		*/	(UINT8 fmt, UINT8 *method, UINT8 *mr, UINT8 *mclk, UINT8 *mclkx2);
	void (*SetLVDSOutputMode_Vars)			/* Hal_TCON_SET_LVDS_OutputMode_Vars_EX		*/	(UINT8 Param);
	void (*GetLVDSOutputMode_Vars)			/* Hal_TCON_GET_LVDS_OutputMode_Vars_EX		*/	(UINT8 *Param);
	void (*SetPanelPitch_Vars)				/* Hal_TCON_SET_Panel_Pitch_Vars_EX			*/	(UINT16 h_pitch, UINT16 v_pitch);
	void (*GetPanelPitch_Vars)				/* Hal_TCON_GET_Panel_Pitch_Vars_EX			*/	(UINT16 *h_pitch, UINT16 *v_pitch);
	void (*SetPLL_CP_Vars)					/* Hal_TCON_SET_PLL_CP_Vars_EX				*/	(UINT8 enable, UINT8 pll_cp);
	void (*GetPLL_CP_Vars)					/* Hal_TCON_GET_PLL_CP_Vars_EX				*/	(UINT8 *enable, UINT8 *pll_cp);
	void (*SetBackLight_OnOff_Vars)			/* Hal_TCON_SET_BackLight_OnOff_Vars_Ex		*/	(void (*pBackLight_OnOff)(BOOL));
	void (*SetBackLight_OnOff)				/* Hal_TCON_SET_BackLight_OnOff_Ex			*/	(UINT8 Param);
	void (*SetDisplaySize_FunPtr)			/* Hal_TCON_SET_DisplaySize_FunPtr_Ex		*/	(void (*pDisplaySize)(void));
	void (*SetDisplaySize)					/* Hal_TCON_SET_DisplaySize_Ex				*/	(void);
	void (*SetTimgingReGen_FunPtr)			/* Hal_TCON_SET_TimingReGen_FunPtr_Ex		*/	(void (*pTimingReGen)(void));
	void (*SetTimgingReGen)					/* Hal_TCON_SET_TimingReGen_Ex				*/	(void);
	void (*SetTXmode_Vars)					/* Hal_TCON_SET_TXmode_Vars_EX				*/	(UINT8 Param);
	void (*GetTXmode_Vars)					/* Hal_TCON_GET_TXmode_Vars_EX				*/	(UINT8 *Param);
	void (*SetFixHPorch_Vars)				/* Hal_TCON_SET_FixHPorch_Vars_EX			*/	(UINT8 Param);
	void (*GetFixHPorch_Vars)				/* Hal_TCON_GET_FixHPorch_Vars_EX			*/	(UINT8 *Param);
	void (*SetFixVPorch_Vars)				/* Hal_TCON_SET_FixVPorch_Vars_EX			*/	(UINT8 Param);
	void (*GetFixVPorch_Vars)				/* Hal_TCON_GET_FixVPorch_Vars_EX			*/	(UINT8 *Param);
	void (*SetFixVTotal_Vars)				/* Hal_TCON_SET_FixVTotal_Vars_EX			*/	(UINT8 Param);
	void (*GetFixVTotal_Vars)				/* Hal_TCON_GET_FixVTotal_Vars_EX			*/	(UINT8 *Param);
	UINT32 (*CalcDisFreq)					/* Hal_TCON_CalcFreq_EX						*/	(UINT32 dclk1, UINT32 dclk2);
	void (*GetDMIX_V_BP)					/* Hal_TCON_GET_DMIX_BP_EX					*/	(UINT16 *bp);
	void (*Report)							/* Hal_TCON_Report_Ex						*/	(void);
	void (*Set_Scene_Gamma_Table)			/* Hal_Set_Scene_Gamma_Table			    */	(char *raw_ptr, UINT32 size);

	void (*Dummy_01)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_02)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_03)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_04)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_05)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_06)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_07)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_08)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_09)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_10)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_11)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_12)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_13)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_14)						/* Hal_TCON_dummy							*/	(void);

	//SPI
	void (*SPI_Read)						/* Hal_TCON_SPI_Read						*/	(UINT16 addr, UINT16 *data);
	void (*SPI_Write)						/* Hal_TCON_SPI_Write						*/	(UINT16 addr, UINT16 data, UINT8 debug);
	void (*SPI_Report)						/* Hal_TCON_SPI_Report						*/	(void);
	void (*SPI_init)						/* Hal_TCON_SPI_init						*/	(void);
	void (*Dummy_601)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_602)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_603)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_604)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_605)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_606)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_607)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_608)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_609)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_610)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_611)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_612)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_613)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_614)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_615)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_616)						/* Hal_TCON_dummy							*/	(void);

	//register drvier
	void (*SetRGB888)						/* Hal_TCON_SET_RGB888						*/	(BOOL Param);
	void (*GetRGB888)						/* Hal_TCON_GET_RGB888						*/	(BOOL *Param);
	void (*SetDOT3BitRev)					/* Hal_TCON_SET_DOT3_BitReverse				*/	(BOOL Param);
	void (*GetDOT3BitRev)					/* Hal_TCON_GET_DOT3_BitReverse				*/	(BOOL *Param);
	void (*SetDOT2BitRev)					/* Hal_TCON_SET_DOT2_BitReverse				*/	(BOOL Param);
	void (*GetDOT2BitRev)					/* Hal_TCON_GET_DOT2_BitReverse				*/	(BOOL *Param);
	void (*SetDOT1BitRev)					/* Hal_TCON_SET_DOT1_BitReverse				*/	(BOOL Param);
	void (*GetDOT1BitRev)					/* Hal_TCON_GET_DOT1_BitReverse				*/	(BOOL *Param);
	void (*SetRGBOrder)						/* Hal_TCON_SET_RGBOrder					*/	(UINT8 Param);
	void (*GetRGBOrder)						/* Hal_TCON_GET_RGBOrder					*/	(UINT8 *Param);
	void (*SetRGBData_Inverse)				/* Hal_TCON_SET_RGBData_Inverse				*/	(BOOL Param);
	void (*GetRGBData_Inverse)				/* Hal_TCON_GET_RGBData_Inverse				*/	(BOOL *Param);
	void (*SetRGB_SampleOut_Enable)			/* Hal_TCON_SET_RGB_SampleOut_Enable		*/	(BOOL Param);
	void (*GetRGB_SampleOut_Enable)			/* Hal_TCON_GET_RGB_SampleOut_Enable		*/	(BOOL *Param);
	void (*SetHVD_IF_Enable)				/* Hal_TCON_SET_HVD_IF_Enable				*/	(BOOL Param);
	void (*GetHVD_IF_Enable)				/* Hal_TCON_GET_HVD_IF_Enable				*/	(BOOL *Param);
	void (*SetTconModule_Enable)			/* Hal_TCON_SET_TconModule_Enable			*/	(BOOL Param);
	void (*GetTconModule_Enable)			/* Hal_TCON_GET_TconModule_Enable			*/	(BOOL *Param);
	void (*SetINV_Mode_Pol)					/* Hal_TCON_SET_INV_Mode_Pol				*/	(BOOL Param);
	void (*GetINV_Mode_Pol)					/* Hal_TCON_GET_INV_Mode_Pol				*/	(BOOL *Param);
	void (*SetINV_Mode)						/* Hal_TCON_SET_INV_Mode					*/	(UINT8 Param);
	void (*GetINV_Mode)						/* Hal_TCON_GET_INV_Mode					*/	(UINT8 *Param);
	void (*SetQ1H_INV_Enable)				/* Hal_TCON_SET_Q1H_INV_Enable				*/	(BOOL Param);
	void (*GetQ1H_INV_Enable)				/* Hal_TCON_GET_Q1H_INV_Enable				*/	(BOOL *Param);
	void (*SetDE_MANUAL_Enable)				/* Hal_TCON_SET_DE_MANUAL_Enable			*/	(BOOL Param);
	void (*GetDE_MANUAL_Enable)				/* Hal_TCON_GET_DE_MANUAL_Enable			*/	(BOOL *Param);
	void (*SetDithering_Mode)				/* Hal_TCON_SET_Dithering_Mode				*/	(UINT8 Param);
	void (*GetDithering_Mode)				/* Hal_TCON_GET_Dithering_Mode				*/	(UINT8 *Param);
	void (*SetVMode_Enable)					/* Hal_TCON_SET_VMode_Enable				*/	(BOOL Param);
	void (*GetVMode_Enable)					/* Hal_TCON_GET_VMode_Enable				*/	(BOOL *Param);
	void (*SetActLine_MANUAL_Enable)		/* Hal_TCON_SET_ActLine_MANUAL_Enable		*/	(BOOL Param);
	void (*GetActLine_MANUAL_Enable)		/* Hal_TCON_GET_ActLine_MANUAL_Enable		*/	(BOOL *Param);
	void (*SetSTVU_MANUAL_Enable)			/* Hal_TCON_SET_STVU_MANUAL_Enable			*/	(BOOL Param);
	void (*GetSTVU_MANUAL_Enable)			/* Hal_TCON_GET_STVU_MANUAL_Enable			*/	(BOOL *Param);
	void (*SetTB_VCOM_Pol)					/* Hal_TCON_SET_TB_VCOM_Pol					*/	(BOOL top_pol, BOOL bot_pol);
	void (*GetTB_VCOM_Pol)					/* Hal_TCON_GET_TB_VCOM_Pol					*/	(BOOL *top_pol, BOOL *bot_pol);
	void (*SetVCOM_MANUAL_Enable)			/* Hal_TCON_SET_VCOM_MANUAL_Enable			*/	(BOOL Param);
	void (*GetVCOM_MANUAL_Enable)			/* Hal_TCON_GET_VCOM_MANUAL_Enable			*/	(BOOL *Param);
	void (*SetSTHLR_DLY)					/* Hal_TCON_SET_STHLR_DLY					*/	(UINT8 Param);
	void (*GetSTHLR_DLY)					/* Hal_TCON_GET_STHLR_DLY					*/	(UINT8 *Param);
	void (*SetQ1H_Enable)					/* Hal_TCON_SET_Q1H_Enable					*/	(BOOL Param);
	void (*GetQ1H_Enable)					/* Hal_TCON_GET_Q1H_Enable					*/	(BOOL *Param);
	void (*SetCPH1_Pol)						/* Hal_TCON_SET_CPH1_Pol					*/	(BOOL Param);
	void (*GetCPH1_Pol)						/* Hal_TCON_GET_CPH1_Pol					*/	(BOOL *Param);
	void (*SetDigital_RGB_Output)			/* Hal_TCON_SET_Digital_RGB_Output			*/	(BOOL Param);
	void (*GetDigital_RGB_Output)			/* Hal_TCON_GET_Digital_RGB_Output			*/	(BOOL *Param);
	void (*SetQ1H_BOT_Pol)					/* Hal_TCON_SET_Q1H_BOT_Pol					*/	(BOOL Param);
	void (*GetQ1H_BOT_Pol)					/* Hal_TCON_GET_Q1H_BOT_Pol					*/	(BOOL *Param);
	void (*SetQ1H_TOP_Pol)					/* Hal_TCON_SET_Q1H_TOP_Pol					*/	(BOOL Param);
	void (*GetQ1H_TOP_Pol)					/* Hal_TCON_GET_Q1H_TOP_Pol					*/	(BOOL *Param);
	void (*SetSTHLR_Pol)					/* Hal_TCON_SET_STHLR_Pol					*/	(BOOL Param);
	void (*GetSTHLR_Pol)					/* Hal_TCON_GET_STHLR_Pol					*/	(BOOL *Param);
	void (*SetSTVU_Pol)						/* Hal_TCON_SET_STVU_Pol					*/	(BOOL Param);
	void (*GetSTVU_Pol)						/* Hal_TCON_GET_STVU_Pol					*/	(BOOL *Param);
	void (*SetCKV_Pol)						/* Hal_TCON_SET_CKV_Pol						*/	(BOOL Param);
	void (*GetCKV_Pol)						/* Hal_TCON_GET_CKV_Pol						*/	(BOOL *Param);
	void (*SetOEV_Pol)						/* Hal_TCON_SET_OEV_Pol						*/	(BOOL Param);
	void (*GetOEV_Pol)						/* Hal_TCON_GET_OEV_Pol						*/	(BOOL *Param);
	void (*SetOEH_Pol)						/* Hal_TCON_SET_OEH_Pol						*/	(BOOL Param);
	void (*GetOEH_Pol)						/* Hal_TCON_GET_OEH_Pol						*/	(BOOL *Param);
	void (*SetVCOM_Pol)						/* Hal_TCON_SET_VCOM_Pol					*/	(BOOL Param);
	void (*GetVCOM_Pol)						/* Hal_TCON_GET_VCOM_Pol					*/	(BOOL *Param);
	void (*SetActiveArea)					/* Hal_TCON_SET_ActiveArea					*/	(UINT16 top_start, UINT16 bot_start, UINT16 top_end, UINT16 bot_end);
	void (*GetActiveArea)					/* Hal_TCON_GET_ActiveArea					*/	(UINT16 *top_start, UINT16 *bot_start, UINT16 *top_end, UINT16 *bot_end);
	void (*SetSTV_Startline)				/* Hal_TCON_SET_STV_Startline				*/	(UINT16 top_start, UINT16 bot_start);
	void (*GetSTV_Startline)				/* Hal_TCON_GET_STV_Startline				*/	(UINT16 *top_start, UINT16 *bot_start);
	void (*SetOEH_Position)					/* Hal_TCON_SET_OEH_Position				*/	(UINT16 start, UINT16 end);
	void (*GetOEH_Position)					/* Hal_TCON_GET_OEH_Position				*/	(UINT16 *start, UINT16 *end);
	void (*SetOEV_Position)					/* Hal_TCON_SET_OEV_Position				*/	(UINT16 start, UINT16 end);
	void (*GetOEV_Position)					/* Hal_TCON_GET_OEV_Position				*/	(UINT16 *start, UINT16 *end);
	void (*SetQ1H_Position)					/* Hal_TCON_SET_Q1H_Position				*/	(UINT16 start, UINT16 end);
	void (*GetQ1H_Position)					/* Hal_TCON_GET_Q1H_Position				*/	(UINT16 *start, UINT16 *end);
	void (*SetVCOM_Position)				/* Hal_TCON_SET_VCOM_Position				*/	(UINT16 start, UINT16 end);
	void (*GetVCOM_Position)				/* Hal_TCON_GET_VCOM_Position				*/	(UINT16 *start, UINT16 *end);
	void (*SetCKV_Position)					/* Hal_TCON_SET_CKV_Position				*/	(UINT16 start, UINT16 end);
	void (*GetCKV_Position)					/* Hal_TCON_GET_CKV_Position				*/	(UINT16 *start, UINT16 *end);
	void (*SetSTV_Position)					/* Hal_TCON_SET_STV_Position				*/	(UINT16 start, UINT16 end);
	void (*GetSTV_Position)					/* Hal_TCON_GET_STV_Position				*/	(UINT16 *start, UINT16 *end);
	void (*SetSTH_Position)					/* Hal_TCON_SET_STH_Position				*/	(UINT16 start, UINT16 end);
	void (*GetSTH_Position)					/* Hal_TCON_GET_STH_Position				*/	(UINT16 *start, UINT16 *end);
	void (*SetHSync_Position)				/* Hal_TCON_SET_HSync_Position				*/	(UINT16 start, UINT16 end);
	void (*GetHSync_Position)				/* Hal_TCON_GET_HSync_Position				*/	(UINT16 *start, UINT16 *end);
	void (*SetDE_H_Position)				/* Hal_TCON_SET_DE_H_Position				*/	(UINT16 start, UINT16 end);
	void (*GetDE_H_Position)				/* Hal_TCON_GET_DE_H_Position				*/	(UINT16 *start, UINT16 *end);
	void (*SetDE_MANUAL_Setting)			/* Hal_TCON_SET_DE_MANUAL_Setting			*/	(UINT16 top_de_v_start, UINT16 top_de_v_end, UINT16 bot_de_v_start, UINT16 bot_de_v_end);
	void (*GetDE_MANUAL_Setting)			/* Hal_TCON_GET_DE_MANUAL_Setting			*/	(UINT16 *top_de_v_start, UINT16 *top_de_v_end, UINT16 *bot_de_v_start, UINT16 *bot_de_v_end);
	void (*SetGammaUPD_Scheme)				/* Hal_TCON_SET_GammaUPD_Scheme				*/	(BOOL Param);
	void (*GetGammaUPD_Scheme)				/* Hal_TCON_GET_GammaUPD_Scheme				*/	(BOOL *Param);
	void (*SetDeltaPnl_RGBOrder)			/* Hal_TCON_SET_DeltaPnl_RGBOrder			*/	(UINT8 rgb_even, UINT8 rgb_odd);
	void (*GetDeltaPnl_RGBOrder)			/* Hal_TCON_GET_DeltaPnl_RGBOrder			*/	(UINT8 *rgb_even, UINT8 *rgb_odd);
	void (*SetDeltaPnl_Enable)				/* Hal_TCON_SET_DeltaPnl_Enable				*/	(BOOL Param);
	void (*GetDeltaPnl_Enable)				/* Hal_TCON_GET_DeltaPnl_Enable				*/	(BOOL *Param);
	void (*SetGamma_RW_Mode)				/* Hal_TCON_SET_Gamma_RW_Mode				*/	(BOOL Param);
	void (*GetGamma_RW_Mode)				/* Hal_TCON_GET_Gamma_RW_Mode				*/	(BOOL *Param);
	void (*SetGammaRGB_Sel)					/* Hal_TCON_SET_GammaRGB_Sel				*/	(UINT8 Param);
	void (*GetGammaRGB_Sel)					/* Hal_TCON_GET_GammaRGB_Sel				*/	(UINT8 *Param);
	void (*SetGamma_Update_Enable)			/* Hal_TCON_SET_Gamma_Update_Enable			*/	(BOOL Param);
	void (*SetGamma_Enable)					/* Hal_TCON_SET_Gamma_Enable				*/	(BOOL Param);
	void (*GetGamma_Enable)					/* Hal_TCON_GET_Gamma_Enable				*/	(BOOL *Param);
	void (*SetVCOUNT_RST_Line)				/* Hal_TCON_SET_VCOUNT_RST_Line				*/	(UINT16 top, UINT16 bot);
	void (*GetVCOUNT_RST_Line)				/* Hal_TCON_GET_VCOUNT_RST_Line				*/	(UINT16 *top, UINT16 *bot);
	void (*SetTPG_Pattern)					/* Hal_TCON_SET_TPG_Pattern					*/	(UINT8 Param, UINT8 hstep, UINT8 vstep);
	void (*SetTPG_Mode)						/* Hal_TCON_SET_TPG_Mode					*/	(UINT8 Param);
	void (*SetTPG_Area)						/* Hal_TCON_SET_TPG_Area					*/	(UINT16 hcount, UINT16 vcount, UINT16 hactive, UINT16 vactive);
	void (*GetTPG_Area)						/* Hal_TCON_GET_TPG_Area					*/	(UINT16 *hcount, UINT16 *vcount, UINT16 *hactive, UINT16 *vactive);
	void (*SetSRGB_IF_Enable)				/* Hal_TCON_SET_SRGB_IF_Enable				*/	(BOOL Param);
	void (*GetSRGB_IF_Enable)				/* Hal_TCON_GET_SRGB_IF_Enable				*/	(BOOL *Param);
	void (*SetVCOM_2H_Pol_Enable)			/* Hal_TCON_SET_VCOM_2H_Pol_Enable			*/	(BOOL Param);
	void (*GetVCOM_2H_Pol_Enable)			/* Hal_TCON_GET_VCOM_2H_Pol_Enable			*/	(BOOL *Param);
	void (*SetSRGB_VS_Pol)					/* Hal_TCON_SET_SRGB_VS_Pol					*/	(BOOL Param);
	void (*GetSRGB_VS_Pol)					/* Hal_TCON_GET_SRGB_VS_Pol					*/	(BOOL *Param);
	void (*SetSRGB_HS_Pol)					/* Hal_TCON_SET_SRGB_HS_Pol					*/	(BOOL Param);
	void (*GetSRGB_HS_Pol)					/* Hal_TCON_GET_SRGB_HS_Pol					*/	(BOOL *Param);
	void (*SetSRGB_VS_DLY)					/* Hal_TCON_SET_SRGB_VS_DLY					*/	(UINT8 Param);
	void (*GetSRGB_VS_DLY)					/* Hal_TCON_GET_SRGB_VS_DLY					*/	(UINT8 *Param);
	void (*SetSRGB_HS_DLY)					/* Hal_TCON_SET_SRGB_HS_DLY					*/	(UINT8 Param);
	void (*GetSRGB_HS_DLY)					/* Hal_TCON_GET_SRGB_HS_DLY					*/	(UINT8 *Param);
	void (*SetSRGB_Sync_Mode)				/* Hal_TCON_SET_SRGB_Sync_Mode				*/	(BOOL Param);
	void (*GetSRGB_Sync_Mode)				/* Hal_TCON_GET_SRGB_Sync_Mode				*/	(BOOL *Param);
	void (*SetPixEn_rate)					/* Hal_TCON_SET_PixEn_rate					*/	(UINT8 Param);
	void (*GetPixEn_rate)					/* Hal_TCON_GET_PixEn_rate					*/	(UINT8 *Param);
	void (*SetTPG_WhiteBG)					/* Hal_TCON_SET_TPG_WhiteBG					*/	(BOOL Param);
	void (*SetTPG_B_Data)					/* Hal_TCON_SET_TPG_B_Data					*/	(UINT8 Param);
	void (*SetTPG_G_Data)					/* Hal_TCON_SET_TPG_G_Data					*/	(UINT8 Param);
	void (*SetTPG_R_Data)					/* Hal_TCON_SET_TPG_R_Data					*/	(UINT8 Param);
	void (*SetDValid_H_Position)			/* Hal_TCON_SET_DValid_H_Position			*/	(UINT16 start, UINT16 end);
	void (*GetDValid_H_Position)			/* Hal_TCON_GET_DValid_H_Position			*/	(UINT16 *start, UINT16 *end);
	void (*SetNew_Dither_Enable)			/* Hal_TCON_SET_New_Dither_Enable			*/	(BOOL Param);
	void (*GetNew_Dither_Enable)			/* Hal_TCON_GET_New_Dither_Enable			*/	(BOOL *Param);
	void (*SetNew_Dither_TempMode_Enable)	/* Hal_TCON_SET_New_Dither_TempMode_Enable	*/	(BOOL Param);
	void (*GetNew_Dither_TempMode_Enable)	/* Hal_TCON_GET_New_Dither_TempMode_Enable	*/	(BOOL *Param);
	void (*SetNew_Dither_InitMode)			/* Hal_TCON_SET_New_Dither_InitMode			*/	(BOOL Param);
	void (*GetNew_Dither_InitMode)			/* Hal_TCON_GET_New_Dither_InitMode			*/	(BOOL *Param);
	void (*SetNew_Dither_RGBComp_Sel)		/* Hal_TCON_SET_New_Dither_RGBComp_Sel		*/	(BOOL Param);
	void (*GetNew_Dither_RGBComp_Sel)		/* Hal_TCON_GET_New_Dither_RGBComp_Sel		*/	(BOOL *Param);
	void (*SetColor_Space_Sel)				/* Hal_TCON_SET_Color_Space_Sel				*/	(BOOL Param);
	void (*GetColor_Space_Sel)				/* Hal_TCON_GET_Color_Space_Sel				*/	(BOOL *Param);
	void (*SetGamma_Addr)					/* Hal_TCON_SET_Gamma_Addr					*/	(UINT16 Param);
	void (*GetGamma_Addr)					/* Hal_TCON_GET_Gamma_Addr					*/	(UINT16 *Param);
	void (*SetGamma_Data)					/* Hal_TCON_SET_Gamma_Data					*/	(UINT16 Param);
	void (*GetGamma_Data)					/* Hal_TCON_GET_Gamma_Data					*/	(UINT16 *Param);
	void (*Dummy_501)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_502)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_503)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_504)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_505)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_506)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_507)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_508)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_509)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_500)						/* Hal_TCON_dummy							*/	(void);

	void (*Set_BitSwap_Enable)				/* Hal_TCON_SET_BitSwap_Enable				*/	(UINT8 Param);
	void (*Get_BitSwap_Enable)				/* Hal_TCON_GET_BitSwap_Enable				*/	(UINT8 *Param);
	void (*Set_BitSwap_B2)					/* Hal_TCON_SET_BitSwap_B2					*/	(UINT8 Param);
	void (*Get_BitSwap_B2)					/* Hal_TCON_GET_BitSwap_B2					*/	(UINT8 *Param);
	void (*Set_BitSwap_B1)					/* Hal_TCON_SET_BitSwap_B1					*/	(UINT8 Param);
	void (*Get_BitSwap_B1)					/* Hal_TCON_GET_BitSwap_B1					*/	(UINT8 *Param);
	void (*Set_BitSwap_B0)					/* Hal_TCON_SET_BitSwap_B0					*/	(UINT8 Param);
	void (*Get_BitSwap_B0)					/* Hal_TCON_GET_BitSwap_B0					*/	(UINT8 *Param);
	void (*Set_BitSwap_B5)					/* Hal_TCON_SET_BitSwap_B5					*/	(UINT8 Param);
	void (*Get_BitSwap_B5)					/* Hal_TCON_GET_BitSwap_B5					*/	(UINT8 *Param);
	void (*Set_BitSwap_B4)					/* Hal_TCON_SET_BitSwap_B4					*/	(UINT8 Param);
	void (*Get_BitSwap_B4)					/* Hal_TCON_GET_BitSwap_B4					*/	(UINT8 *Param);
	void (*Set_BitSwap_B3)					/* Hal_TCON_SET_BitSwap_B3					*/	(UINT8 Param);
	void (*Get_BitSwap_B3)					/* Hal_TCON_GET_BitSwap_B3					*/	(UINT8 *Param);
	void (*Set_BitSwap_B8)					/* Hal_TCON_SET_BitSwap_B8					*/	(UINT8 Param);
	void (*Get_BitSwap_B8)					/* Hal_TCON_GET_BitSwap_B8					*/	(UINT8 *Param);
	void (*Set_BitSwap_B7)					/* Hal_TCON_SET_BitSwap_B7					*/	(UINT8 Param);
	void (*Get_BitSwap_B7)					/* Hal_TCON_GET_BitSwap_B7					*/	(UINT8 *Param);
	void (*Set_BitSwap_B6)					/* Hal_TCON_SET_BitSwap_B6					*/	(UINT8 Param);
	void (*Get_BitSwap_B6)					/* Hal_TCON_GET_BitSwap_B6					*/	(UINT8 *Param);
	void (*Set_BitSwap_B11)					/* Hal_TCON_SET_BitSwap_B11					*/	(UINT8 Param);
	void (*Get_BitSwap_B11)					/* Hal_TCON_GET_BitSwap_B11					*/	(UINT8 *Param);
	void (*Set_BitSwap_B10)					/* Hal_TCON_SET_BitSwap_B10					*/	(UINT8 Param);
	void (*Get_BitSwap_B10)					/* Hal_TCON_GET_BitSwap_B10					*/	(UINT8 *Param);
	void (*Set_BitSwap_B9)					/* Hal_TCON_SET_BitSwap_B9					*/	(UINT8 Param);
	void (*Get_BitSwap_B9)					/* Hal_TCON_GET_BitSwap_B9					*/	(UINT8 *Param);
	void (*Set_BitSwap_B14)					/* Hal_TCON_SET_BitSwap_B14					*/	(UINT8 Param);
	void (*Get_BitSwap_B14)					/* Hal_TCON_GET_BitSwap_B14					*/	(UINT8 *Param);
	void (*Set_BitSwap_B13)					/* Hal_TCON_SET_BitSwap_B13					*/	(UINT8 Param);
	void (*Get_BitSwap_B13)					/* Hal_TCON_GET_BitSwap_B13					*/	(UINT8 *Param);
	void (*Set_BitSwap_B12)					/* Hal_TCON_SET_BitSwap_B12					*/	(UINT8 Param);
	void (*Get_BitSwap_B12)					/* Hal_TCON_GET_BitSwap_B12					*/	(UINT8 *Param);
	void (*Set_BitSwap_B17)					/* Hal_TCON_SET_BitSwap_B17					*/	(UINT8 Param);
	void (*Get_BitSwap_B17)					/* Hal_TCON_GET_BitSwap_B17					*/	(UINT8 *Param);
	void (*Set_BitSwap_B16)					/* Hal_TCON_SET_BitSwap_B16					*/	(UINT8 Param);
	void (*Get_BitSwap_B16)					/* Hal_TCON_GET_BitSwap_B16					*/	(UINT8 *Param);
	void (*Set_BitSwap_B15)					/* Hal_TCON_SET_BitSwap_B15					*/	(UINT8 Param);
	void (*Get_BitSwap_B15)					/* Hal_TCON_GET_BitSwap_B15					*/	(UINT8 *Param);
	void (*Set_BitSwap_B20)					/* Hal_TCON_SET_BitSwap_B20					*/	(UINT8 Param);
	void (*Get_BitSwap_B20)					/* Hal_TCON_GET_BitSwap_B20					*/	(UINT8 *Param);
	void (*Set_BitSwap_B19)					/* Hal_TCON_SET_BitSwap_B19					*/	(UINT8 Param);
	void (*Get_BitSwap_B19)					/* Hal_TCON_GET_BitSwap_B19					*/	(UINT8 *Param);
	void (*Set_BitSwap_B18)					/* Hal_TCON_SET_BitSwap_B18					*/	(UINT8 Param);
	void (*Get_BitSwap_B18)					/* Hal_TCON_GET_BitSwap_B18					*/	(UINT8 *Param);
	void (*Set_BitSwap_B23)					/* Hal_TCON_SET_BitSwap_B23					*/	(UINT8 Param);
	void (*Get_BitSwap_B23)					/* Hal_TCON_GET_BitSwap_B23					*/	(UINT8 *Param);
	void (*Set_BitSwap_B22)					/* Hal_TCON_SET_BitSwap_B22					*/	(UINT8 Param);
	void (*Get_BitSwap_B22)					/* Hal_TCON_GET_BitSwap_B22					*/	(UINT8 *Param);
	void (*Set_BitSwap_B21)					/* Hal_TCON_SET_BitSwap_B21					*/	(UINT8 Param);
	void (*Get_BitSwap_B21)					/* Hal_TCON_GET_BitSwap_B21					*/	(UINT8 *Param);
	void (*Dummy_101)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_102)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_103)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_104)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_105)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_106)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_107)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_108)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_109)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_100)						/* Hal_TCON_dummy							*/	(void);

	void (*Set_PWM_Mode_Ctl)				/* Hal_TCON_SET_PWM_Mode_Ctl				*/	(TCON_PWM_t num, UINT8 enable, UINT8 bypass);
	void (*Get_PWM_Mode_Ctl)				/* Hal_TCON_GET_PWM_Mode_Ctl				*/	(TCON_PWM_t num, UINT8 *enable, UINT8 *bypass);
	void (*Set_PWM_Clk54M_Enable)			/* Hal_TCON_SET_PWM_Clk54M_Enable			*/	(UINT8 Param);
	void (*Get_PWM_Clk54M_Enable)			/* Hal_TCON_GET_PWM_Clk54M_Enable			*/	(UINT8 *Param);
	void (*Set_PWM_DD_Mode)					/* Hal_TCON_SET_PWM_DD_Mode					*/	(TCON_PWM_DD_t num, UINT8 enable, UINT16 dd);
	void (*Get_PWM_DD_Mode)					/* Hal_TCON_GET_PWM_DD_Mode					*/	(TCON_PWM_DD_t num, UINT8 *enable, UINT16 *dd);
	void (*Set_PWM_Duty)					/* Hal_TCON_SET_PWM_Duty					*/	(TCON_PWM_t num, UINT8 duty, TCON_PWM_DD_t dd_sel);
	void (*Get_PWM_Duty)					/* Hal_TCON_GET_PWM_Duty					*/	(TCON_PWM_t num, UINT8 *duty, TCON_PWM_DD_t *dd_sel);
	void (*Dummy_201)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_202)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_203)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_204)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_205)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_206)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_207)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_208)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_209)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_200)						/* Hal_TCON_dummy							*/	(void);

	void (*SetLVDS_R0_Sel)					/* Hal_TCON_SET_LVDS_R0_Sel					*/	(UINT8 Param);
	void (*GetLVDS_R0_Sel)					/* Hal_TCON_GET_LVDS_R0_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_R1_Sel)					/* Hal_TCON_SET_LVDS_R1_Sel					*/	(UINT8 Param);
	void (*GetLVDS_R1_Sel)					/* Hal_TCON_GET_LVDS_R1_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_R2_Sel)					/* Hal_TCON_SET_LVDS_R2_Sel					*/	(UINT8 Param);
	void (*GetLVDS_R2_Sel)					/* Hal_TCON_GET_LVDS_R2_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_R3_Sel)					/* Hal_TCON_SET_LVDS_R3_Sel					*/	(UINT8 Param);
	void (*GetLVDS_R3_Sel)					/* Hal_TCON_GET_LVDS_R3_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_R4_Sel)					/* Hal_TCON_SET_LVDS_R4_Sel					*/	(UINT8 Param);
	void (*GetLVDS_R4_Sel)					/* Hal_TCON_GET_LVDS_R4_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_R5_Sel)					/* Hal_TCON_SET_LVDS_R5_Sel					*/	(UINT8 Param);
	void (*GetLVDS_R5_Sel)					/* Hal_TCON_GET_LVDS_R5_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_R6_Sel)					/* Hal_TCON_SET_LVDS_R6_Sel					*/	(UINT8 Param);
	void (*GetLVDS_R6_Sel)					/* Hal_TCON_GET_LVDS_R6_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_R7_Sel)					/* Hal_TCON_SET_LVDS_R7_Sel					*/	(UINT8 Param);
	void (*GetLVDS_R7_Sel)					/* Hal_TCON_GET_LVDS_R7_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_R8_Sel)					/* Hal_TCON_SET_LVDS_R8_Sel					*/	(UINT8 Param);
	void (*GetLVDS_R8_Sel)					/* Hal_TCON_GET_LVDS_R8_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_R9_Sel)					/* Hal_TCON_SET_LVDS_R9_Sel					*/	(UINT8 Param);
	void (*GetLVDS_R9_Sel)					/* Hal_TCON_GET_LVDS_R9_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_G0_Sel)					/* Hal_TCON_SET_LVDS_G0_Sel					*/	(UINT8 Param);
	void (*GetLVDS_G0_Sel)					/* Hal_TCON_GET_LVDS_G0_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_G1_Sel)					/* Hal_TCON_SET_LVDS_G1_Sel					*/	(UINT8 Param);
	void (*GetLVDS_G1_Sel)					/* Hal_TCON_GET_LVDS_G1_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_G2_Sel)					/* Hal_TCON_SET_LVDS_G2_Sel					*/	(UINT8 Param);
	void (*GetLVDS_G2_Sel)					/* Hal_TCON_GET_LVDS_G2_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_G3_Sel)					/* Hal_TCON_SET_LVDS_G3_Sel					*/	(UINT8 Param);
	void (*GetLVDS_G3_Sel)					/* Hal_TCON_GET_LVDS_G3_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_G4_Sel)					/* Hal_TCON_SET_LVDS_G4_Sel					*/	(UINT8 Param);
	void (*GetLVDS_G4_Sel)					/* Hal_TCON_GET_LVDS_G4_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_G5_Sel)					/* Hal_TCON_SET_LVDS_G5_Sel					*/	(UINT8 Param);
	void (*GetLVDS_G5_Sel)					/* Hal_TCON_GET_LVDS_G5_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_G6_Sel)					/* Hal_TCON_SET_LVDS_G6_Sel					*/	(UINT8 Param);
	void (*GetLVDS_G6_Sel)					/* Hal_TCON_GET_LVDS_G6_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_G7_Sel)					/* Hal_TCON_SET_LVDS_G7_Sel					*/	(UINT8 Param);
	void (*GetLVDS_G7_Sel)					/* Hal_TCON_GET_LVDS_G7_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_G8_Sel)					/* Hal_TCON_SET_LVDS_G8_Sel					*/	(UINT8 Param);
	void (*GetLVDS_G8_Sel)					/* Hal_TCON_GET_LVDS_G8_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_G9_Sel)					/* Hal_TCON_SET_LVDS_G9_Sel					*/	(UINT8 Param);
	void (*GetLVDS_G9_Sel)					/* Hal_TCON_GET_LVDS_G9_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_B0_Sel)					/* Hal_TCON_SET_LVDS_B0_Sel					*/	(UINT8 Param);
	void (*GetLVDS_B0_Sel)					/* Hal_TCON_GET_LVDS_B0_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_B1_Sel)					/* Hal_TCON_SET_LVDS_B1_Sel					*/	(UINT8 Param);
	void (*GetLVDS_B1_Sel)					/* Hal_TCON_GET_LVDS_B1_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_B2_Sel)					/* Hal_TCON_SET_LVDS_B2_Sel					*/	(UINT8 Param);
	void (*GetLVDS_B2_Sel)					/* Hal_TCON_GET_LVDS_B2_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_B3_Sel)					/* Hal_TCON_SET_LVDS_B3_Sel					*/	(UINT8 Param);
	void (*GetLVDS_B3_Sel)					/* Hal_TCON_GET_LVDS_B3_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_B4_Sel)					/* Hal_TCON_SET_LVDS_B4_Sel					*/	(UINT8 Param);
	void (*GetLVDS_B4_Sel)					/* Hal_TCON_GET_LVDS_B4_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_B5_Sel)					/* Hal_TCON_SET_LVDS_B5_Sel					*/	(UINT8 Param);
	void (*GetLVDS_B5_Sel)					/* Hal_TCON_GET_LVDS_B5_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_B6_Sel)					/* Hal_TCON_SET_LVDS_B6_Sel					*/	(UINT8 Param);
	void (*GetLVDS_B6_Sel)					/* Hal_TCON_GET_LVDS_B6_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_B7_Sel)					/* Hal_TCON_SET_LVDS_B7_Sel					*/	(UINT8 Param);
	void (*GetLVDS_B7_Sel)					/* Hal_TCON_GET_LVDS_B7_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_B8_Sel)					/* Hal_TCON_SET_LVDS_B8_Sel					*/	(UINT8 Param);
	void (*GetLVDS_B8_Sel)					/* Hal_TCON_GET_LVDS_B8_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_B9_Sel)					/* Hal_TCON_SET_LVDS_B9_Sel					*/	(UINT8 Param);
	void (*GetLVDS_B9_Sel)					/* Hal_TCON_GET_LVDS_B9_Sel					*/	(UINT8 *Param);
	void (*SetLVDS_Field_INV_Enable)		/* Hal_TCON_SET_LVDS_Field_INV_Enable		*/	(BOOL Param);
	void (*GetLVDS_Field_INV_Enable)		/* Hal_TCON_GET_LVDS_Field_INV_Enable		*/	(BOOL *Param);
	void (*SetLVDS_DENB_INV_Enable)			/* Hal_TCON_SET_LVDS_DENB_INV_Enable		*/	(BOOL Param);
	void (*GetLVDS_DENB_INV_Enable)			/* Hal_TCON_GET_LVDS_DENB_INV_Enable		*/	(BOOL *Param);
	void (*SetLVDS_DValid_INV_Enable)		/* Hal_TCON_SET_LVDS_DValid_INV_Enable		*/	(BOOL Param);
	void (*GetLVDS_DValid_INV_Enable)		/* Hal_TCON_GET_LVDS_DValid_INV_Enable		*/	(BOOL *Param);
	void (*SetLVDS_VSync_INV_Enable)		/* Hal_TCON_SET_LVDS_VSync_INV_Enable		*/	(BOOL Param);
	void (*GetLVDS_VSync_INV_Enable)		/* Hal_TCON_GET_LVDS_VSync_INV_Enable		*/	(BOOL *Param);
	void (*SetLVDS_HSync_INV_Enable)		/* Hal_TCON_SET_LVDS_HSync_INV_Enable		*/	(BOOL Param);
	void (*GetLVDS_HSync_INV_Enable)		/* Hal_TCON_GET_LVDS_HSync_INV_Enable		*/	(BOOL *Param);
	void (*SetLVDS_DualChannel_Enable)		/* Hal_TCON_SET_LVDS_DualChannel_Enable		*/	(BOOL Param);
	void (*GetLVDS_DualChannel_Enable)		/* Hal_TCON_GET_LVDS_DualChannel_Enable		*/	(BOOL *Param);
	void (*SetLVDS_Video_INV_Enable)		/* Hal_TCON_SET_LVDS_Video_INV_Enable		*/	(BOOL Param);
	void (*GetLVDS_Video_INV_Enable)		/* Hal_TCON_GET_LVDS_Video_INV_Enable		*/	(BOOL *Param);
	void (*SetLVDSIF_Field_Enable)			/* Hal_TCON_SET_LVDSIF_Field_Enable			*/	(BOOL Param);
	void (*GetLVDSIF_Field_Enable)			/* Hal_TCON_GET_LVDSIF_Field_Enable			*/	(BOOL *Param);
	void (*SetLVDSIF_DENB_Enable)			/* Hal_TCON_SET_LVDSIF_DENB_Enable			*/	(BOOL Param);
	void (*GetLVDSIF_DENB_Enable)			/* Hal_TCON_GET_LVDSIF_DENB_Enable			*/	(BOOL *Param);
	void (*SetLVDSIF_HSync_Enable)			/* Hal_TCON_SET_LVDSIF_HSync_Enable			*/	(BOOL Param);
	void (*GetLVDSIF_HSync_Enable)			/* Hal_TCON_GET_LVDSIF_HSync_Enable			*/	(BOOL *Param);
	void (*SetLVDSIF_VSync_Enable)			/* Hal_TCON_SET_LVDSIF_VSync_Enable			*/	(BOOL Param);
	void (*GetLVDSIF_VSync_Enable)			/* Hal_TCON_GET_LVDSIF_VSync_Enable			*/	(BOOL *Param);
	void (*SetLVDSIF_ChannelSwap_Enable)	/* Hal_TCON_SET_LVDSIF_ChannelSwap_Enable	*/	(BOOL Param);
	void (*GetLVDSIF_ChannelSwap_Enable)	/* Hal_TCON_GET_LVDSIF_ChannelSwap_Enable	*/	(BOOL *Param);
	void (*SetLVDSIF_DataType_Sel)			/* Hal_TCON_SET_LVDSIF_DataType_Sel			*/	(BOOL Param);
	void (*GetLVDSIF_DataType_Sel)			/* Hal_TCON_GET_LVDSIF_DataType_Sel			*/	(BOOL *Param);
	void (*SetLVDS_RGBOrder_Con_Enable)		/* Hal_TCON_SET_LVDS_RGBOrder_Con_Enable	*/	(BOOL Param);
	void (*GetLVDS_RGBOrder_Con_Enable)		/* Hal_TCON_GET_LVDS_RGBOrder_Con_Enable	*/	(BOOL *Param);
	void (*SetLVDS_DI_Channel0_Sel)			/* Hal_TCON_SET_LVDS_DI_Channel0_Sel		*/	(UINT8 Param);
	void (*GetLVDS_DI_Channel0_Sel)			/* Hal_TCON_GET_LVDS_DI_Channel0_Sel		*/	(UINT8 *Param);
	void (*SetLVDS_DI_Channel1_Sel)			/* Hal_TCON_SET_LVDS_DI_Channel1_Sel		*/	(UINT8 Param);
	void (*GetLVDS_DI_Channel1_Sel)			/* Hal_TCON_GET_LVDS_DI_Channel1_Sel		*/	(UINT8 *Param);
	void (*SetLVDS_DI_Channel2_Sel)			/* Hal_TCON_SET_LVDS_DI_Channel2_Sel		*/	(UINT8 Param);
	void (*GetLVDS_DI_Channel2_Sel)			/* Hal_TCON_GET_LVDS_DI_Channel2_Sel		*/	(UINT8 *Param);
	void (*SetLVDS_DI_Channel3_Sel)			/* Hal_TCON_SET_LVDS_DI_Channel3_Sel		*/	(UINT8 Param);
	void (*GetLVDS_DI_Channel3_Sel)			/* Hal_TCON_GET_LVDS_DI_Channel3_Sel		*/	(UINT8 *Param);
	void (*SetLVDS_DI_Channel4_Sel)			/* Hal_TCON_SET_LVDS_DI_Channel4_Sel		*/	(UINT8 Param);
	void (*GetLVDS_DI_Channel4_Sel)			/* Hal_TCON_GET_LVDS_DI_Channel4_Sel		*/	(UINT8 *Param);
	void (*SetLVDS_DIA_Channel0_Sel)		/* Hal_TCON_SET_LVDS_DIA_Channel0_Sel		*/	(UINT8 Param);
	void (*GetLVDS_DIA_Channel0_Sel)		/* Hal_TCON_GET_LVDS_DIA_Channel0_Sel		*/	(UINT8 *Param);
	void (*SetLVDS_DIA_Channel1_Sel)		/* Hal_TCON_SET_LVDS_DIA_Channel1_Sel		*/	(UINT8 Param);
	void (*GetLVDS_DIA_Channel1_Sel)		/* Hal_TCON_GET_LVDS_DIA_Channel1_Sel		*/	(UINT8 *Param);
	void (*SetLVDS_DIA_Channel2_Sel)		/* Hal_TCON_SET_LVDS_DIA_Channel2_Sel		*/	(UINT8 Param);
	void (*GetLVDS_DIA_Channel2_Sel)		/* Hal_TCON_GET_LVDS_DIA_Channel2_Sel		*/	(UINT8 *Param);
	void (*SetLVDS_DIA_Channel3_Sel)		/* Hal_TCON_SET_LVDS_DIA_Channel3_Sel		*/	(UINT8 Param);
	void (*GetLVDS_DIA_Channel3_Sel)		/* Hal_TCON_GET_LVDS_DIA_Channel3_Sel		*/	(UINT8 *Param);
	void (*SetLVDS_DIA_Channel4_Sel)		/* Hal_TCON_SET_LVDS_DIA_Channel4_Sel		*/	(UINT8 Param);
	void (*GetLVDS_DIA_Channel4_Sel)		/* Hal_TCON_GET_LVDS_DIA_Channel4_Sel		*/	(UINT8 *Param);
	void (*Dummy_301)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_302)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_303)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_304)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_305)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_306)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_307)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_308)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_309)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_310)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_311)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_312)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_313)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_314)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_315)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_316)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_317)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_318)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_319)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_320)						/* Hal_TCON_dummy							*/	(void);

	void (*Set_Adj_R_Enable)				/* Hal_TCON_SET_Adj_R_Enable				*/	(UINT8 Param);
	void (*Get_Adj_R_Enable)				/* Hal_TCON_GET_Adj_R_Enable				*/	(UINT8 *Param);
	void (*Set_Adj_G_Enable)				/* Hal_TCON_SET_Adj_G_Enable				*/	(UINT8 Param);
	void (*Get_Adj_G_Enable)				/* Hal_TCON_GET_Adj_G_Enable				*/	(UINT8 *Param);
	void (*Set_Adj_B_Enable)				/* Hal_TCON_SET_Adj_B_Enable				*/	(UINT8 Param);
	void (*Get_Adj_B_Enable)				/* Hal_TCON_GET_Adj_B_Enable				*/	(UINT8 *Param);
	void (*Set_Adj_R_cp1)					/* Hal_TCON_SET_Adj_R_cp1					*/	(UINT8 src, UINT8 dest);
	void (*Get_Adj_R_cp1)					/* Hal_TCON_GET_Adj_R_cp1					*/	(UINT8 *src, UINT8 *dest);
	void (*Set_Adj_R_cp2)					/* Hal_TCON_SET_Adj_R_cp2					*/	(UINT8 src, UINT8 dest);
	void (*Get_Adj_R_cp2)					/* Hal_TCON_GET_Adj_R_cp2					*/	(UINT8 *src, UINT8 *dest);
	void (*Set_Adj_R_cp3)					/* Hal_TCON_SET_Adj_R_cp3					*/	(UINT8 src, UINT8 dest);
	void (*Get_Adj_R_cp3)					/* Hal_TCON_GET_Adj_R_cp3					*/	(UINT8 *src, UINT8 *dest);
	void (*Set_Adj_R_slope0)				/* Hal_TCON_SET_Adj_R_slope0				*/	(UINT16 slope);
	void (*Get_Adj_R_slope0)				/* Hal_TCON_GET_Adj_R_slope0				*/	(UINT16 *slope);
	void (*Set_Adj_R_slope1)				/* Hal_TCON_SET_Adj_R_slope1				*/	(UINT16 slope);
	void (*Get_Adj_R_slope1)				/* Hal_TCON_GET_Adj_R_slope1				*/	(UINT16 *slope);
	void (*Set_Adj_R_slope2)				/* Hal_TCON_SET_Adj_R_slope2				*/	(UINT16 slope);
	void (*Get_Adj_R_slope2)				/* Hal_TCON_GET_Adj_R_slope2				*/	(UINT16 *slope);
	void (*Set_Adj_R_slope3)				/* Hal_TCON_SET_Adj_R_slope3				*/	(UINT16 slope);
	void (*Get_Adj_r_slope3)				/* Hal_TCON_GET_Adj_r_slope3				*/	(UINT16 *slope);
	void (*Set_Adj_G_cp1)					/* Hal_TCON_SET_Adj_G_cp1					*/	(UINT8 src, UINT8 dest);
	void (*Get_Adj_G_cp1)					/* Hal_TCON_GET_Adj_G_cp1					*/	(UINT8 *src, UINT8 *dest);
	void (*Set_Adj_G_cp2)					/* Hal_TCON_SET_Adj_G_cp2					*/	(UINT8 src, UINT8 dest);
	void (*Get_Adj_G_cp2)					/* Hal_TCON_GET_Adj_G_cp2					*/	(UINT8 *src, UINT8 *dest);
	void (*Set_Adj_G_cp3)					/* Hal_TCON_SET_Adj_G_cp3					*/	(UINT8 src, UINT8 dest);
	void (*Get_Adj_G_cp3)					/* Hal_TCON_GET_Adj_G_cp3					*/	(UINT8 *src, UINT8 *dest);
	void (*Set_Adj_G_slope0)				/* Hal_TCON_SET_Adj_G_slope0				*/	(UINT16 slope);
	void (*Get_Adj_G_slope0)				/* Hal_TCON_GET_Adj_G_slope0				*/	(UINT16 *slope);
	void (*Set_Adj_G_slope1)				/* Hal_TCON_SET_Adj_G_slope1				*/	(UINT16 slope);
	void (*Get_Adj_G_slope1)				/* Hal_TCON_GET_Adj_G_slope1				*/	(UINT16 *slope);
	void (*Set_Adj_G_slope2)				/* Hal_TCON_SET_Adj_G_slope2				*/	(UINT16 slope);
	void (*Get_Adj_G_slope2)				/* Hal_TCON_GET_Adj_G_slope2				*/	(UINT16 *slope);
	void (*Set_Adj_G_slope3)				/* Hal_TCON_SET_Adj_G_slope3				*/	(UINT16 slope);
	void (*Get_Adj_G_slope3)				/* Hal_TCON_GET_Adj_G_slope3				*/	(UINT16 *slope);
	void (*Set_Adj_B_cp1)					/* Hal_TCON_SET_Adj_B_cp1					*/	(UINT8 src, UINT8 dest);
	void (*Get_Adj_B_cp1)					/* Hal_TCON_GET_Adj_B_cp1					*/	(UINT8 *src, UINT8 *dest);
	void (*Set_Adj_B_cp2)					/* Hal_TCON_SET_Adj_B_cp2					*/	(UINT8 src, UINT8 dest);
	void (*Get_Adj_B_cp2)					/* Hal_TCON_GET_Adj_B_cp2					*/	(UINT8 *src, UINT8 *dest);
	void (*Set_Adj_B_cp3)					/* Hal_TCON_SET_Adj_B_cp3					*/	(UINT8 src, UINT8 dest);
	void (*Get_Adj_B_cp3)					/* Hal_TCON_GET_Adj_B_cp3					*/	(UINT8 *src, UINT8 *dest);
	void (*Set_Adj_B_slope0)				/* Hal_TCON_SET_Adj_B_slope0				*/	(UINT16 slope);
	void (*Get_Adj_B_slope0)				/* Hal_TCON_GET_Adj_B_slope0				*/	(UINT16 *slope);
	void (*Set_Adj_B_slope1)				/* Hal_TCON_SET_Adj_B_slope1				*/	(UINT16 slope);
	void (*Get_Adj_B_slope1)				/* Hal_TCON_GET_Adj_B_slope1				*/	(UINT16 *slope);
	void (*Set_Adj_B_slope2)				/* Hal_TCON_SET_Adj_B_slope2				*/	(UINT16 slope);
	void (*Get_Adj_B_slope2)				/* Hal_TCON_GET_Adj_B_slope2				*/	(UINT16 *slope);
	void (*Set_Adj_B_slope3)				/* Hal_TCON_SET_Adj_B_slope3				*/	(UINT16 slope);
	void (*Get_Adj_B_slope3)				/* Hal_TCON_GET_Adj_B_slope3				*/	(UINT16 *slope);
	void (*Set_Adj_R_slope_auto)			/* Hal_TCON_SET_Adj_R_slope_auto			*/	(void);
	void (*Set_Adj_G_slope_auto)			/* Hal_TCON_SET_Adj_G_slope_auto			*/	(void);
	void (*Set_Adj_B_slope_auto)			/* Hal_TCON_SET_Adj_B_slope_auto			*/	(void);
	void (*Dummy_401)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_402)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_403)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_404)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_405)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_406)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_407)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_408)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_409)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_400)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_411)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_412)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_413)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_414)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_415)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_416)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_417)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_418)						/* Hal_TCON_dummy							*/	(void);
	void (*Dummy_419)						/* Hal_TCON_dummy							*/	(void);
	void (*Report_timing)					/* Hal_TCON_report_timing					*/	(UINT16 *v_top_start, UINT16 *v_bot_start, UINT16 *v_top_end, UINT16 *v_bot_end, UINT16 *h_active, UINT16 *h_total, UINT16 *v_top_total, UINT16 *v_bot_total);
} stTCON_API_t;
STATIC_ASSERT(sizeof(stTCON_API_t)==4*570);

//tcon library function
extern const stTCON_API_t* Hal_Get_TCON(void);
extern UINT8 Hal_Check_TCON(void);
//for debug
extern void PAD_DS(UINT8 gpio, UINT8 value);

//other library function
extern int diag_printf( const char *fmt, ... ) __attribute__((format(printf, 1, 2)));

//panel library function
extern void panel_set_gpio(UINT8 mode, UINT8 pin, UINT8 value);
extern void panel_pulllow_setting(UINT8 pulllow, UINT8 panel_type, UINT8 rgb888, UINT8 mode);
extern void panel_LVDS_output_format(UINT32 format);
extern void panel_init(void);
extern void panel_Header_addr(UINT32 addr);

#ifndef UBOOT_TCON
	extern void panel_report(void);
	extern void panel_spi_report(void);
	extern void panel_pwm_report(void);
	extern void panel_pinmux_report(void);
	extern void panel_showparts(void);
	extern void panel_chgpart(UINT8 part);
#endif

#endif		// __HAL_SPHE8388_TCON_H__

