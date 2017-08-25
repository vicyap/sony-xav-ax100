#ifndef	__IOP_H
#define __IOP_H
#include <linux/semaphore.h>
//#include "types.h" //Kaney mark
#include <mach/common.h> //Kaney add
#include "platform_define.h" //Kaney add
#include "customize.h" //Kaney add

typedef void (*handle_func)(void);
struct iop_handle_node{
	struct list_head list;
	handle_func handle;
};

void  iop_register_handle(struct iop_handle_node *pnode);
void iop_unregister_handle(struct iop_handle_node *pnode);
void iop_get_buffer_addr(unsigned int *buf_va, unsigned int *buf_pa);

#if 1 //Kaney add
typedef struct wakeup_timer_s {
	UINT8	bCurSec;
	UINT8	bCurMin;
	UINT8   bCurHour;
	UINT8	bCurDay;
	UINT8	bCurMon;
	UINT16  bCurYear;
	UINT8	bWakeupMin;
	UINT8   bWakeupHour;
	UINT8	bWakeupDay;
	UINT8	bWakeupMon;
	UINT16  bWakeupYear;
} wakeup_timer_t;

typedef struct vfd_current_time_s {
	UINT8   u8Hour;
	UINT8	u8Min;
	UINT8	u8Sec;
} vfd_current_time_t;

// IOP IOCTL define:
#define IOP_CMD_TYPE		'I'

#define IOP_CMD_STANDBY_WAKEUPTIMER     		_IOWR(IOP_CMD_TYPE, 0x01, wakeup_timer_t)
#define IOP_CMD_VFD_EVENT_COLON_ON				_IOWR(IOP_CMD_TYPE, 0x02, unsigned long)
#define IOP_CMD_VFD_EVENT_COLON_OFF				_IOWR(IOP_CMD_TYPE, 0x03, unsigned long)
#define IOP_CMD_VFD_EVENT_CUR_TIME				_IOWR(IOP_CMD_TYPE, 0x04, vfd_current_time_t)
#define IOP_CMD_VFD_EVENT_CURPROG_NO			_IOWR(IOP_CMD_TYPE, 0x05, unsigned long)
#define IOP_CMD_VFD_EVENT_CURMULTIMEDIA_NO		_IOWR(IOP_CMD_TYPE, 0x06, unsigned long)
#define IOP_CMD_VFD_EVENT_SINGAL_LOCK			_IOWR(IOP_CMD_TYPE, 0x07, unsigned long)
#define IOP_CMD_VFD_EVENT_SINGAL_UNLOCK			_IOWR(IOP_CMD_TYPE, 0x08, unsigned long)
#define I2C_CMD_WRITE							_IOWR(IOP_CMD_TYPE, 0x09, unsigned int)
#define I2C_CMD_READ							_IOWR(IOP_CMD_TYPE, 0x10, unsigned int)
#define IOP_CMD_STANDBY                 		_IOWR(IOP_CMD_TYPE, 0x11, unsigned long)
#define IOP_CMD_IS_BACKUP_CAM                 	_IOWR(IOP_CMD_TYPE, 0x12, unsigned long)
#endif

//IOP configuration
#define IOP_DATA_REG_COUNT		12

//I2C device ID
#define I2C_ID_MEMORY			0xa0
#define I2C_ID_TV_ENCODE		0x40
#define I2C_ID_AUDIO_DAC		0x20

//IOP function call define
#define FUN_PORT				0
#define FUN_MASK				0xf000 
#define FUN_VFD_READ			(0x1<<12)
#define FUN_VFD_WRITE			(0x2<<12)
#define FUN_IR_CUSTID			(0x4<<12)
#define FUN_IR_GET_CUSTID		(0x5<<12)
#define FUN_UART_SEND			(0x6<<12)
#define FUN_I2C_WRITE			(0x7<<12)
#define FUN_I2C_READ			(0x8<<12)
#define FUN_GPAD_READ			(0x9<<12)
#ifdef IOP_GPIO_CONTROL_TEST
#define FUN_GPIO_CONTROL		(0xb<<12)
#endif
#ifdef RISC_CFG_IOP_USE_GPAD_GPIO
#define FUN_GPAD_CFG_USE_PIN	(0xc<<12)//2010/5/4 lizhan added for mantis bug 0085542
#endif
#ifdef SUPPORT_GPIO_MONITOR //2009/12/09,Haoye added for gpio monitor.
#define FUN_GPMON_WRITE			(0xe<<12)
#define FUN_GPMON_READ			(0xf<<12)
#endif

