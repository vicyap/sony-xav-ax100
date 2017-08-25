/** @file
*    File    Name  :  drv_i2c.h
*    Descriptions  :  header for SPV9000 I2c driver
*        
*    Copyright (c) 2007 SunMedia technology (Chengdu) co. Ltd.
*    All right reserved
*   
* @ Date          :  2007-07-23
* @author        :  ds.tian    
*/

/* static const char *  _VERSION_DRV_I2C_H = "$Id: drv_i2c.h,v 1.4 2008/11/08 04:52:32 anlzhao Exp $"; */

#ifndef __DRV_I2C_H__
#define __DRV_I2C_H__

//#include "types.h" //Kaney mark
#include <mach/common.h> //Kaney add
#include "drv_common.h"
/****************************************************************************
*                         Public Constant Definition                        *
****************************************************************************/

/*! 
\def SUPPORT_RISC_I2C
* to choose  use RISC mode, if defined, I2c work at Risc mode, 
    or work at IOP mode
*/
//#define SUPPORT_RISC_I2C

#define DRV_I2C_RETRYTIME			(10)

//---------------------------------------------------------------------------
/*! 
\def DRV_I2CBUS_0
* to choose use no begin address mode to send or receive data
*/
#define DRV_I2CBUS_0				(0x00)

/*! 
\def DRV_I2CBUS_1
* to choose use begin address mode to send ro receive data
*/
#define DRV_I2CBUS_1				(0x01)

/*! 
\def DRV_I2CSUBADDR_8BIT
* len of I2C subaddr
*/
#define DRV_I2CSUBADDR_8BIT                 (0)

/*! 
\def DRV_I2CSUBADDR_16BIT
* len of I2C subaddr
*/
#define DRV_I2CSUBADDR_16BIT               (1)

/*! 
\def DRV_I2CSUBADDR_24BIT
* len of I2C subaddr
*/
#define DRV_I2CSUBADDR_24BIT               (2)  //2010/7/23 lizhan modify bug id 0096889 

/*! 
\def DRV_NOSTOP
* No Stop
*/
#define DRV_NOSTOP                            (0x01)

/*! 
\def DRV_I2C_LOCKBUS
* to choose Lock I2C BUS
*/
#define DRV_I2C_LOCKBUS                   (0x02)

/*! 
\def DRV_I2C_UNLOCKBUS
* to choose unLock I2C BUS
*/
#define DRV_I2C_UNLOCKBUS                   (0x04)

/*! 
\def DRV_I2C_NO_SUBADDRESS
* to choose no subaddress
*/
#define	DRV_I2C_NO_SUBADDRESS	(0x08)

//---------------------------------------------------------------------------
/*! 
\def PAGE_SIZE
* to at write page mode the max page size
*/
#define PAGE_SIZE                   (255)//(0x10)//2010/4/19 lizhan  for fix mantis bug 0084438 some tuner one times send 60 byte.

