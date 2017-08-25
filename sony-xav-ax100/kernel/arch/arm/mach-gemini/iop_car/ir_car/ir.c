/******************************************************
 IR

 This driver controls the IR component.

 Copyright (c) 2006 Sunplus, Inc.
 All rights reserved.
*******************************************************/
//#include "config_sys.h" //Kaney mark
//#include "drivers/drv_iop.h" //Kaney mark
//#include "drivers/drv_ir.h" //Kaney mark
#include <iop_car/platform_define.h> //Kaney add
#include <iop_car/customize.h> //Kaney add
#include <iop_car/drv_iop.h> //Kaney add
#include <iop_car/drv_ir.h> //Kaney add
#include <iop_car/iop.h>
//#include "gsl.h" //Kaney mark
#include <iop_car/regmap.h> //Kaney add
#include "ir_local.h" //Kaney add

/****************************************************************************
*                          Constant Definition                        *
****************************************************************************/
//#define IR_KEY_TEST
#define REG_R(addr)			(*((volatile UINT16 *)(addr)))
#define REG_W(addr, value)	(*((volatile UINT16 *)(addr)) = ((UINT16)(value)))
//#define IR_RDY	(1 << 15) //This defined in iop.h
#define IR_REPEAT_KEY_NUM	4

/*****************************************************************************
*                           STATIC    D A T A                               *
*****************************************************************************/
extern const unsigned char IOPcode_NEC[];
extern const unsigned char IOPcode_PHILIPS[];
static volatile UINT16 wCustomID = IR_INVALID_VALUE;
static DRV_IOPIrType_t irType;
//static GL_Flag_t ir_flag; //Kaney mark
//static Boolean fInit = FALSE; //Kaney mark
//static Boolean fEnable = FALSE; //Kaney mark
static bool fInit = FALSE; //Kaney add
static bool fEnable = FALSE; //Kaney add
//static UINT32 dIOPBase = CYGNUM_HAL_MEMRGN_IOP_START;
static DRV_IOPIrType_t irType = DRV_IOP_IrType_MAX;
//2010/4/26 lizhan mantis 0082122 fix the eup green power ui spec.START {{{
#ifdef SUPPORT_EUP_AUTO_STANDBY 
static vfunc g_pfResetPDTimeCallbk = NULL;
#endif
//2010/4/26 lizhan mantis 0082122 fix the eup green power ui spec. END }}}

/*****************************************************************************
                         STATIC    FUNCTIONS
*****************************************************************************/
//IOP ISR&DSR moved to iop.c
static UINT8 get_last_cid(void)
{
	UINT16 wLastCid;
	UINT16 wKey;

	/* CID is vaild only if we have a valid key */
	wKey = (UINT16) HWREG_R(iop_data[5]);
	if ((wKey & IR_RDY) == 0)
	{
		return 0x00;
	}

	wLastCid = (UINT16) HWREG_R(iop_data[6]);
	if (irType == DRV_IOP_IrType_PHILIPS)
	{
		return (UINT8) ((wLastCid >> 6) & 0x5f);
	}

	return (UINT8) wLastCid;
}

static UINT8 reverse_bits(UINT8 bKey)
{
	UINT8 bRevKey = 0;
	UINT32 i;

	for (i = 0; i < 8; i++)
	{
		bRevKey = (bRevKey << 1) | (bKey & 0x01);
		bKey = bKey >> 1;
	}

	return bRevKey;
}