//2010/11/10 wenjing.yang added for mantis 0110106 	START {{{
#ifdef RISC_SET_IOP_I2C_GPIO
#define FUN_I2C_SET_GPIO		(0xa << 12)
#endif
//2010/11/10 wenjing.yang added for mantis 0110106 	END }}}

#ifdef IOP_GPIO_CONTROL_TEST
#define GPIO_TEST_CMD_OUTPUT				(0x04<<8)
#define GPIO_TEST_CMD_INPUT					(0x08<<8)

#define GPIO_TEST_GPIO_LENEL				(0x01<<8)
#define GPIO_TEST_GPIO_INPUT_READY			(0x02<<8)
#endif
//IOP IR control interface
//register mapping of RISC register
#define IR_PORT					5
//control flag
#define IR_RDY					(1<<15)
#define IR_REPEAT				(1<<14)
#define IR_REPLACE				(1<<13)
#ifdef SUPPORT_IR_SPEEDUP
#define IR_REPEAT_STOP			(1<<11)
#endif

#define IrRepeatRate(x)			(x<<8)

#ifdef SUPPORT_GPIO_MONITOR //2009/12/09,haoye added for gpio monitor.
#define IOP_CMD					1  
#define IOP_DAT0				2  
#define IOP_DAT1				3  
#define IOP_DAT2				4
#define IOP_DATLEN_MASK		(~0x0007)
#define IOP_INTR_MSK			(0x1f00)
#define IOP_INTR_IR				(0x1<<8)
#define IOP_INTR_GPMON			(0x1<<13)
#endif

//IOP vfd control interface

//register mapping of RISC register
#define VFD_CMD					1  
#define VFD_DAT0				2  
#define VFD_DAT1				3  
#define VFD_DAT2				4
//control flag  
#define VFD_DATLEN_MASK			(~0x0007)
#define VFD_DAT_LEN0			0
#define VFD_DAT_LEN1			1  
#define VFD_DAT_LEN2			2 
#define VFD_DAT_LEN3			3
#define VFD_DAT_LEN4			4  
#define VFD_DAT_LEN5			5 
#define VFD_DAT_LEN6			6

//IOP UART control interface
#define UART_DAT				1
 
//I2C control interface
#define I2C_CHIP_ADDR			8
#define I2C_DATA				8
#define I2C_SUB_ADDR			9
#define I2C_SUB_ADDR_EXT		2  //2010/7/23 lizhan modify bug id 0096889 
#define I2C_STATUS				10

#define I2C_READ_ONE			(0<<13)
#define I2C_READ_CONT			(1<<13)

#define I2C_WRITE_BYTE			(0<<12)
#define I2C_WRITE_PAGE			(1<<12)

#define I2C_RW_ERR				(1<<8)
#define I2C_DATA_RDY			(1<<9)

#define I2C_SUB_ADDR_EXIST     	(1<<0)
#define I2C_SUB_ADDR_16BIT     	(1<<1)
#define I2C_STOP_EXIST     		(1<<2)
#define I2C_SUB_ADDR_24BIT		(1<<3)//2010/7/23 lizhan modify bug id 0096889 

#define I2C_SEQUENTIAL_ORER     (1<<8)//xyy 2003-11-30 17:12

#define	IOP_RDY_WAIT			4000000

//IOP DATA Mapping Table - Standby(STBY) to On.
#define IOP_STBY_ON_PORT 			0
#define IOP_STBY_ON_CMD_PWR_BIT		(1<<2)
#define IOP_STBY_ON_CMD_EJECT_BIT	(1<<3)
#define IOP_STBY_ON_CMD_PLAY_BIT	(1<<4)
//
#define IOP_STBY_ON_DATA_MAX		512
#define IOP_STBY_ON_FUNCs_MAX		4