/****************************************************************************
*                          Public Type Declaration                          *
****************************************************************************/
//---------------------------------------------------------------------------
/*! 
* define I2c work frequence
*/
//2010/11/26 wenjing.yang modified for mantis 0112959 	START {{{
#ifndef RISC_SET_IOP_I2C_GPIO
typedef enum
{
    I2CCLK_FREQ_50KHZ = 1,
    I2CCLK_FREQ_100KHZ = 2,
    I2CCLK_FREQ_200KHZ = 3,
    I2CCLK_FREQ_300KHZ = 4,
    I2CCLK_FREQ_400KHZ = 5
}I2CCLK_FREQ_t;
#else
typedef enum
{
	#if 0
	IOP_I2CCLK_FREQ_DEFAULT	= 30,
	IOP_I2CCLK_FREQ_20KHZ		= 200,
	IOP_I2CCLK_FREQ_40KHZ		= 100,
	IOP_I2CCLK_FREQ_90KHZ		= 40,
	IOP_I2CCLK_FREQ_125KHZ	= 30,
	IOP_I2CCLK_FREQ_280KHZ	= 10,
	IOP_I2CCLK_FREQ_500KHZ	= 5,
	IOP_I2CCLK_FREQ_800KHZ	= 4,
	#endif
	
	IOP_I2CCLK_FREQ_DEFAULT	= 15,//111k		
	IOP_I2CCLK_FREQ_400KHZ	= 1,//397
	IOP_I2CCLK_FREQ_333KHZ	= 2,
	IOP_I2CCLK_FREQ_278KHZ	= 3,
	IOP_I2CCLK_FREQ_250KHZ	= 4,
	IOP_I2CCLK_FREQ_227KHZ	= 5,
	IOP_I2CCLK_FREQ_170KHZ	= 8,
	IOP_I2CCLK_FREQ_147KHZ	= 10,
	IOP_I2CCLK_FREQ_111KHZ	= 15,
	IOP_I2CCLK_FREQ_56KHZ		= 30,
	IOP_I2CCLK_FREQ_39KHZ		= 50,
	IOP_I2CCLK_FREQ_19_84KHZ	= 100,
	IOP_I2CCLK_FREQ_10_10KHZ	= 200,
	IOP_I2CCLK_FREQ_8_19KHZ	= 250,
}I2CCLK_FREQ_t;

typedef struct
{
	UINT32	dSda;
	UINT32	dScl;
	UINT32	dFreq;
}DRV_I2C_PARAM_t;
#endif
//2010/11/26 wenjing.yang modified for mantis 0112959 	END }}}
/*!
 * @brief   select I2C device
 */
typedef enum {
	DRV_I2C_EEPROM = 1,
	DRV_I2C_VGA,
	DRV_I2C_HDMI,
	DRV_I2C_SNIM1,
	DRV_I2C_SNIM2,
	DRV_I2C_TNIM1,
	DRV_I2C_TNIM2,
	DRV_I2C_CI,
	DRV_I2C_IPOD,
	// 20100416 zhangjie add for dvb_c start{{{
	DRV_I2C_CNIM1,
	DRV_I2C_CNIM2,
	// 20100416 zhangjie add for dvb_c end{{{
	//haleliu 20100505 add for sbtvd +
	DRV_I2C_INIM1,
	DRV_I2C_INIM2,
	//haleliu 20100505 add for sbtvd -
	// 20101229 lijin add for DVBST NIM start{{{
	DRV_I2C_STNIM1,
	DRV_I2C_STNIM2,
	// 20101229 lijin add for DVBST NIM end}}}	

	DRV_I2C_LNB1,
	DRV_I2C_LNB2,
} DRV_I2CDEV_t;

#if 1 //Kaney add
typedef struct I2C_CMD_PARAM_S
{
    unsigned char bus;
	DRV_I2CDEV_t  eDevSel; 
	unsigned char slaveAddr;
	unsigned int  DevSetting;
	unsigned char subAddrLength;	
	unsigned char *subAddr;
	unsigned short dataCnt;
	unsigned char *data;
	unsigned short Timeout;
	unsigned short flag;
} I2C_CMD_PARAM_T;
#endif

/*! 
* DRV_Status_t DRV_I2cWrite_UsePresetFreq(UINT16 wFreq,UINT16 u16Timeout);
*  SPV9000 I2c Speed
* @param wFreq :UsePresetFreq
* @param u16Timeout :delay ms after write stop
*@return
* DRV_SUCCESS on success
*/
void DRV_I2cWrite_UsePresetFreq(UINT16 wFreq,UINT16 u16Timeout);

/*! 
*Prototype:
* DRV_Status_t DRV_I2c_Init(void);
* Initial SPV9000 I2c Driver
* @param none
*@return
* DRV_SUCCESS on success
*/
DRV_Status_t DRV_I2c_Init(void);