static UINT16 get_key(void)
{
	UINT16 wKey;
	UINT16 tempdata;
    UINT16 wRet; // for key down/up mode
    
	wKey = (UINT16) HWREG_R(iop_data[5]);
	tempdata = wKey;
	if ((wKey & IR_RDY) == 0)
	{
		return IR_INVALID_VALUE;
	}
	HWREG_W(iop_data[5], wKey & ~(IR_RDY));
	#ifdef SUPPORT_IR_SPEEDUP
	if((wKey & IR_REPEAT_STOP) != 0)
	{				
		HWREG_W(iop_data[5], wKey & ~(IR_REPEAT_STOP));			
		DRV_IOPIrSetRateDefault();	
	}
	else if ((wKey & IR_REPEAT) != 0)
	{		
		DRV_IOPIrSetRate(0);	
	}
	#endif	
	if (irType == DRV_IOP_IrType_PHILIPS)
	{
		//return (UINT8) (wKey & 0x003f);
		wRet = (wKey & 0x003f) | (wKey & IR_REP); // | (wKey & IR_REP) for key down/up mode
		return wRet;
	}

	//wKey = reverse_bits((UINT8) wKey);
	wRet = (UINT16)reverse_bits((UINT8) wKey) | (wKey & IR_REP); // | (wKey & IR_REP) for key down/up mode
#if 0 //spike.yang, IR Respond Rate must be variable.
	if ((tempdata & (1 << 14)) != 0)
	{
		HWREG_W(iop_data[5], HWREG_R(iop_data[5]) | (IR_FIRST_KEY_INTERVAL << 8));
		
#if 0
		if (SP_RCIR_CheckRepeatKey(wKey) == 0)
		{
			return IR_INVALID_VALUE;
		}
#endif
	}
#endif
	//return wKey;
	return wRet;
}

/***************************************************
Prototype:
	DRV_Status_t DRV_IOPIrInit(void)
Parameters:
Returns:
	DRV_SUCCESS: if successful
	DRV_ERR_FAILURE: if driver cannot be initialized
	DRV_WARN_NO_ACTION: if the driver was previously initialized
Description:
	Initialize the IR module. This allocates any required resources.
	The IR input is disabled by default upon initialization.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPIrInit(void)
{
	if (fInit == TRUE)
	{
		return DRV_WARN_NO_ACTION;
	}

	wCustomID = IR_INVALID_VALUE;
	fEnable = FALSE;
	irType = DRV_IOP_IrType_MAX;
	//GL_FlagCreate("DRVIOP FLAG",&ir_flag); //Kaney mark
	
	fInit = TRUE;

	return DRV_SUCCESS;
}

DRV_Status_t DRV_IOPIrIsInited(void)
{
	if (fInit == TRUE)
		return DRV_SUCCESS;

	return DRV_ERR_FAILURE;
}

void DRV_IOPIrDSR(void)
{
	//GL_FlagSetbits(ir_flag, 1); //Kaney mark
	return;
}

/***************************************************
Prototype:
	DRV_Status_t DRV_IOPIrUninit(void)
Parameters:
Returns:
	DRV_SUCCESS: if successful
	DRV_ERR_FAILURE: if driver cannot be uninitialized
	DRV_WARN_NO_ACTION: if the driver was previously uninitialized
Description:
	Un-initialize the IR module. This frees currently used resources.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPIrUninit(void)
{
	if (fInit == FALSE)
	{
		return DRV_WARN_NO_ACTION;
	}

	wCustomID = IR_INVALID_VALUE;
	fEnable = FALSE;
	irType = DRV_IOP_IrType_MAX;
	fInit = FALSE;


	return DRV_SUCCESS;
}

/***************************************************
Prototype:
	DRV_Status_t DRV_IOPIrEnable(void)
Parameters:
Returns:
	DRV_SUCCESS: if successful
	DRV_ERR_FAILURE: if IR cannot be disabled
Description:
	Enable the IR device. While the IR device is enabled,
	IR remote keypresses are received and queued up.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPIrEnable(void)
{
	fEnable = TRUE;
	get_key();					/* skip old key */
	return DRV_SUCCESS;
}

