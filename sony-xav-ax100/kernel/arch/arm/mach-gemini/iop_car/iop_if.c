/******************************************************
 Car IOP

 This APIs control the IOP component.

 Copyright (c) 2014 Sunplus, Inc.
 All rights reserved.
*******************************************************/

#include <iop_car/iop_if.h>
#include <mach/common.h>
#include <iop_car/regmap.h>
#include <iop_car/drv_common.h>
#include <linux/delay.h>
#include <mach/gpio_drv.h>
#include <mach/sp_customize_config.h>

extern UINT32 gIOPBufAddr;
//////////////////////////////////////////////////////////

int DRV_CheckIOPData3Flag(UINT16 wFlag)
{
	if (HWREG_R(iop_data[3]) & wFlag)
		return 1;
	else
		return 0;
}

void DRV_ClearIOPData3Flag(UINT16 wFlag)
{
	HWREG_W(iop_data[3], HWREG_R(iop_data[3]) & (~wFlag));
}

int Check_IOPData0Clear(void)
{
	UINT32 dTimeOutCnt = 100;
	
	while(((HWREG_R(iop_data[0]) >> 12) & 0xF) != 0)
	{
		dTimeOutCnt--;
		if(dTimeOutCnt == 0)
		{	
			printk("[IOP]>>> Error(wait iop data0 clear timeout)................!!!! <<<\n");
			return IOP_WAIT_CLEAR_TIMEOUT;
		}		
		msleep(1);
	}

	return IOP_SUCCESS;
}

void GetMsgFromIOPData1(UINT8* pParam1, UINT8* pParam2)
{
	*pParam1=(HWREG_R(iop_data[0]) >> 8) & 0xFF;
	*pParam2=HWREG_R(iop_data[0]) & 0xFF;
}

void GetMsgFromIOPData0(UINT8* pParam)
{
	*pParam=HWREG_R(iop_data[0]) & 0xFF;
}

void SendCmdToIOPData1(UINT8 bParam1,UINT8 bParam2)
{
	UINT16 wData1=0;
	
	wData1=IOP_DATA1_CMD(bParam1, bParam2);
	HWREG_W(iop_data[1], wData1);
}

void SendCmdToIOPData0(UINT8 bType, UINT8 bCmd ,UINT8 bParam)
{
	UINT16 wData0=0;
	
	wData0=IOP_DATA0_CMD(bType, bCmd, bParam);
	HWREG_W(iop_data[0], wData0);
}



/***********************************************************************************
* ±Û¶s(rotate).
***********************************************************************************/
int IOP_Rotate_SetGPIO(UINT8 bNum1, UINT8 bNum2)
{
	int dRet=IOP_SUCCESS;

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	SendCmdToIOPData1(bNum2, 0);	
	SendCmdToIOPData0(IOP_CMD_TYPE_ROATE, ROATE2IOP_SET_GPIO, bNum1);
	return dRet;
}

int IOP_Rotate_Debugmode(UINT8 bValue)
{
	int dRet=IOP_SUCCESS;

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	SendCmdToIOPData0(IOP_CMD_TYPE_ROATE, ROATE2IOP_SET_DEBUGMODE, bValue);
	return dRet;
}

int IOP_Rotate_GetDatalen(UINT8* pDatalen)
{
	UINT8 bParam=0;
	int dRet=IOP_SUCCESS;

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	SendCmdToIOPData0(IOP_CMD_TYPE_ROATE, ROATE2IOP_GET_DATALENGTH, 0);
	
	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	GetMsgFromIOPData0(&bParam);
	*pDatalen=bParam;

	return dRet;
}

int IOP_Rotate_SetMode(UINT8 bNum, UINT8 bOrder)
{
	int dRet=IOP_SUCCESS;

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	SendCmdToIOPData1(bOrder, 0);	
	SendCmdToIOPData0(IOP_CMD_TYPE_ROATE, ROATE2IOP_SET_MODE, bNum);
	return dRet;
}

int IOP_Rotate_GetBufAddress(UINT16* pAddress, UINT8* pBuflen)
{
	UINT8 bParam=0, bParam1=0, bParam2=0;
	int dRet=IOP_SUCCESS;

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	SendCmdToIOPData0(IOP_CMD_TYPE_ROATE, ROATE2IOP_GET_BUFADDRESS, 0);

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	GetMsgFromIOPData0(&bParam);
	GetMsgFromIOPData1(&bParam1, &bParam2);

	*pBuflen = bParam;
	*pAddress = (bParam1 << 8) | bParam2;
	
	return dRet;
}

int IOP_Rotate_Scan(UINT8 bValue)
{	
	int dRet=IOP_SUCCESS;

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	SendCmdToIOPData0(IOP_CMD_TYPE_ROATE, ROATE2IOP_SET_SCANONOFF, bValue);	
	return dRet;
}

