/******************************************************************************/
/**
*
* \file	drv_iop.h
*
* \brief	This driver controls the IOP component.
*
* \note	Copyright (c) 2008 Sunplus Technology Co., Ltd. \n
*		All rights reserved.
*
* \author
*
******************************************************************************/
#ifndef IOP_H
#define IOP_H
#include "drv_common.h"
//#include "types.h" //Kaney mark
#include <mach/common.h> //Kaney add

/**
*\brief driver iop system clock
*/
typedef enum {
   DRV_IOP_SYSCLK_10MHZ ///<10 MHZ
} DRV_IOPSysClk_t;

#define IOP_REGBASE			(0xbffe8000 + (14 * 32 * 4))/**< iop register base */
#define IOP_CONTROL			(IOP_REGBASE + (0 << 2))/**< iop control register */
#define IOP_STATUS			(IOP_REGBASE + (1 << 2))/**< iop status register */
#define IOP_BP				(IOP_REGBASE + (2 << 2))/**< iop breakpoint register */
#define IOP_REGSEL			(IOP_REGBASE + (3 << 2))/**< iop select register */
#define IOP_REGOUT			(IOP_REGBASE + (4 << 2))/**< iop register value,Use REGSEL to select register to read */
#define IOP_MEMLIMIT		(IOP_REGBASE + (5 << 2))/**< iop memory limit register*/
#define IOP_DATA_BASE		(IOP_REGBASE + (8 << 2))/**< iop data register base*/
#define IOP_DATA(i)			(IOP_DATA_BASE + (i << 2))/**<iop-risc mail box register*/
#define IOP_CODE_BASE		(IOP_REGBASE + (16 << 2))/**<iop code base register*/
#define IOP_CODE_BASE_EXT	(IOP_REGBASE + (17 << 2))/**<iop code base extend register*/
#define IOP_CODE_SIZE (16384) /**< iop code size*/
/**
*\brief driver iop standby config structure
*/
typedef struct 
{
	const UINT8 *pbIopCode;///<point the standby iop map code 
	UINT32	dIopCodeSize;  			//zhangjihua add for mantis 149050
	vfunc pfSetupIrIop;///< setup standby iop repond ir key
	vfunc pfSetupVfdIop;///< setup vfd standby iop 
	vfunc pfSetupCecIop;///<setup hdmi cec standby iop 
	#ifdef SUPPORT_IOP2_STANDBY_WAKEUPTIMER //lizhan add 2010-3-23
	vfunc pfWriteDateTimeIop;///<setup wakeup timer time standby iop 
	#endif
	
} DRV_IopStandbyCfg_t;

typedef enum
{
	IOP_STBY_ON_CMD_Power,
	IOP_STBY_ON_CMD_Eject,
	IOP_STBY_ON_CMD_Play,
	
} IopStbyOnCmd_e;
#ifdef SUPPORT_IOP2_STANDBY_WAKEUPTIMER 
typedef struct 
{
	UINT8 *pbIopWkTimerCount;
	UINT8 *pbAutoWkTimerCount;
	UINT8 *pbRiscIntoStbyTimingInfo;
}IopStbyWkTimingInfo_t;
#endif
typedef struct 
{
	UINT8				fSaved;
	IopStbyOnCmd_e		eCmd;
#ifdef SUPPORT_IOP2_STANDBY_WAKEUPTIMER 
	IopStbyWkTimingInfo_t   stWkTimingInfo;
#endif
	//UINT32			Others;
	
} DRV_IopStandbyOnRequest_t;
//2011/5/10 wenjing.yang modified for mantis 0130994 	START {{{
#define IOP_STBY_ON_DATA_MAX		512
#define IOP_STBY_ON_FUNCs_MAX		4

typedef struct DataLoc_t_
{
	UINT8 fValid;
	UINT32 dOffset;
	UINT32 dSize;
	
} DataLoc_t;

typedef struct IopDataMap_t_
{
	UINT8 fValid;
	DataLoc_t DataLoc[IOP_STBY_ON_FUNCs_MAX];
} IopDataMap_t;
//2011/5/10 wenjing.yang modified for mantis 0130994 	END }}}