/*!
* DRV_Status_t DRV_I2c_Wintool_Init(void);
* Initial SPV9000 I2c Driver
* @param none
*@return
* DRV_SUCCESS on success
*/
DRV_Status_t DRV_I2c_Wintool_Init(void);

/*! 
* DRV_Status_t DRV_I2c_Uninit(void)
* Un-Initial SPV9000 I2c Driver.
* @param none
*@return
* DRV_SUCCESS on success;
* DRV_ERR_INVALID_PARAM, if any of the Params is invalid;
* DRV_ERR_INVALID_OP, in case of error or NACK;
* DRV_ERR_TIMEOUT, if timeout was reached.
*/
DRV_Status_t DRV_I2c_Uninit(void);

/**
* DRV_Status_t DRV_I2cWrite(UINT8 u8Bus,UINT16 u16DeviceAdd,
        UINT32 u32DevSetting, UINT16 u32Star_Add, UINT32 u32Len, 
        UINT8 * p_u8Data, INT32 n32Timeout,UINT16 u16Flags);
* Send the specific number of bytes to the specific device 
* @param u8Bus [IN] I2C Bus number
* @param eDevSel [IN] Select I2C access device,DRV_I2C_EEPROM ,DRV_I2C_VGA,DRV_I2C_HDMI,DRV_I2C_SNIM1,DRV_I2C_SNIM2,	DRV_I2C_TNIM1,
	DRV_I2C_TNIM2,	DRV_I2C_CI,	DRV_I2C_IPOD,DRV_I2C_CNIM1,DRV_I2C_CNIM2,	DRV_I2C_INIM1,DRV_I2C_INIM2,	DRV_I2C_STNIM1,
	DRV_I2C_STNIM2,
* @param u8DevAddr [IN] I2C device address
* @param u32DevSetting [IN] I2C device setting,u32DevSetting is defined as (SUBADDR_TYPE<<12) bit13:1 subaddr type:subaddr len 2 bytes bit13:0 subaddr type:subaddr len 1 bytes 
* @param u8SubAddrLen [IN]Length of  Start sub-address 
* @param p_u8SubAddr [IN] Start sub-address from where reawrite should start
* @param u16Count [IN] Length of  data to write
* @param p_u8Data [IN] Pointer of data to be transmitted
* @param u16Timeout [IN] Timeout in ms to wait for IOP or Bus Lock
* @param u16Flags [IN]Flags, 0 means normal transcation
*@return
* DRV_SUCCESS on success;
* DRV_ERR_INVALID_PARAM, if any of the Params is invalid;
* DRV_ERR_INVALID_OP, in case of error or NACK;
* DRV_ERR_TIMEOUT, if timeout was reached.
*/
DRV_Status_t DRV_I2cWrite(UINT8 u8Bus, DRV_I2CDEV_t eDevSel, UINT8 u8DevAddr,
							UINT32 u32DevSetting, UINT8 u8SubAddrLen, UINT8* p_u8SubAddr, 
							UINT16 u16Count, UINT8* p_u8Data, UINT16 u16Timeout, UINT16 u16Flags);