DRV_Status_t DRV_Rotate_SetMode(void)
{
	UINT8* pbAddress;
	UINT16* pwAddress;
	UINT16 wOffset=0;

	
	UINT8 bBuflen=0, bDatalen=0, bOrder=0;
	int i=0;
	
	if(IOP_Rotate_Debugmode(SET_ENABLE) != IOP_SUCCESS)
		return DRV_ERR_FAILURE;

	if(IOP_Rotate_GetBufAddress(&wOffset, &bBuflen) != IOP_SUCCESS)
	{
		IOP_Rotate_Debugmode(SET_DISABLE);
		return DRV_ERR_FAILURE;
	}
	
	if(IOP_Rotate_GetDatalen(&bDatalen) != IOP_SUCCESS)
	{
		IOP_Rotate_Debugmode(SET_DISABLE);
		return DRV_ERR_FAILURE;
	}

	pwAddress = (UINT16 *)gIOPBufAddr + wOffset;
	pbAddress=(UINT8 *)pwAddress;

	for(i=0; i<bDatalen; i++)
	{
		bOrder= (bOrder<<2) | *(pbAddress+i) ;
	}
	
	if(IOP_Rotate_SetMode(bDatalen, bOrder) != IOP_SUCCESS)
	{
		IOP_Rotate_Debugmode(SET_DISABLE);
		return DRV_ERR_FAILURE;
	}
	
	if(IOP_Rotate_Debugmode(SET_DISABLE) != IOP_SUCCESS)
		return DRV_ERR_FAILURE;
	
	return DRV_SUCCESS;
}

DRV_Status_t DRV_Rotate_Init(void)
{
	if(IOP_Rotate_Scan(SET_ENABLE) != IOP_SUCCESS)
		return DRV_ERR_FAILURE;
	
	GPIO_F_SET(77,1);
	GPIO_M_SET(77,0);
	GPIO_F_SET(78,1);
	GPIO_M_SET(78,0);
	if(IOP_Rotate_SetGPIO(77, 78) != IOP_SUCCESS)
		return DRV_ERR_FAILURE;
	
	return DRV_Rotate_SetMode();
}

DRV_Status_t DRV_Rotate_GetKeyValue(ROTATE_MSG_ITEM *pRotateMSG)
{	
	UINT16* pwAddress;
	UINT16 wOffset=0;
	UINT8 bBuflen=0, bDatalen=0;
	int i=0;

	if (pRotateMSG == NULL)
	{
		return DRV_ERR_INVALID_PARAM;
	}
		
	if(IOP_Rotate_GetBufAddress(&wOffset, &bBuflen) != IOP_SUCCESS)
		return DRV_ERR_FAILURE;
	
	if(IOP_Rotate_GetDatalen(&bDatalen) != IOP_SUCCESS)
		return DRV_ERR_FAILURE;
	
	pwAddress = (UINT16 *)gIOPBufAddr + wOffset;
	
	for(i=0; i<(bDatalen/2); i++)
	{
		pRotateMSG->dDataBuffer[i][0]=(*(pwAddress+i)) >> 8;
		pRotateMSG->dDataBuffer[i][1]=(*(pwAddress+i)) & 0xFF; 
	}
	pRotateMSG->dDataNum=bDatalen/2;

	return DRV_SUCCESS;
}



/***********************************************************************************
* Keyboard.
***********************************************************************************/
int IOP_Keyboard_GetDatalen(UINT8* pDatalen)
{
	UINT8 bParam=0;
	int dRet=IOP_SUCCESS;

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	SendCmdToIOPData0(IOP_CMD_TYPE_KEYBOARD, KEYBOARD2IOP_GET_DATALENGTH, 0);
	
	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	GetMsgFromIOPData0(&bParam);
	*pDatalen=bParam;

	return dRet;
}

int IOP_Keyboard_SetAD(UINT8 bNum)
{	
	int dRet=IOP_SUCCESS;

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	SendCmdToIOPData0(IOP_CMD_TYPE_KEYBOARD, KEYBOARD2IOP_SET_AD, bNum);	
	return dRet;
}

int IOP_Keyboard_GetBufAddress(UINT16* pAddress, UINT8* pBuflen)
{
	UINT8 bParam=0, bParam1=0, bParam2=0;
	int dRet=IOP_SUCCESS;

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	SendCmdToIOPData0(IOP_CMD_TYPE_KEYBOARD, KEYBOARD2IOP_GET_BUFADDRESS, 0);

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	GetMsgFromIOPData0(&bParam);
	GetMsgFromIOPData1(&bParam1, &bParam2);

	*pBuflen = bParam;
	*pAddress = (bParam1 << 8) | bParam2;

	return dRet;
}

DRV_Status_t DRV_Keyboard_Init(void)
{	
	if(IOP_Keyboard_SetAD(0xF) != IOP_SUCCESS);	//set AD number for keyboard
		return DRV_ERR_FAILURE;
		
	return DRV_SUCCESS;
}