#if 1 //Kaney add
typedef struct iop_standby_timer_s {
	UINT8	bCurSec;
	UINT8	bCurMin;
	UINT8   bCurHour;
	UINT8	bCurDay;
	UINT8	bCurMon;
	UINT16  bCurYear;
} iop_standby_timer_t;
#endif


/**********************************************************************/
/**
* \fn	DRV_Status_t DRV_IOPMemoryInit(void)
*
* \brief	Initialize the IOP memory config.
*
* \return DRV_SUCCESS if successful
*
**********************************************************************/
DRV_Status_t DRV_IOPMemoryInit(void);

/**********************************************************************/
/**
* \fn	DRV_Status_t DRV_IOPInit(const UINT8 *pbIopcode)
*
* \brief 	Initialize the IOP firmware.  IOP firmware needs to be loaded on bootup.
*		Special IOP firmware (IR-only) may be loaded when entering low power mode.
*		In this case, normal IOP code must be re-loaded when leaving low power mode.
*
* \param pbIopcode: IOP code
*
* \return DRV_SUCCESS if successful
*		DRV_ERR_INVALID_PARAM if any of the parameters is invalid
*
**********************************************************************/
DRV_Status_t DRV_IOPInit(const UINT8 *pbIopcode);

/**********************************************************************/
/**
* \fn 	DRV_IOPStatus_t DRV_IOPInitByClk(const UINT8 * pbIopcode, UINT16 wSystemClock)
*
* \brief 	Initialize the IOP firmware.  IOP firmware needs to be loaded on bootup.
*		Special IOP firmware (IR-only) may be loaded when entering low power mode.
*		In this case, normal IOP code must be re-loaded when leaving low power mode.
*
* \param pbIopcode: IOP code
* \param wSystemClock: system clock
*
* \return DRV_SUCCESS if successful
*		DRV_ERR_INVALID_PARAM if any of the parameters is invalid
*
**********************************************************************/
DRV_Status_t DRV_IOPInitByClk(const UINT8 * pbIopcode, UINT16 wSystemClock);

/**********************************************************************/
/**
* \fn 	DRV_IOPStatus_t DRV_IOPUninit(void)
*
* \brief 	Stop IOP
*
* \return DRV_SUCCESS on success
*
**********************************************************************/
DRV_Status_t DRV_IOPUninit(void);

/**********************************************************************/
/**
* \fn 	DRV_IOPSetSysClk(DRV_IOPSysClk_t clkFreq)
*
* \brief 	Set the IOP system clock
*
* \param clkFreq: IOP system clock frequency
*
* \return DRV_SUCCESS: if successful
*		DRV_ERR_INVALID_PARAM: if any of the parameters is invalid
*
**********************************************************************/
DRV_Status_t DRV_IOPSetSysClk(DRV_IOPSysClk_t clkFreq);

/**********************************************************************/
/**
* \fn 	DRV_Status_t DRV_IOPStandby_Init(const UINT8 * pbIopcode, UINT32 u32IRKey, UINT16 u16CustomID)
*
* \brief 	 IOP init of Standby mode
*
* \param pbIopcode: IOP code
* \param	u32IRKey: responsed key of standbymode
* \param	u16CustomID: custom ID of IR type
*
* \return DRV_SUCCESS: if successful
*		DRV_ERR_INVALID_PARAM: if any of the parameters is invalid
*
**********************************************************************/
DRV_Status_t DRV_IOPStandby_Init(const UINT8 * pbIopcode, UINT32 u32IRKey, UINT16 u16CustomID);

/**********************************************************************/
/**
* \fn 	DRV_Status_t DRV_IOPStandby_Initialize(DRV_IopStandbyCfg_t *pstConfig)
*
* \brief 	 IOP init of Standby mode and IR/VFD/CEC
*
* \param pstConfig:  iop standby config structure
*
* \return DRV_SUCCESS: if successful
*		DRV_ERR_INVALID_PARAM: if any of the parameters is invalid
*
**********************************************************************/
DRV_Status_t DRV_IOPStandby_Initialize(DRV_IopStandbyCfg_t *pstConfig);