/***************************************************
Prototype:
	DRV_Status_t DRV_IOPIrDisable(void)
Parameters:
Returns:
DRV_SUCCESS: if successful
	DRV_ERR_FAILURE: if IR cannot be disabled
Description:
	Disable the IR device. While the IR device is disabled,
	all IR remote keypresses are discarded.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPIrDisable(void)
{
	fEnable = FALSE;
	return DRV_SUCCESS;
}
#if 0
/***************************************************
Prototype:
	DRV_Status_t DRV_IOPIrSetType(DRV_IOPIrType_t type)
Parameters:
	type: identifies the IR type
Returns:
	DRV_SUCCESS: if successful
	DRV_ERR_INVALID_PARAM: if type is not appropriate
Description:
	Set the IR type as NEC, PHILIPS Ketc.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPIrSetType(DRV_IOPIrType_t type)
{
	UINT8 *pbIOPCode;
	DRV_Status_t drvStatus;

	if (type == DRV_IOP_IrType_NEC)
	{
		pbIOPCode = (UINT8 *) IOPcode_NEC;
	}
	else if (type == DRV_IOP_IrType_PHILIPS)
	{
		pbIOPCode = (UINT8 *) IOPcode_PHILIPS;
	}
	else
	{
		return DRV_ERR_INVALID_PARAM;
	}
	irType = type;

	drvStatus = DRV_IOPInit(CYGNUM_HAL_MEMRGN_IOP_START, IOP_CODE_SIZE, (UINT8 *) pbIOPCode);

	if (drvStatus != DRV_SUCCESS)
	{
		return DRV_ERR_INVALID_PARAM;
	}

	irType = type;				/* keep it in global variable */
	get_key();					/* skip old key */

	return DRV_SUCCESS;
}
#endif
/***************************************************
Prototype:
	DRV_Status_t DRV_IOPIrGetType(DRV_IOPIrType_t* peRetType)
Parameters:
	peRetType: identifies the IR type (return value)
Returns:
	DRV_SUCCESS: if successful
	DRV_ERR_INVALID_PARAM: if return pointer is NULL
	DRV_ERR_FAILURE:  if IR type has not yet been set
Description:
	Gets the current IR type.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPIrGetType(DRV_IOPIrType_t * peRetType)
{
	if (peRetType == NULL)
	{
		return DRV_ERR_INVALID_PARAM;
	}

	if (irType == DRV_IOP_IrType_MAX)
	{
		return DRV_ERR_FAILURE;
	}

	*peRetType = irType;

	return DRV_SUCCESS;
}

/***************************************************
Prototype:
	DRV_Status_t DRV_IOPIrSetCID(UINT32 dCustomerID)
Parameters:
	dCustomerID: identifies the customer ID
Returns:
	DRV_SUCCESS: if successful
	DRV_ERR_INVALID_PARAM: if customer ID is not appropriate
Description:
	Set the IR customer ID.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPIrSetCID(UINT32 dCustomerID)
{
	if ((dCustomerID & 0xffffff00) != 0)
	{
		return DRV_ERR_INVALID_PARAM;
	}
	wCustomID = (UINT16) dCustomerID;
	get_key();					/* skip old key */

	return DRV_SUCCESS;
}