/**
*Prototype:
* DRV_Status_t DRV_I2cRead (UINT8 u8Bus, UINT16 u16DeviceAdd, 
        UINT32 u32DevSetting, UINT32 u32Star_Add, UINT32 u32Len, 
        UINT8 * p_u8Data, INT32 n32Timeout,UINT16 u16Flags);
* Read the specific number of bytes to the specific device 
* @param u8Bus [IN] I2C Bus number
* @param eDevSel [IN] Select I2C access device,DRV_I2C_EEPROM ,DRV_I2C_VGA,DRV_I2C_HDMI,DRV_I2C_SNIM1,DRV_I2C_SNIM2,	DRV_I2C_TNIM1,
	DRV_I2C_TNIM2,	DRV_I2C_CI,	DRV_I2C_IPOD,DRV_I2C_CNIM1,DRV_I2C_CNIM2,	DRV_I2C_INIM1,DRV_I2C_INIM2,	DRV_I2C_STNIM1,
	DRV_I2C_STNIM2,
* @param u8DeviceAddr [IN] I2C device address
* @param u32DevSetting [IN] I2C device setting,u32DevSetting is defined as (SUBADDR_TYPE<<12) bit13:1 subaddr type:subaddr len 2 bytes bit13:0 subaddr type:subaddr len 1 bytes 
* @param u8SubAddrLen [IN]Length of  Start sub-address 
* @param p_u8SubAddr [IN] Start sub-address from where reawrite should start
* @param u16Count [IN] Length of  data to read
* @param p_u8Data [IN]  Data read will be returned in this buffer
* @param n32Timeout [IN] Timeout in ms to wait for IOP or Bus Lock
* @param u16Flags [IN]Flags, 0 means normal transcation
*@return
* DRV_SUCCESS on success;
* DRV_ERR_INVALID_PARAM, if any of the Params is invalid;
* DRV_ERR_INVALID_OP, in case of error or NACK;
* DRV_ERR_TIMEOUT, if timeout was reached.
*/
DRV_Status_t DRV_I2cRead (UINT8 u8Bus, DRV_I2CDEV_t eDevSel, UINT8 u8DeviceAddr, 
							UINT32 u32DevSetting, UINT8 u8SubAddrLen, UINT8* p_u8SubAddr,
							UINT16 u16Count, UINT8* p_u8Data, INT32 n32Timeout, UINT16 u16Flags);
                            
DRV_Status_t DRV_I2cWrite_DisInc(UINT8 u8Bus, DRV_I2CDEV_t eDevSel, UINT8 u8DevAddr,
							UINT32 u32DevSetting, UINT8 u8SubAddrLen, UINT8* p_u8SubAddr, 
							UINT16 u16Count, UINT8* p_u8Data, UINT16 u16Timeout, UINT16 u16Flags);
                            
DRV_Status_t DRV_I2cRead_DisInc(UINT8 u8Bus, DRV_I2CDEV_t eDevSel, UINT8 u8DeviceAddr, 
							UINT32 u32DevSetting, UINT8 u8SubAddrLen, UINT8* p_u8SubAddr,
							UINT16 u16Count, UINT8* p_u8Data, INT32 n32Timeout, UINT16 u16Flags);
/*! 
* DRV_Status_t DRV_I2cScan(UINT8 u8Bus);
* Debug for I2c Driver
* @param u8Bus [IN] I2C Bus number
*@return
* DRV_SUCCESS on success;
*/
DRV_Status_t DRV_I2cScan(UINT8 u8Bus);



/**
*Prototype:
* DRV_Status_t DRV_I2cReadBytes (
                        DRV_I2CDEV_t eDevSel,
						UINT8	bDevAddr,
						UINT8	bSubAddrLen,
						UINT8*	pSubAddr,
						UINT16	wCount,
						UINT8*	pBuf
						);
* Read the specific number of bytes to the specific device 
* @param eDevSel [IN] Select I2C access device,DRV_I2C_EEPROM ,DRV_I2C_VGA,DRV_I2C_HDMI,DRV_I2C_SNIM1,DRV_I2C_SNIM2,	DRV_I2C_TNIM1,
	DRV_I2C_TNIM2,	DRV_I2C_CI,	DRV_I2C_IPOD,DRV_I2C_CNIM1,DRV_I2C_CNIM2,	DRV_I2C_INIM1,DRV_I2C_INIM2,	DRV_I2C_STNIM1,
	DRV_I2C_STNIM2
* @param bDevAddr [IN] I2C device address
* @param bSubAddrLen [IN]Length of  Start sub-address 
* @param pSubAddr [IN] Start sub-address from where reawrite should start
* @param wCount [IN] Length of  data to read
* @param pBuf [IN]  Data read will be returned in this buffer
*@return
*	DRV_SUCCESS on success
*	DRV_ERR_INVALID_PARAM if any of the Params is invalid
*	DRV_ERR_INVALID_OP, in case of error or NACK
*Description:
*	Read the specified number of bytes from the specified device starting
*	at offset pSubAddr.
*/
DRV_Status_t DRV_I2cReadBytes(
                        DRV_I2CDEV_t eDevSel,
						UINT8	bDevAddr,
						UINT8	bSubAddrLen,
						UINT8*	pSubAddr,
						UINT16	wCount,
						UINT8*	pBuf,
						UINT8	u8Flag
						);
