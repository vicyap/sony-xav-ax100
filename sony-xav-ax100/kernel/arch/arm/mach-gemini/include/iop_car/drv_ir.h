/**@file	drv_ir.h
*@brief	This driver controls the IR component.
*@note	Copyright (c) 2006 Sunplus, Inc.
 		All rights reserved.
*/

#ifndef DRV_IR_H
#define DRV_IR_H
#include "drv_common.h"
#include <mach/common.h> //Kaney add

#define IR_FIRST_KEY_INTERVAL	2//3	/**<IR Key Interval	*///xsyan modify for mantis 86292 20100517
#define IR_INVALID_VALUE	0x100	/**<IR Invalid Value	*/

#define IR_STANDBY_SERVICED_CMD_COUNT	3	/**<3 key can wake  up platform in standby	*/

/** IR Type */
typedef enum
{
	DRV_IOP_IrType_NEC,
	DRV_IOP_IrType_PHILIPS,
	DRV_IOP_IrType_MAX
} DRV_IOPIrType_t;

/** IR Standby  Command struct */
typedef struct
{
	UINT8	IrCmd;		/**< TCommand is serviced in standby mod.*/
	UINT32	SysCmd;	/**< Action after iop turning on system.*/

} DRV_IRStandbySupCmd_t;

/** IR Standby  Config struct */
typedef struct
{
	UINT32	dCustomerID;		/**< Costomer ID*/
	UINT8	bCmdCnt;		/**< Command Count*/
	UINT8	abCmd[IR_STANDBY_SERVICED_CMD_COUNT];	/**< IR Command Value*/

} DRV_IRStandbyCfg_t;

/**********************************************************************/
/**
* @fn DRV_Status_t DRV_IOPIrInit(void)
*
* @brief Initialize the IR module
*
*Initialize the IR module. This allocates any required resources.
*The IR input is disabled by default upon initialization.
*
* @return 	DRV_SUCCESS: if successful
* @return 	DRV_ERR_FAILURE: if driver cannot be initialized
* @return 	DRV_WARN_NO_ACTION: if the driver was previously initialized
*
* @note	None
**********************************************************************/
DRV_Status_t DRV_IOPIrInit(void);

/**********************************************************************/
/**
* @fn DRV_Status_t DRV_IOPIrUninit(void)
*
* @brief	Un-initialize the IR module
*
*Un-initialize the IR module. This frees currently used resources.
*
* @return 	DRV_SUCCESS: if successful
* @return 	DRV_ERR_FAILURE: if driver cannot be uninitialized
* @return 	DRV_WARN_NO_ACTION: if the driver was previously uninitialized
*
* @note	None
**********************************************************************/
DRV_Status_t DRV_IOPIrUninit(void);

/**********************************************************************/
/**
* @fn DRV_Status_t DRV_IOPIrEnable(void)
*
*@brief	Enable the IR device
*
*Enable the IR device. While the IR device is enabled,
*IR remote keypresses are received and queued up.
*
* @return 	DRV_SUCCESS: if successful
* @return 	DRV_ERR_FAILURE: if IR cannot be disabled
*
* @note	None
**********************************************************************/
DRV_Status_t DRV_IOPIrEnable (void);

/**********************************************************************/
/**
* @fn DRV_Status_t DRV_IOPIrDisable(void)
*
*@brief	Disable the IR device
*
*Disable the IR device. While the IR device is disabled,
*all IR remote keypresses are discarded.
*
* @return 	DRV_SUCCESS: if successful
* @return 	DRV_ERR_FAILURE: if IR cannot be disabled
*
* @note	None
**********************************************************************/
DRV_Status_t DRV_IOPIrDisable(void);

#if 0
/**********************************************************************/
/**
* @fn DRV_Status_t DRV_IOPIrSetType(void)
*
*@brief	Set the IR type
*
*Set the IR type as NEC, PHILIPS¡Ketc.
*
* @param	type : identifies the IR type
*
* @return 	DRV_SUCCESS: if successful
* @return 	DRV_ERR_INVALID_PARAM: if type is not appropriate
*
* @note	None
**********************************************************************/
DRV_Status_t DRV_IOPIrSetType(DRV_IOPIrType_t type);
#endif

/**********************************************************************/
/**
* @fn DRV_Status_t DRV_IOPIrGetType(DRV_IOPIrType_t *peRetType)
*
*@brief	Gets the current IR type
*
* @param	peRetType : Ir Type(return value)
*
* @return 	DRV_SUCCESS: if successful
* @return 	DRV_ERR_INVALID_PARAM: if return pointer is DTV_NULL
* @return 	DRV_ERR_FAILURE:  if IR type has not yet been set
*
* @note	None
**********************************************************************/
DRV_Status_t DRV_IOPIrGetType(DRV_IOPIrType_t *peRetType);