#define IOP_FUNC_ID_0		((1<<3)|(1<<2)|(1<<1)|(1<<0))
#define IOP_FUNC_ID_1		((1<<3)|(1<<2)|(1<<1))
#define IOP_FUNC_ID_2		((1<<3)|(1<<2))
#define IOP_FUNC_ID_3		((1<<3))

#define MAGIC_WORD_LEN		8	//2011/5/17 wenjing.yang modified for mantis 0130994 	


// for IOP frequency
#define IOP_13_5MHz				1
#define IOP_27MHz				2
#define IOP_40_5MHz				3
#define IOP_54MHz				4
#define IOP_67_5MHz				5
#define IOP_81MHz				6
#define IOP_94_5MHz				7
#define IOP_108MHz				8
#define IOP_121_5MHz			9
#define IOP_135MHz				10

#define IOP_10MHz				20

#if 0
void iop_init(void);
void iop_reset(void);
int	IsIopFunRdy(void);
int	WaitIopFunRdy(int);
void SetIopFreq(unsigned char);
int	WriteToI2cImmediate(UINT16 wDeviceAddr, UINT8 *bData, int len);
#endif
//extern GL_Mutex_t gIOPMutexID; //Kaney mark
extern struct semaphore semIOP; //Kaney add

#ifdef SUPPORT_GPIO_MONITOR //2009/12/09,Haoye added for gpio monitor.
extern int (*WriteToGpMon)(UINT16 wCmd,UINT8 *pData, UINT8 bLen);
extern int (*ReadFromGpMon)(UINT16 wCmd,UINT8 *pData, UINT8 bLen);
#endif
#ifdef SUPPORT_IOP2_STANDBY_WAKEUPTIMER //added for wake up from IOP
#define IOP_MANUALWKTIME_COUNT 6
#define IOP_AUTOWKTIME_COUNT 4
#define STANDBY_TIMEINFO 6
INT32 DRV_IopStby_WkTimeUinit(void);
#endif
#ifdef IOP_GPIO_CONTROL_TEST
UINT8 DRV_IOP_TestGpio(UINT32 dGpioNum, UINT8 eGpioState);
#endif

extern UINT32 g_u32IopBufAddr;

#ifdef MODEL_BOARD_bd_8602evb //Titan added. iop buffer address changed at this customer type (8602).
  #define BASE_IOP_BUF_ADDR							((UINT8 *)(g_u32IopBufAddr + 0x0b00))
#else
  #define BASE_IOP_BUF_ADDR							((UINT8 *)(g_u32IopBufAddr + 0x0f00))
#endif

//HDMI CEC
#define CEC_BUF_READ_ADDR        						(BASE_IOP_BUF_ADDR)	//0x0f00   or 0xb00 (at8602type)
#define CEC_BUF_WRITE_ADDR        					(CEC_BUF_READ_ADDR + 0x20)	//0x0f20
#define CEC_BUF_WRITE_LENGTH						(CEC_BUF_WRITE_ADDR + 0x20)	//0x0f40
#define CEC_BUF_READ_LENGTH						(CEC_BUF_WRITE_LENGTH + 1)	//0x0f41
#define CEC_BUF_PHYSICAL_ADDR_A					(CEC_BUF_READ_LENGTH + 1)	//0x0f42
#define CEC_BUF_PHYSICAL_ADDR_B					(CEC_BUF_PHYSICAL_ADDR_A + 1)	//0x0f43
#define CEC_BUF_LOGICAL_ADDR1						(CEC_BUF_PHYSICAL_ADDR_B + 1)//0x0f44	
#define CEC_BUF_LOGICAL_ADDR2						(CEC_BUF_LOGICAL_ADDR1 + 1)	//0x0f45
#define CEC_BUF_DEV_NO								(CEC_BUF_LOGICAL_ADDR2 + 1)	//0x0f46

//wakup Timer
#define CUR_TS_MIN_ADDR							(CEC_BUF_DEV_NO + 1)//0x0f47
#define CUR_TS_HOUR_ADDR							(CUR_TS_MIN_ADDR + 1)//0x0f48
#define CUR_TS_DAY_ADDR							(CUR_TS_HOUR_ADDR + 1)//0x0f49
#define CUR_TS_MON_ADDR							(CUR_TS_DAY_ADDR + 1)//0x0f4a
#define CUR_TS_YEAR_LOW_ADDR						(CUR_TS_MON_ADDR + 1)//0x0f4b
#define CUR_TS_YEAR_HIGH_ADDR						(CUR_TS_YEAR_LOW_ADDR + 1)//0x0f4c