DRV_Status_t DRV_Keyboard_GetKeyValue(KEYBOARD_MSG_ITEM *pKeyboardMSG)
{	
	UINT16* pwAddress;
	UINT16 wOffset=0;
	UINT8 bBuflen=0, bDatalen=0;
	int i=0;

	if (pKeyboardMSG == NULL)
	{
		return DRV_ERR_INVALID_PARAM;
	}
		
	if(IOP_Keyboard_GetBufAddress(&wOffset, &bBuflen) != IOP_SUCCESS)
		return DRV_ERR_FAILURE;
	
	if(IOP_Keyboard_GetDatalen(&bDatalen) != IOP_SUCCESS)
		return DRV_ERR_FAILURE;
	
	pwAddress = (UINT16 *)gIOPBufAddr + wOffset;
	
	for(i=0; i<(bDatalen/2); i++)
	{
		pKeyboardMSG->dDataBuffer[i]=*(pwAddress+i);
	}
	pKeyboardMSG->dDataNum=bDatalen/2;

	return DRV_SUCCESS;
}



/***********************************************************************************
* Backup Camera.
***********************************************************************************/
int IOP_BackupCam_SetGPIO(UINT8 bNum)
{	
	int dRet=IOP_SUCCESS;

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;

	SendCmdToIOPData0(IOP_CMD_TYPE_BACKUPCAM, BACKUPCAM2IOP_SET_GPIO, bNum);
	return dRet;	
}

int IOP_BackupCam_CheckIsIOPControl(UINT8* bValue)
{
	UINT8 bParam=0;
	int dRet=IOP_SUCCESS;

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;

	SendCmdToIOPData0(IOP_CMD_TYPE_BACKUPCAM, BACKUPCAM2IOP_CHECK_ISIOPCTRL, 0);

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	GetMsgFromIOPData0(&bParam);
	*bValue=bParam;

	return dRet;
}

int IOP_BackupCam_SetIOPControlCam(UINT8 bValue)
{	
	int dRet=IOP_SUCCESS;

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	SendCmdToIOPData0(IOP_CMD_TYPE_BACKUPCAM, BACKUPCAM2IOP_SET_IOPCTRLCAMONOFF, bValue);
	return dRet;
}

int IOP_BackupCam_SetPanelResolution(UINT8 bValue)
{	
	int dRet=IOP_SUCCESS;

	dRet=Check_IOPData0Clear();
	if(dRet != IOP_SUCCESS)
		return dRet;
	
	SendCmdToIOPData0(IOP_CMD_TYPE_BACKUPCAM, BACKUPCAM2IOP_SET_PAN_RES, bValue);
	return dRet;
}

DRV_Status_t DRV_BackupCam_Init(void)
{
	int panelResolution = 0, reverseDetectIO = 1;
	if (getConfigInt("panel_resolution", &panelResolution))
	{
		//printf("Iop get panel resolution fail,use default\n");
		if(IOP_BackupCam_SetPanelResolution(PAN_RES_0) != IOP_SUCCESS)
			return DRV_ERR_FAILURE;
	}
	else
	{
		//printf("Iop get panel resolution:%d\n",panelResolution);
		if(IOP_BackupCam_SetPanelResolution(panelResolution) != IOP_SUCCESS)
		return DRV_ERR_FAILURE;
	}

	if (getConfigInt("gpio_car_reverse_detect", &reverseDetectIO))
	{
		//printf("Iop get gpio_car_reverse_detect,use default\n");
		GPIO_F_SET(27,1);
		GPIO_M_SET(27,0);
		if(IOP_BackupCam_SetGPIO(27) != IOP_SUCCESS)
			return DRV_ERR_FAILURE;
	}
	else
	{
		//printf("Iop get gpio_car_reverse_detect:%x\n",reverseDetectIO);
		GPIO_F_SET(reverseDetectIO,1);
		GPIO_M_SET(reverseDetectIO,0);
		if(IOP_BackupCam_SetGPIO(reverseDetectIO) != IOP_SUCCESS)	
			return DRV_ERR_FAILURE;
	}
	return DRV_SUCCESS;	
}

DRV_Status_t DRV_BackupCam_CheckBackupState(UINT8 *bValue)
{
	if (HWREG_R(iop_data[3]) & BACKUPCAM_STATE)
		*bValue = INTO_REVERSE;
	else
		*bValue = EXIT_REVERSE;

	return DRV_SUCCESS;
}

DRV_Status_t DRV_BackupCam_CheckIsIOPControl(UINT8* bValue) 
{
	if(IOP_BackupCam_CheckIsIOPControl(bValue) != IOP_SUCCESS)
		return DRV_ERR_FAILURE;
			
	return DRV_SUCCESS;
}

DRV_Status_t DRV_BackupCam_IOPControlCamDisable(void) 
{
	if(IOP_BackupCam_SetIOPControlCam(0) != IOP_SUCCESS)
		return DRV_ERR_FAILURE;
			
	return DRV_SUCCESS;
}