/**
*Prototype:
*    DRV_Status_t DRV_I2cWriteBytes(
*                       DRV_I2CDEV_t eDevSel,
*						UINT8	bDevAddr,
*						UINT8	bSubAddrLen,
*						UINT8*	pSubAddr,
*						UINT16	bCount,
*						UINT8*	pBuf,
*						UINT16	wTimecycle
*						)

* @param eDevSel :Select I2C access device
* @param bDevAddr :I2c device slave address
* @param bSubAddrLen :Length of  Start sub-address 
* @param pSubAddr :Start sub-address from where reawrite should start
* @param bCount :Byte count
* @param pBuf :write data into this buffer
* @param wTimecycle :Page write cycle time
*@return
*	DRV_SUCCESS on success
*	DRV_ERR_INVALID_PARAM if any of the Params is invalid
*	DRV_ERR_INVALID_OP, in case of error or NACK
*Description:
*	Write the specified number of bytes to the specified device starting
*	from the pSubAddr.
*/
DRV_Status_t DRV_I2cWriteBytes(
                        DRV_I2CDEV_t eDevSel,
						UINT8	bDevAddr,
						UINT8	bSubAddrLen,
						UINT8*	pSubAddr,
						UINT16	bCount,
						UINT8*	pBuf,
						UINT16	wTimecycle
						);

/**
*Prototype:
*	DRV_Status_t DRV_MasterI2cRead(
*                        DRV_I2CDEV_t eDevSel,
*						UINT8 bDevAddr,
*						UINT16 wCount,
*						UINT8 *pBuf
*						)

*@param eDevSel :Select I2C device
*@param bDevAddr :I2c device slave address
*@param wCount :Byte count
*@param pBuf :Dsata read will be returned in this buffer
*@return
*	DRV_SUCCESS on success
*	DRV_ERR_INVALID_PARAM if any of the Params is invalid
*	DRV_ERR_INVALID_OP, in case of error or NACK
*Description:
*	Read the specified number of bytes from the specified device.
*/
DRV_Status_t DRV_MasterI2cRead(
                        DRV_I2CDEV_t eDevSel,
						UINT8 bDevAddr,
						UINT16 wCount,
						UINT8 *pBuf
						);

/**
*Prototype:
*	DRV_Status_t DRV_MasterI2cWrite(
                        DRV_I2CDEV_t eDevSel,
						UINT8 bDevAddr,
						UINT16 wCount,
						UINT8 *pBuf
						)
*@param eDevSel :Select I2C device
*@param bDevAddr :I2c device slave address
*@param wCount :Byte count
*@param pBuf :Data write will be  in this buffer
@return
	DRV_SUCCESS on success
	DRV_ERR_INVALID_PARAM if any of the Params is invalid
	DRV_ERR_INVALID_OP, in case of error or NACK
Description:
	Read the specified number of bytes from the specified device.
*/
DRV_Status_t DRV_MasterI2cWrite(
                        DRV_I2CDEV_t eDevSel,
						UINT8 bDevAddr,
						UINT16 wCount,
						UINT8 *pBuf
						);

#ifdef SUPPORT_RISC_I2C_SPEED_FINTUNE
void RISC_SetI2cSpeed(INT16 wI2cFreq);
#endif

#endif
