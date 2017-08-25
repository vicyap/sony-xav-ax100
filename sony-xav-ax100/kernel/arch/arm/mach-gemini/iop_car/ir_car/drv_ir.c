/******************************************************************************/
/**
 *
 * @file		gl_Ir.c
 *
 * @brief       IR driver
 *
 *
 * @note        Copyright (c) 2007 Sunplus Technology Co., Ltd.  	\n
 *              All rights reserved.								\n
 *                   												\n
 *
 *              1. Last Modified: 2005/04/07   						\n
 *
 * @author
 *
 ******************************************************************************/
/*******************************************************************************
* Header include
******************************************************************************/
//#include "drivers/dtv_types.h" //Kaney mark
//#include "gsl.h" //Kaney mark
//#include "drivers/drv_ir.h" //Kaney mark
#include <iop_car/platform_define.h> //Kaney add
#include <iop_car/customize.h> //Kaney add
#include <iop_car/drv_iop.h> //Kaney add
#include <iop_car/drv_ir.h> //Kaney add
#include <iop_car/regmap.h> //Kaney add
#include <linux/string.h> //Kaney add
#include <linux/module.h> //Kaney add
/*******************************************************************************
* Constant
******************************************************************************/

/*******************************************************************************
* Structure
******************************************************************************/
static DRV_IRStandbyCfg_t gstIrStandbyCfg = {0};

/*******************************************************************************
* Variable
******************************************************************************/

/*******************************************************************************
* Program
******************************************************************************/

/******************************************************************************/
/**
* @brief    Initialize the IR module, This allocates any required resources,
*           The IR input is disabled by defaulr upon initialization
*
* @param    Uint32 dCustomerID: Set 0xFFFFFFFF to ignore customer code check
*
* @return    GL_Status_t    GL_SUCCESS
*
*******************************************************************************/
DRV_Status_t DRV_IRInit(UINT32 dCustomerID)
{
#if 1
	DRV_IOPIrInit();
	DRV_IOPIrSetCID(dCustomerID);
#else	
	SP_RCIR_Init(dCustomerID);
#endif
	return DRV_SUCCESS;
}

DRV_Status_t DRV_IRStandby_SetConfig(DRV_IRStandbyCfg_t stConfig)
{
	gstIrStandbyCfg = stConfig;
	return DRV_SUCCESS;
}

void DRV_IRStandby_SetupIop(void)
{
	UINT8 i;
	
	if(gstIrStandbyCfg.bCmdCnt > IR_STANDBY_SERVICED_CMD_COUNT)
	{
		gstIrStandbyCfg.bCmdCnt = IR_STANDBY_SERVICED_CMD_COUNT;
	}
	
	UINT8 bCustomerId = (UINT8)(gstIrStandbyCfg.dCustomerID & 0xff); //spike.yang, Temp.
	UINT8 abCmd[IR_STANDBY_SERVICED_CMD_COUNT] = {0};
	
	for(i=0; i<IR_STANDBY_SERVICED_CMD_COUNT; i++)
	{
		abCmd[i] = gstIrStandbyCfg.abCmd[i];
	}
	
	//Setup IR-IOP Data
	HWREG_W(iop_data[1], (bCustomerId | (abCmd[0]<<8)));
	HWREG_W(iop_data[2], (abCmd[1] | (abCmd[2]<<8)));
	
	//check
	printk("[IR Drv] bCustomerId = %2x\n",bCustomerId);
	printk("[IR Drv] %s, iop_data[1][2]= [%4x][%4x]\n",__FUNCTION__, HWREG_R(iop_data[1]), HWREG_R(iop_data[2]));
}

/******************************************************************************/
/**
* @brief    Un-initialize the IR module. This frees currently used resources
*
* @param    void
*
* @return    GL_Status_t    GL_SUCCESS
*
*******************************************************************************/
DRV_Status_t DRV_IRUnInit(void)
{
	return DRV_IOPIrUninit();
}