/**********************************************************************/
/**
* @fn DRV_Status_t DRV_IOPIrSetCID(UINT32 dCustomerID)
*
*@brief	Set the IR customer ID
*
* @param	dCustomerID : identifies the customer ID
*
* @return 	DRV_SUCCESS: if successful
* @return 	DRV_ERR_INVALID_PARAM: if customer ID is not appropriate
*
* @note	None
**********************************************************************/
DRV_Status_t DRV_IOPIrSetCID(UINT32 dCustomerID);

/**********************************************************************/
/**
* @fn DRV_Status_t DRV_IOPIrGetCID(UINT32 *pdRetCustomerID)
*
*@brief	Get the IR current customer ID
*
* @param	pdRetCustomerID : current customer ID (return value)
*
* @return 	DRV_SUCCESS: if successful
* @return 	DRV_ERR_INVALID_PARAM: if return pointer is DTV_NULL
* @return 	DRV_ERR_FAILURE: if CID has not yet been set
*
* @note	None
**********************************************************************/
DRV_Status_t DRV_IOPIrGetCID(UINT32 *pdRetCustomerID);

/**********************************************************************/
/**
* @fn DRV_Status_t DRV_IOPIrQueryCID(UINT32 *pdRetCustomerID)
*
*@brief	Get the IR customer ID last received
*
* @param	pdRetCustomerID : customer ID last received (return value)
*
* @return 	DRV_SUCCESS: if successful
* @return 	DRV_ERR_INVALID_PARAM: if return pointer is DTV_NULL
*
* @note	None
**********************************************************************/
DRV_Status_t DRV_IOPIrQueryCID(UINT32 *pdRetCustomerID);

/**********************************************************************/
/**
* @fn DRV_Status_t DRV_IOPIrGetValue(UINT32 *pdRetIrCode, UINT32 dTimeout)
*
*@brief	Get the IR code
*
*Get the IR code. This function blocks until an IR code is received or
*until dTimeout milliseconds have passed, whichever comes first.
*If dTimeout is set to DRV_TIMEOUT_FOREVER (0), it will wait indefinitely.
*
* @param	pdRetIrCode : received IR code (return value)
* @param	dTimeout : timeout (in milliseconds) to wait for an IR code to be received
*
* @return 	DRV_SUCCESS: if successful
* @return 	DRV_ERR_INVALID_PARAM: if return pointer is DTV_NULL
* @return 	DRV_ERR_TIMEOUT: if IR code was not received within specified timeout
* @return 	DRV_ERR_FAILURE: if IR was disabled
*
* @note	None
**********************************************************************/
DRV_Status_t DRV_IOPIrGetValue(UINT32 *pdRetIrCode, UINT32 *pdRetIrMode, UINT32 dTimeout);

/**********************************************************************/
/**
* @fn DRV_Status_t DRV_IOPIrFlushKeys(void)
*
*@brief	Flush all the queued key events
*
*Flush all the queued key events.
*Returns DRV_SUCCESS if driver does not maintain a queue.
*
* @return 	DRV_SUCCESS: if successful
* @return 	DRV_ERR_FAILURE: if IR cannot be disabled
*
* @note	None
**********************************************************************/
DRV_Status_t DRV_IOPIrFlushKeys(void);

/**********************************************************************/
/**
* @fn DRV_Status_t DRV_IOPIrSetRate(UINT8 bRate)
*
*@brief	Set IR Repeat Rate
*
* @param	bRate : Set IR Repeat Rate
*
* @return 	DRV_SUCCESS: if successful
*
* @note	None
**********************************************************************/
DRV_Status_t DRV_IOPIrSetRate(UINT8 bRate);



DRV_Status_t DRV_IOPIrSetSpeedup(void);				//zhangjie add for mantis 0149901

/**********************************************************************/
/**
* @fn DRV_Status_t DRV_IOPIrSetRateDefault(void)
*
*@brief	Set IR Repeat Rate to default value (by IR_FIRST_KEY_INTERVAL)
*
* @return 	DRV_SUCCESS: if successful
*
* @note	None
**********************************************************************/
DRV_Status_t DRV_IOPIrSetRateDefault(void);


/******************************************************************************/
/**
* @fn DRV_Status_t DRV_IRInit(UINT32 dCustomerID)
*
* @brief	Initialize the IR module, This allocates any required resources,
*		The IR input is disabled by defaulr upon initialization
*
* @param    dCustomerID: Set 0xFFFFFFFF to ignore customer code check
*
* @return    GL_Status_t    GL_SUCCESS
*
* @note	None
*******************************************************************************/
DRV_Status_t DRV_IRInit(UINT32 dCustomerID);