/**********************************************************************/
/**
* \fn		DRV_IOPStandby_POTGet(DRV_IopStandbyOnRequest_t *pstRet)
*
* \brief	Get POT to check which command is request from standby to on.
*
* \param	pstRet:	Return POT.
*
* \return	DRV_SUCCESS: if successful
*			DRV_ERR_INVALID_OP: if un-saved.
*
**********************************************************************/
DRV_Status_t DRV_IOPStandby_POTGet(DRV_IopStandbyOnRequest_t *pstRet);

/**********************************************************************/
/**
* \fn 		DRV_Status_t DRV_IOPStandby_POTSave(void)
*
* \brief 	Save POT(Power On Table) to check which command is 
*			request from standby to on.
*
* \return	DRV_SUCCESS: if successful
*
**********************************************************************/
DRV_Status_t DRV_IOPStandby_POTSave(void);

//2011/5/10 wenjing.yang modified for mantis 0130994 	START {{{
void DRV_IOP_Get_Standby_POT(UINT8 **ppIopData, IopDataMap_t **ppstIopMap);
//2011/5/10 wenjing.yang modified for mantis 0130994 	END }}}
/**********************************************************************/
/**
* \fn 	DRV_Status_t DRV_IOP_GetIopBufAddrValue(UINT32 *pIopBufVal)
*
* \brief 	 Get IOP memory address
*
* \param pIopBufVal:  IOP memory address
*
* \return DRV_SUCCESS: if successful
*		DRV_ERR_INVALID_PARAM: if any of the parameters is invalid
*
**********************************************************************/
DRV_Status_t DRV_IOP_GetIopBufAddrValue(UINT32 *pIopBufVal);

//2010/4/27 lizhan add for standby wakeup system by gpio control hardware reset.START {{{
/**********************************************************************/
/**
* \fn 	DRV_Status_t DRV_IOPStandby_Ctrl_Hw_Reset_Gpio(UINT8 bGpioNum)
*
* \brief 	 RISC send control hardware reset gpio number to standby iop
*
* \param bGpioNum:  gpio number
*
* \return DRV_SUCCESS: if successful
*		DRV_ERR_INVALID_PARAM: if any of the parameters is invalid
*
**********************************************************************/
DRV_Status_t DRV_IOPStandby_Ctrl_Hw_Reset_Gpio(UINT8 bGpioNum);
//2010/4/27 lizhan add for standby wakeup system by gpio control hardware reset. END }}}

//2011/8/3 wenjing.yang modified for mantis 0145407 	START {{{
/**********************************************************************/
/**
* \fn 	DRV_Status_t DRV_IOPStandby_TestGpio(UINT8 u8GPIONum, UINT8 u8ICVer)
*
* \brief 	 RISC send GPIO number to standby IOP for testing
*
* \param 
*
*u8GPIONum:(important: All of the GPIO numbers musn't be shifted!!!)
*	bit0~3: Test GPIO Num
*	0~9(single test)	only test the given GPIO number
*	0x0f(all test)	test GPIO 0~9	
*
*	bit4~7: HW Reset GPIO Num
*	
*u8ICVer:
*	0(1500)
*	1(1502)
*
* \return DRV_SUCCESS: if successful
*		DRV_ERR_INVALID_PARAM: if any of the parameters is invalid
*
**********************************************************************/
#ifdef IOP_GPIO_CONTROL_TEST
DRV_Status_t DRV_IOPStandby_ConfigTestGpio(UINT8 u8GPIONum, UINT8 u8ICVer);
DRV_Status_t DRV_IOPStandby_TestGpio(void);
#endif
//2011/8/3 wenjing.yang modified for mantis 0145407 	END }}}

void DRV_IOPStandby_RISC_PowerOff(void);

DRV_Status_t DRV_IOPInit_without_fw(void); //Kaney add
int MAINAPP_FinalizePlatform(void); //Kaney add
int MAINAPP_FinalizePlatform2(void); //Kaney add
DRV_Status_t DRV_IOP_InitByClk(const UINT8 * pbIopcode, UINT16 wSystemClock); //Kaney add
void get_time_from_IOP_standby(void); //Kaney add
void get_time_from_IOP(iop_standby_timer_t *time); //Kaney add

#endif /* IOP_H */