/******************************************************************************/
/**
* @brief    Enable the IR device.  While the IR device is enabled,
*            IR remote keypresses are received and queued up.
*
* @param    void
*
* @return    GL_SUCCESS if successful

*
*******************************************************************************/
DRV_Status_t DRV_IREnable (void)
{
	return 	DRV_IOPIrEnable();
}
/******************************************************************************/
/**
* @brief    Disable the IR device.  While the IR device is disabled,
*            all IR remote keypresses are discarded.
*
* @param    void
*
* @return    GL_SUCCESS if successful
*
*******************************************************************************/
DRV_Status_t DRV_IRDisable (void)
{
	return DRV_IOPIrDisable();
}

/******************************************************************************/
/**
* @brief    Get the IR code.  This function blocks until an IR code is received
*           or until dTimeout milliseconds have passed, whichever comes first.
*           If dTimeout is set to GL_INFINITE_WAIT (-1), it will wait indefinitely.
*
* @param    pdRetIrCode	received IR code (return value)
*           dTimeout		timeout (in milliseconds) to wait for an IR code to be received

*
* @return    GL_SUCCESS if successful
*           GL_ERR_INVALID_PARAM if return pointer is NULL
*           GL_ERR_TIMEOUT if IR code was not received within specified timeout

*
*******************************************************************************/
DRV_Status_t DRV_IRGetValue(UINT32* pdRetIrCode, UINT32 *pdRetIrMode, UINT32 dTimeout)
{

	if (pdRetIrCode ==  NULL)
	{
	 	return DRV_ERR_INVALID_PARAM;
	}

#if 1
	return DRV_IOPIrGetValue(pdRetIrCode, pdRetIrMode, dTimeout);
#else
	*pdRetIrCode = SP_RCIR_ReadKey(3);

	printk("\n\n\n[Rex] IR value = 0x%x\n", *pdRetIrCode);
	
	if(*pdRetIrCode==0x00ff)
	{
		return DRV_ERR_TIMEOUT;
	}
	else
	{
		*pdRetIrCode = (*pdRetIrCode&0x0000FFFF);
		return DRV_SUCCESS;
	}
#endif	
}

/******************************************************************************/
/**
* @brief    Set IR device Repeat Rate. 
*
* @param    UINT8 bRate: The Repeat Rate (it should not above 0xf)
*
* @return    GL_SUCCESS if successful
*
*******************************************************************************/
DRV_Status_t DRV_IRSetRate(UINT8 bRate)
{	
	return DRV_IOPIrSetRate(bRate);
}

/******************************************************************************/
/**
* @brief    Set IR device Repeat Rate to default value. 
*
* @param    void
*
* @return    GL_SUCCESS if successful
*
*******************************************************************************/
DRV_Status_t DRV_IRSetRateDefault(void)
{
	return DRV_IOPIrSetRateDefault();
}

#if 1 //Kaney add
int ir_Standby_Initialize(UINT8* pIrStandbyTable, UINT8 bCmdCount, UINT32 dCustomCode)
{
	
	UINT8 i;
	DRV_IRStandbySupCmd_t *pTable = (DRV_IRStandbySupCmd_t *) pIrStandbyTable;
	UINT8 bCmdCnt = bCmdCount;
	
	
	//Error Handling
	if(!bCmdCnt || (pIrStandbyTable == NULL))
	{
		return 0;
	}
	else if(bCmdCnt > IR_STANDBY_SERVICED_CMD_COUNT)
	{
		bCmdCnt = IR_STANDBY_SERVICED_CMD_COUNT;
	}
	
	DRV_IRStandbyCfg_t stIrStandbyCfg = {0};
	stIrStandbyCfg.dCustomerID	= dCustomCode;
	stIrStandbyCfg.bCmdCnt		= bCmdCnt;
	
	for(i=0; i<bCmdCnt; i++)
	{
		stIrStandbyCfg.abCmd[i] = pTable[i].IrCmd;
	}
	
	DRV_IRStandby_SetConfig(stIrStandbyCfg);
	
	return 1;
}

vfunc ir_Standby_Get_SetupIopFunct(void)
{
	return DRV_IRStandby_SetupIop;
}
#endif