/***************************************************
Prototype:
	DRV_Status_t DRV_IOPIrGetCID(UINT32 *pdRetCustomerID)
Parameters:
	pdRetCustomerID: current customer ID (return value)
Returns:
	DRV_SUCCESS: if successful
	DRV_ERR_INVALID_PARAM: if return pointer is NULL
	DRV_ERR_FAILURE: if CID has not yet been set
Description:
	Get the IR current customer ID.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPIrGetCID(UINT32 * pdRetCustomerID)
{
	if (pdRetCustomerID == NULL)
	{
		return DRV_ERR_INVALID_PARAM;
	}
	if (wCustomID == IR_INVALID_VALUE)
	{
		return DRV_ERR_FAILURE;
	}
	*pdRetCustomerID = (UINT32) wCustomID;

	return DRV_SUCCESS;
}

/***************************************************
Prototype:
	DRV_Status_t DRV_IOPIrQueryCID(UINT32 *pdRetCustomerID)
Parameters:
	pdRetCustomerID: customer ID last received (return value)
Returns:
	DRV_SUCCESS: if successful
	DRV_ERR_INVALID_PARAM: if return pointer is NULL
Description:
	Get the IR customer ID last received
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPIrQueryCID(UINT32 * pdRetCustomerID)
{
	if (pdRetCustomerID == NULL)
	{
		return DRV_ERR_INVALID_PARAM;
	}
	*pdRetCustomerID = get_last_cid();

	return DRV_SUCCESS;
}

//2010/3/31 lizhan mantis 0082122 fix the eup green power ui spec.START {{{
#ifdef SUPPORT_EUP_AUTO_STANDBY //Reset power down time;
void DRV_IOPIr_ResetPDTime_callback(vfunc pfResetPDTimeCallbk)
{
	g_pfResetPDTimeCallbk = pfResetPDTimeCallbk;
}
#endif
//2010/3/31 lizhan mantis 0082122 fix the eup green power ui spec. END }}}


/***************************************************
Prototype:
	DRV_Status_t DRV_IOPIrGetValue(UINT32 *pdRetIrCode, UINT32 *pdRetIrMode, UINT32 dTimeout) // pdRetIrMode for key down/up mode
Parameters:
	pdRetIrCode: received IR code (return value)
	dTimeout: timeout (in milliseconds) to wait for an IR code to be received
Returns:
	DRV_SUCCESS: if successful
	DRV_ERR_INVALID_PARAM: if return pointer is NULL
	DRV_ERR_TIMEOUT: if IR code was not received within specified timeout
	DRV_ERR_FAILURE: if IR was disabled
Description:
	Get the IR code. This function blocks until an IR code is received or
	until dTimeout milliseconds have passed, whichever comes first.
	If dTimeout is set to DRV_TIMEOUT_FOREVER (0), it will wait indefinitely.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPIrGetValue(UINT32 * pdRetIrCode, UINT32 *pdRetIrMode, UINT32 dTimeout) // pdRetIrMode for key down/up mode
{
	UINT16 wCode = 0;
	UINT16 wCid = 0;  //wsc 20100325 add
	//UINT32 dTime = 0;

	if (pdRetIrCode == NULL)
	{
		return DRV_ERR_INVALID_PARAM;
	}

	if (fEnable == FALSE)
	{
		return DRV_ERR_FAILURE;
	}
	/*if (dTimeout == DRV_TIMEOUT_FOREVER) //Kaney mark
	{
		GL_FlagWait(ir_flag, 1, GL_FLAG_WAITMODE_CLR|GL_FLAG_WAITMODE_OR,
					GL_INFINITE_WAIT);
	}
	else
	{
		if (GL_FlagWait(ir_flag, 1, GL_FLAG_WAITMODE_CLR|GL_FLAG_WAITMODE_OR,
			dTimeout) == 0)
		{
			return DRV_ERR_TIMEOUT;
		}
	}*/

	//wsc 20100325 modify begin{{{
	wCid = get_last_cid();
	#ifdef IR_KEY_TEST
	printk("[IR]: CID=0x%x, KeyVal=0x%x\n", wCid, get_key());
	#endif
	
	if (wCid == wCustomID)
	//wsc 20100325 modify end}}}
	{
		wCode = get_key();
		//william : Return raw ir data,
		//if (wCode != IR_INVALID_VALUE)
		{
			//*pdRetIrCode = (UINT32) wCode;
			*pdRetIrCode = (UINT32) wCode & 0xff; // & 0xff for key down/up mode
			*pdRetIrMode = (UINT32) wCode & IR_REP ; // for key down/up mode
            //printk("[IR]: CID=0x%x, KeyVal=0x%x, KeyMode=0x%x\n", wCid, *pdRetIrCode, *pdRetIrMode); // pdRetIrMode for key down/up mode //Kaney add
			//2010/3/31 lizhan mantis 0082122 fix the eup green power ui spec.START {{{
			#ifdef SUPPORT_EUP_AUTO_STANDBY //2010/4/26 lizhan modify
			if(g_pfResetPDTimeCallbk)
			{
				g_pfResetPDTimeCallbk();
			}
			#endif
			//2010/3/31 lizhan mantis 0082122 fix the eup green power ui spec. END }}}
			return DRV_SUCCESS;
		}
	}

	return DRV_ERR_TIMEOUT;
}
/***************************************************
Prototype:
	DRV_Status_t DRV_IOPIrGetValue_debug(UINT32 *pdRetIrCode, UINT32 dTimeout,UINT16 *pCid)
Parameters:
	pdRetIrCode: received IR code (return value)
	pCid:received IR HEADER (return value)
	dTimeout: timeout (in milliseconds) to wait for an IR code to be received
Returns:
	DRV_SUCCESS: if successful
	DRV_ERR_INVALID_PARAM: if return pointer is NULL
	DRV_ERR_TIMEOUT: if IR code was not received within specified timeout
	DRV_ERR_FAILURE: if IR was disabled
Description:
	Get the IR code. This function blocks until an IR code is received or
	until dTimeout milliseconds have passed, whichever comes first.
	If dTimeout is set to DRV_TIMEOUT_FOREVER (0), it will wait indefinitely.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPIrGetValue_debug(UINT32 * pdRetIrCode, UINT32 dTimeout,UINT16 *pCid)
{
	UINT16 wCode;
//	UINT32 dTime = 0;

	if (pdRetIrCode == NULL)
	{
		return DRV_ERR_INVALID_PARAM;
	}

	if (fEnable == FALSE)
	{
		return DRV_ERR_FAILURE;
	}
	/*if (dTimeout == DRV_TIMEOUT_FOREVER) //Kaney mark
	{
		GL_FlagWait(ir_flag, 1, GL_FLAG_WAITMODE_CLR|GL_FLAG_WAITMODE_OR,
					GL_INFINITE_WAIT);
	}
	else
	{
		if (GL_FlagWait(ir_flag, 1, GL_FLAG_WAITMODE_CLR|GL_FLAG_WAITMODE_OR,
			dTimeout) == 0)
			return DRV_ERR_TIMEOUT;
	}*/
	*pCid=get_last_cid();
	//wCode = get_key();
	wCode = get_key() & 0xff;
	*pdRetIrCode = (UINT32) wCode;
	return DRV_SUCCESS;
}
/***************************************************
Prototype:
	DRV_Status_t DRV_IOPIrFlushKeys(void)
Parameters:
Returns:
	DRV_SUCCESS: if successful
	DRV_ERR_FAILURE: if IR cannot be disabled
Description:
	Flush all the queued key events.
	Returns DRV_SUCCESS if driver does not maintain a queue.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPIrFlushKeys(void)
{
	get_key();					/* skip old key */
	return DRV_SUCCESS;
}

