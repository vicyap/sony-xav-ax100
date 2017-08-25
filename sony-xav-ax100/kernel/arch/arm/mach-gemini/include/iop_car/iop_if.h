#include <mach/common.h> //Kaney add
#include <iop_car/drv_common.h>

#define MAX_DATA_NUM	(10)

#define IOP_DATA0_CMD(type, cmd, param)	((type<<12)|(cmd<<8)|(param))
#define IOP_DATA1_CMD(param1, param2)		((param1<<8)|(param2))
#define IOP_DATA3_CMD(param3, param4)		((param3<<8)|(param4))


#define	SET_DISABLE	(0)
#define	SET_ENABLE		(1)

#define	EXIT_REVERSE	(0)
#define	INTO_REVERSE	(1)

typedef enum{	
	PAN_RES_0,		// 0:800X480
	PAN_RES_1,		// 1:800X600
	PAN_RES_2,		// 2:1024X600
	PAN_RES_3,		// 3:1280X720
	PAN_RES_4,		// 4:1366X768
	PAN_RES_5,		// 5:1440X900
	PAN_RES_6,		// 6:1920X1080
}PANNEL_RESOLUTION_T;

//POWER TO IOP CMD
typedef enum{	
	POWER2IOP_SET,
}POWER2IOP_CMD_T;

//BACKUPCAM TO IOP CMD
typedef enum{	
	BACKUPCAM2IOP_SET_IOPCTRLCAMONOFF,
	BACKUPCAM2IOP_CHECK_ISIOPCTRL,
	BACKUPCAM2IOP_SET_GPIO,
	BACKUPCAM2IOP_SET_PAN_RES,
}BACKUPCAM2IOP_CMD_T;

//KEYBOARD TO IOP CMD
typedef enum{
	KEYBOARD2IOP_GET_BUFADDRESS,
	KEYBOARD2IOP_SET_AD,
	KEYBOARD2IOP_GET_DATALENGTH,
}KEYBOARD2IOP_CMD_T;

//ROATE TO IOP CMD
typedef enum{
	ROATE2IOP_SET_SCANONOFF,
	ROATE2IOP_GET_BUFADDRESS,
	ROATE2IOP_SET_MODE,
	ROATE2IOP_GET_DATALENGTH,
	ROATE2IOP_SET_DEBUGMODE,
	ROATE2IOP_SET_GPIO,
}ROATE2IOP_CMD_T;

//IOP CMD TYPE
typedef enum{
	IOP_CMD_TYPE_IDLE,	
	IOP_CMD_TYPE_UART,	
	IOP_CMD_TYPE_I2C,	
	IOP_CMD_TYPE_ROATE,	
	IOP_CMD_TYPE_KEYBOARD,	
	IOP_CMD_TYPE_TOUCH,	
	IOP_CMD_TYPE_BACKUPCAM,	
	IOP_CMD_TYPE_POWER,
}IOP_CMD_TYPE_T;

//IOP DATA2 Flag Check Is Data Exit
#define ROATE_DATA_EXIST		(1<<0)
#define KEYBOARD_DATA_EXIST	(1<<1)
#define BACKUPCAM_STATE		(1<<2)



#define IOP_SUCCESS					(0)
#define IOP_WAIT_CLEAR_TIMEOUT	(-1)


////////////////////////////////////////////////

int DRV_CheckIOPData3Flag(UINT16 wFlag);
void DRV_ClearIOPData3Flag(UINT16 wFlag);
int Check_IOPData0Clear(void);

/***********************************************************************************
* ±Û¶s(rotate)
***********************************************************************************/
 typedef struct _ROTATE_MSG_ITEM_
{
	UINT8	dDataBuffer[MAX_DATA_NUM][2];	//[o]:direct(1 for right, 2 for left)  [1]:turn times 
	UINT16	dDataNum;
}ROTATE_MSG_ITEM;

DRV_Status_t DRV_Rotate_SetMode(void);
DRV_Status_t DRV_Rotate_Init(void);
DRV_Status_t DRV_Rotate_GetKeyValue(ROTATE_MSG_ITEM *pRotateMSG);


/***********************************************************************************
* keyboard
***********************************************************************************/
 typedef struct _KEYBOARD_MSG_ITEM_
{
	UINT16    dDataBuffer[MAX_DATA_NUM]; 
	UINT16    dDataNum;
}KEYBOARD_MSG_ITEM;

DRV_Status_t DRV_Keyboard_Init(void);
DRV_Status_t DRV_Keyboard_GetKeyValue(KEYBOARD_MSG_ITEM *pKeyboardMSG);


/***********************************************************************************
* Backup Camera.
***********************************************************************************/
DRV_Status_t DRV_BackupCam_Init(void); 
DRV_Status_t DRV_BackupCam_CheckBackupState(UINT8 *bValue);
DRV_Status_t DRV_BackupCam_CheckIsIOPControl(UINT8* bValue);
DRV_Status_t DRV_BackupCam_IOPControlCamDisable(void);


