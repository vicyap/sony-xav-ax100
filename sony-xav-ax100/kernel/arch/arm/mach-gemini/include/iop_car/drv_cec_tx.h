#ifndef DRV_CEC_TX_H
#define DRV_CEC_TX_H

//#include <stdio.h> //Kaney mark
//#include <string.h> //Kaney mark
//#include <types.h> //Kaney mark
//#include <gsl.h> //Kaney mark
#include <linux/string.h> //Kaney add
#include <mach/common.h> //Kaney add
#include "drv_common.h" //Kaney add


typedef int (*CEC_ModCbk_pf)(int, int);

//CEC Modual Config
typedef struct CEC_ModualCfg_t_
{
	//ifunc2 pfCallBack;
	CEC_ModCbk_pf		pfCallBack;
	
} CEC_ModualCfg_t;

//CEC Return Message definition
#define CEC_OPERATION_FAIL			(-1)
#define CEC_OPERATION_SUCCESS		0

#define CEC_CENTER_PRIORITY_BASE	(5)
#define CEC_CENTER_STACK_SIZE		(4096)

typedef struct CEC_QMsgBody_t_ {
	
	UINT32 dMsg;
	UINT32 dParam;

}CEC_QMsgBody_t;

#define CEC_CENTER_QMSG_BODY_SIZE	(sizeof(CEC_QMsgBody_t))

#define DRV_CEC_DEVICE_SUPPORTED_MAX	(3)	// it should as same as CEC_DEVICE_SUPPORTED_MAX in cec_cmd.h
#define DRV_CEC_DEVICE_NULL				(0)
//CEC QMsg
#define CEC_QMSG_ON			(1)
#define CEC_QMSG_OFF		(2)
#define CEC_QMSG_PHYADDR	(3)