#define WAKEUP_TIMER_MIN_ADDR					(CUR_TS_YEAR_HIGH_ADDR + 1)//0x0f4d
#define WAKEUP_TIMER_HOUR_ADDR					(WAKEUP_TIMER_MIN_ADDR + 1)//0x0f4e
#define WAKEUP_TIMER_DAY_ADDR					(WAKEUP_TIMER_HOUR_ADDR + 1)//0x0f4f
#define WAKEUP_TIMER_MON_ADDR					(WAKEUP_TIMER_DAY_ADDR + 1)//0x0f50
#define WAKEUP_TIMER_YEAR_LOW_ADDR				(WAKEUP_TIMER_MON_ADDR + 1)//0x0f51
#define WAKEUP_TIMER_YEAR_HIGH_ADDR				(WAKEUP_TIMER_YEAR_LOW_ADDR + 1)//0x0f52
#define CUR_NEXT_TS_YEAR_IS_LEAP_ADDR			(WAKEUP_TIMER_YEAR_HIGH_ADDR + 1)//0x0f53
#define CUR_TS_SEC_ADDR							(CUR_NEXT_TS_YEAR_IS_LEAP_ADDR + 1)//0x0f54
//------------------
#define IC_VERSION_ADDR									(CUR_TS_SEC_ADDR + 1)//0x0f55
#define HDMI_CEC_ONOFF_ADDR								(IC_VERSION_ADDR + 1)//0x0f56
#define IRC_POWER_ADDR_ADDR							(HDMI_CEC_ONOFF_ADDR +1)//0x0f57
#define IRC_POWER_DATA_ADDR							(IRC_POWER_ADDR_ADDR +1)//0x0f58
//----------------------

#ifdef SUPPORT_IOP2_STANDBY_WAKEUPTIMER //Kaney add
#define CUR_TS_MIN_REG								(*CUR_TS_MIN_ADDR)//IOP_DATA3_L
#define CUR_TS_HOUR_REG							(*CUR_TS_HOUR_ADDR)//IOP_DATA3_H
#define CUR_TS_DAY_REG								(*CUR_TS_DAY_ADDR)//IOP_DATA4_L
#define CUR_TS_MON_REG								(*CUR_TS_MON_ADDR)//IOP_DATA4_H
#define CUR_TS_YEAR_LOW_REG						(*CUR_TS_YEAR_LOW_ADDR)//IOP_DATA5_L
#define CUR_TS_YEAR_HIGH_REG						(*CUR_TS_YEAR_HIGH_ADDR)//IOP_DATA5_H

#define WAKEUP_TIMER_MIN_REG						(*WAKEUP_TIMER_MIN_ADDR)//IOP_DATA6_L
#define WAKEUP_TIMER_HOUR_REG					(*WAKEUP_TIMER_HOUR_ADDR)//IOP_DATA6_H
#define WAKEUP_TIMER_DAY_REG						(*WAKEUP_TIMER_DAY_ADDR)//IOP_DATA7_L
#define WAKEUP_TIMER_MON_REG						(*WAKEUP_TIMER_MON_ADDR)//IOP_DATA7_H
#define WAKEUP_TIMER_YEAR_LOW_REG				(*WAKEUP_TIMER_YEAR_LOW_ADDR)//IOP_DATA8_L
#define WAKEUP_TIMER_YEAR_HIGH_REG				(*WAKEUP_TIMER_YEAR_HIGH_ADDR)//IOP_DATA8_H
#define CUR_NEXT_TS_YEAR_IS_LEAP_REG				(*CUR_NEXT_TS_YEAR_IS_LEAP_ADDR)//IOP_DATA9_L
#define CUR_TS_SEC_REG								(*CUR_TS_SEC_ADDR)//IOP_DATA9_H;;;2010/5/21 lizhan add for fix mantis 0087785 use iop_data[9]_H save curent ts second time.
#endif


#endif/*__IOP_H*/