/***************************************************
Prototype:
	DRV_Status_t DRV_IOPIrSetRateDefault(void)
Parameters:
Returns:
	DRV_SUCCESS: if successful
	DRV_ERR_FAILURE: if IR cannot be disabled
Description:
	.
	.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPIrSetRateDefault(void)
{
	//Clear
	HWREG_W(iop_data[5], HWREG_R(iop_data[5]) & (~(0xf<<8)));
	
	//Set Repeat Rate
	HWREG_W(iop_data[5], HWREG_R(iop_data[5]) | (IR_FIRST_KEY_INTERVAL << 8));	
	
	return DRV_SUCCESS;
}


/***************************************************
Prototype:
	DRV_Status_t DRV_IOPIrSetRate(UINT8 bRate)
Parameters:
Returns:
	DRV_SUCCESS: if successful
	DRV_ERR_FAILURE: if IR cannot be disabled
Description:
	.
	.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPIrSetRate(UINT8 bRate)
{	
	if( (bRate&0xf0) > 0)
		return DRV_ERR_INVALID_PARAM;
	
	//Clear
	HWREG_W(iop_data[5], HWREG_R(iop_data[5]) & (~(0xf<<8)));
	
	//Set Repeat Rate
	HWREG_W(iop_data[5], HWREG_R(iop_data[5]) | (bRate << 8));
	
	return DRV_SUCCESS;
}



DRV_Status_t DRV_IOPIrSetSpeedup(void)		//zhangjie add for mantis 0149901
{	
	//Set Speedup 
	HWREG_W(iop_data[5], HWREG_R(iop_data[5]) | (1 << 11));
	
	return DRV_SUCCESS;
}