/******************************************************************************/
/**
* @fn DRV_Status_t DRV_IRUnInit(void)
*
* @brief    Un-initialize the IR module. This frees currently used resources
*
* @return    GL_Status_t    GL_SUCCESS
*
* @note	None
*******************************************************************************/
DRV_Status_t DRV_IRUnInit(void);

/******************************************************************************/
/**
* @fn DRV_Status_t DRV_IREnable (void)
*
* @brief 	Enable the IR device.  While the IR device is enabled,
* 		IR remote keypresses are received and queued up.
*
* @return    GL_SUCCESS if successful
*
* @note	None
*******************************************************************************/
DRV_Status_t DRV_IREnable (void);

/******************************************************************************/
/**
* @fn DRV_Status_t DRV_IRDisable (void)
*
* @brief	Disable the IR device.  While the IR device is disabled,
*		all IR remote keypresses are discarded.
*
* @return    GL_SUCCESS if successful
*
* @note	None
*******************************************************************************/
DRV_Status_t DRV_IRDisable (void);

/******************************************************************************/
/**
* @fn DRV_Status_t DRV_IRGetValue(UINT32* pdRetIrCode,	UINT32 dTimeout)
*
* @brief	Get the IR code.  This function blocks until an IR code is received
*		or until dTimeout milliseconds have passed, whichever comes first.
* 		If dTimeout is set to GL_INFINITE_WAIT (-1), it will wait indefinitely.
*
* @param	pdRetIrCode : received IR code (return value)
* @param	dTimeout : timeout (in milliseconds) to wait for an IR code to be received
*
* @return    GL_SUCCESS if successful
*           GL_ERR_INVALID_PARAM if return pointer is DTV_NULL
*           GL_ERR_TIMEOUT if IR code was not received within specified timeout
*
* @note	None
*******************************************************************************/
DRV_Status_t DRV_IRGetValue(UINT32* pdRetIrCode, UINT32 *pdRetIrMode,	UINT32 dTimeout);

/******************************************************************************/
/**
* @fn DRV_Status_t DRV_IRStandby_SetConfig(DRV_IRStandbyCfg_t stConfig)
*
* @brief    Setup the IR command serviced by standby mode.
*
* @param    stConfig : Serviced command.
*
* @return    GL_Status_t    GL_SUCCESS
*
* @note	None
*******************************************************************************/
DRV_Status_t DRV_IRStandby_SetConfig(DRV_IRStandbyCfg_t stConfig);

/******************************************************************************/
/**
* @fn void DRV_IRStandby_SetupIop(void)
*
* @brief    System will call this funciton between "going into standby" and "IOP Go".
*
* @return   void
*
* @note	None
*******************************************************************************/
void DRV_IRStandby_SetupIop(void);

/******************************************************************************/
/**
* @fn	DRV_Status_t DRV_IOPIrGetValue_debug(UINT32 * pdRetIrCode, UINT32 dTimeout,UINT16 *pCid)
*
* @brief    Get the IR code. This function blocks until an IR code is received or
*until dTimeout milliseconds have passed, whichever comes first.
*If dTimeout is set to DRV_TIMEOUT_FOREVER (0), it will wait indefinitely.
*
* @param	pdRetIrCode : received IR code (return value)
* @param	pCid : received IR HEADER (return value)
* @param	dTimeout : timeout (in milliseconds) to wait for an IR code to be received
*
* @return	DRV_SUCCESS: if successful
* @return	DRV_ERR_INVALID_PARAM: if return pointer is NULL
* @return	DRV_ERR_TIMEOUT: if IR code was not received within specified timeout
* @return	DRV_ERR_FAILURE: if IR was disabled
*
*@note
******************************************************************************/
DRV_Status_t DRV_IOPIrGetValue_debug(UINT32 * pdRetIrCode, UINT32 dTimeout,UINT16 *pCid); //cuiyonghua 2009-8-11 17:14 .

//2010/3/31 lizhan mantis 0082122 fix the eup green power ui spec.START {{{
#ifdef SUPPORT_EUP_AUTO_STANDBY
/******************************************************************************/
/**
* @fn void DRV_IOPIr_ResetPDTime_callback(vfunc pfResetPDTimeCallbk)
*
* @brief    Set Function Point
*
* @param    pfResetPDTimeCallbk : Function Point
*
* @return   void
*
* @note	None
*******************************************************************************/
void DRV_IOPIr_ResetPDTime_callback(vfunc pfResetPDTimeCallbk);
#endif
//2010/3/31 lizhan mantis 0082122 fix the eup green power ui spec. END }}}

int ir_Standby_Initialize(UINT8* pIrStandbyTable, UINT8 bCmdCount, UINT32 dCustomCode); //Kaney add
vfunc ir_Standby_Get_SetupIopFunct(void); //Kaney add

#endif /* IOP_H */