typedef enum CecEvent_t_ //refer to event.h --> For Update Later, M: Must.
{
//-----------------------------------------------
	//User Input Event Definitions
	//General Events

	//CEC_UI_EVENT_10_PLUS,

	CEC_UI_EVENT_POWER = 0x00000000,
	CEC_UI_EVENT_CLEAR,
	CEC_UI_EVENT_UP,
	CEC_UI_EVENT_DOWN,
	CEC_UI_EVENT_LEFT,
	CEC_UI_EVENT_RIGHT,
	CEC_UI_EVENT_ENTER,
	CEC_UI_EVENT_DISPLAY,
	CEC_UI_EVENT_MUTE,
	CEC_UI_EVENT_VOL_UP,
	CEC_UI_EVENT_VOL_DN,
	//CEC_UI_EVENT_MENU, 			//M
	//CEC_UI_EVENT_GOTO,
	//CEC_UI_EVENT_AB,
	//CEC_UI_EVENT_REPEAT,
	CEC_UI_EVENT_STOP,
	CEC_UI_EVENT_PLAY,
	CEC_UI_EVENT_PAUSE,
	CEC_UI_EVENT_SLOW,
	CEC_UI_EVENT_FF,
	CEC_UI_EVENT_FR,
	CEC_UI_EVENT_NEXT,
	CEC_UI_EVENT_PREV,
	CEC_UI_EVENT_RED,				//M
	CEC_UI_EVENT_GREEN,			//M
	CEC_UI_EVENT_YELLOW,			//M
	CEC_UI_EVENT_BLUE,			//M
	//CEC_UI_EVENT_VIRTUAL_REMOTE,

	CEC_UI_EVENT_TITLE,
	CEC_UI_EVENT_SETUP,
	//CEC_UI_EVENT_A,
	//CEC_UI_EVENT_B,
	//CEC_UI_EVENT_C,
	//CEC_UI_EVENT_D,
	//CEC_UI_EVENT_ZOOM,
	//CEC_UI_EVENT_MARK,
	//CEC_UI_EVENT_RANDOM,
	//CEC_UI_EVENT_PIPAUDIO,
	//CEC_UI_EVENT_PIPVIDEO,
	//CEC_UI_EVENT_LR,
	//CEC_UI_EVENT_PN,
	//CEC_UI_EVENT_KARAOKE,
	//CEC_UI_EVENT_RESOLUTION,
	
	//Disc Player Specific Events
	CEC_UI_EVENT_EJECT,
	CEC_UI_EVENT_TITLE_MENU,
	CEC_UI_EVENT_RETURN,
	CEC_UI_EVENT_ANGLE,
	CEC_UI_EVENT_LANGUAGE,

	//STB Specific Events

	CEC_UI_EVENT_CH_UP,
	CEC_UI_EVENT_CH_DN,

	CEC_UI_EVENT_FAV,
	CEC_UI_EVENT_AUDIO,
	CEC_UI_EVENT_TVRADIO,
	CEC_UI_EVENT_PAGE_UP,
	CEC_UI_EVENT_PAGE_DN,
	CEC_UI_EVENT_RECALL,
	CEC_UI_EVENT_INFO,
	CEC_UI_EVENT_EXIT,
	CEC_UI_EVENT_TEXT,
	CEC_UI_EVENT_EPG,
	CEC_UI_EVENT_SUBTITLE,
	//CEC_UI_EVENT_P_SCAN,	
	//CEC_UI_EVENT_DEV,
	//CEC_UI_EVENT_VFMT,
	//CEC_UI_EVENT_MIXER,
	CEC_UI_EVENT_SOURCE,
	//CEC_UI_EVENT_REC,
	
	CEC_UI_EVENT_NULL,

//-----------------------------------------------
	CEC_UI_EVENT_0 = 0x10000000,
	CEC_UI_EVENT_1,
	CEC_UI_EVENT_2,
	CEC_UI_EVENT_3,
	CEC_UI_EVENT_4,
	CEC_UI_EVENT_5,
	CEC_UI_EVENT_6,
	CEC_UI_EVENT_7,
	CEC_UI_EVENT_8,
	CEC_UI_EVENT_9,
	CEC_UI_EVENT_10,
	CEC_UI_EVENT_DOT,

//-----------------------------------------------
	CEC_DMN_EVENT_FF_MIN = 0xf0000000,
	CEC_DMN_EVENT_FF_MEDIUM,
	CEC_DMN_EVENT_FF_MAX,
	CEC_DMN_EVENT_FR_MIN,
	CEC_DMN_EVENT_FR_MEDIUM,
	CEC_DMN_EVENT_FR_MAX,
	CEC_DMN_EVENT_SF_MIN,
	CEC_DMN_EVENT_SF_MEDIUM,
	CEC_DMN_EVENT_SF_MAX,
	CEC_DMN_EVENT_SR_MIN,
	CEC_DMN_EVENT_SR_MEDIUM,
	CEC_DMN_EVENT_SR_MAX,
	CEC_DMN_EVENT_ROOT_MENU,
	CEC_DMN_EVENT_RESTORE_VOL,
	CEC_DMN_EVENT_SET_LANGUAGE,
//------------------------------------------------
	CEC_DMN_EVENT_MAX,

} CecEvent_t;

typedef enum CecEventType_t_ 
{
	CEC_EVENT_TYPE_UI,
	CEC_EVENT_TYPE_UI_NUM,
	CEC_EVENT_TYPE_DMN,
	
} CecEventType_t;

typedef struct CecStandbyCfg_t_
{
	UINT8	bLogAddr[DRV_CEC_DEVICE_SUPPORTED_MAX];
	UINT8	bPhyAddr[2];
	
} CecStandbyCfg_t;


/***********************************************************************
*
*	.IF
*
***********************************************************************/
UINT32 DRV_CEC_EventRemoveHeader(CecEvent_t eEvent);
CecEventType_t DRV_CEC_EventChkType(CecEvent_t eEvent);
int DRV_CEC_Initialize(CEC_ModualCfg_t *CecMdlCfg);
int DRV_CEC_Finalize(void);
int DRV_CEC_GetLogAddr(UINT8 bLogNo, UINT8 *pbRetLogAddr);
int DRV_CEC_GetPhyAddr(UINT16 *pwRetPhyAddr);
int DRV_CEC_SendQMsg(UINT32 dMsg ,UINT32 dParam);
int DRV_CEC_StandbySetConfig(CecStandbyCfg_t stConfig);
void DRV_CEC_StandbySetupIop(void);
extern UINT8 CEC_IO_GetLogAddr(UINT8 bLogNo);
void DRV_CEC_ISR(void); //Kaney add
int cec_Standby_Initialize(void); //Kaney add
vfunc cec_Standby_Get_SetupIopFunct(void); //Kaney add

#endif



