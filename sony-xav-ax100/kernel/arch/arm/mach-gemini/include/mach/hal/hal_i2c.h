#ifndef __I2C_HAL_H__
#define __I2C_HAL_H__

#include <linux/types.h>
#include <linux/module.h>
#include <mach/common.h>
#include <mach/hal/regmap/reg_i2c.h>

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
#define I2C_NACK_FLAG				1<<12
#define I2C_WFIFO_ENABLE_FLAG       1<<10
#define I2C_EMPTY_THRESHOLD_CTL 	4<<9
//#define I2C_EMPTY_THRESHOLD_CTL   1<<9
#define I2C_EMPTY_FLAG				1<<8
#define I2C_SCL_WAIT_FLAG			1<<7
#define I2C_EMPTY_THRESHOLD_FLAG	1<<6
#define I2C_DATA_NACK_FLAG		    1<<5
#define I2C_ADDRESS_NACK_FLAG		1<<4
#define I2C_BUSY_FLAG				1<<3
#define I2C_CLKERR_FLAG				1<<2
#define I2C_ACTIVE_DONE_FLAG		1<<1
#define I2C_SIFBUSY_FLAG			1
#define I2C_ALL_FLAG				0xff

//#define I2C_BURST_RDATA_FLAG		0x88888888
#define I2C_BURST_RDATA_FLAG        0x80008000 //must sync with GET_BYTES_EACHTIME
#define I2C_BURST_RDATA_ALL_FLAG	0xffffffff
#define GET_BYTES_EACHTIME   16 //if you change this, remember change other define at sp_i2cm0/m1

#define SP_OK			0
#define SP_FAIL			1
#define SP_SUCCESS		0
#define SP_TRUE			1
#define SP_FALSE		0
#define SP_IDLE_STATE    0
#define SP_WRITE_STATE   1
#define SP_READ_STATE    2

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

#if 0
typedef unsigned char			BYTE;
typedef unsigned short			WORD;
typedef unsigned long			DWORD;

typedef unsigned long long 		UINT64;
typedef unsigned int   			UINT32;
typedef unsigned short 		UINT16;
typedef unsigned char 			UINT8;

typedef long long 				INT64;
typedef int   					INT32;
typedef short 				INT16;
typedef signed char			INT8;
#endif

typedef enum _I2C_ACTIVE_MODE_
{
	I2C_TRIGGER = 0,
	I2C_AUTO = 1,
}I2C_ACTIVE_MODE;

typedef enum _I2C_READ_MODE_
{
	NORMAL_MODE = 0,
	RESTART_MODE = 1,
}I2C_READ_MODE;

typedef struct _I2C_DEV_INFO_
{
	UINT32 freq;					// Khz
	UINT8 slaveAddr;				// 7bit
	UINT8 subAddrLength;		// byte
	I2C_READ_MODE readMode;	// normal or restart
} I2C_DEV_INFO;

typedef struct _I2C_CMD_INFO_
{
	I2C_DEV_INFO i2cDev;
	UINT32 subAddr;
	UINT32 dataCnt;
	UINT8 *data;
} I2C_CMD_INFO;

typedef struct _I2C_IRQ_EVENT_
{
	UINT8 active_done;
	UINT8 nack;
	UINT8 fifo_empty;
	UINT8 roverflow;
	UINT8 rdata_event[32/GET_BYTES_EACHTIME];//8 or 2
	//UINT8 rdata_event[2];//because 16 each time
} I2C_IRQ_EVENT;

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

#if 0
INT32 hal_i2c_reg_mmap(void);
void hal_i2c_reg_unmmap(void);
#endif

void hal_i2cm0_enable(void);
void hal_i2cm0_disable(void);
void hal_i2cm0_setpinmux(INT32 cfg);
void hal_i2cm0_reg_set(UINT32 offset, UINT32 val);
void hal_i2cm0_reg_get(UINT32 offset, UINT32 *val);
void hal_i2cm0_reset(void);
void hal_i2cm0_data_set(UINT32 *wdata);
void hal_i2cm0_data_get(UINT32 *rdata);
void hal_i2cm0_data0_set(UINT32 *wdata);
void hal_i2cm0_data_single_get(UINT32 *rdata, UINT32 num, UINT8 bytes_eachtime);
void hal_i2cm0_trans_cnt_set(UINT8 write_cnt, UINT8 read_cnt);
void hal_i2cm0_rw_mode_set(UINT8 rw_mode, UINT8 sub_addr_en,I2C_READ_MODE read_mode);
void hal_i2cm0_active_mode_set(I2C_ACTIVE_MODE mode);
void hal_i2cm0_manual_trigger(void);
void hal_i2cm0_slave_add_set(UINT8 addr);
void hal_i2cm0_clock_freq_set(UINT32 freq);
void hal_i2cm0_status_get(UINT32 *flag);
void hal_i2cm0_status_clear(UINT32 flag);
void hal_i2cm0_rdata_flag_get(UINT32 *flag);
void hal_i2cm0_rdata_flag_clear(UINT32 flag);
void hal_i2cm0_roverflow_flag_get(UINT32 *flag);
void hal_i2cm0_fifo_cnt_get(UINT32 *cnt);
INT32 hal_i2cm0_address_ack_wait(void);
INT32 hal_i2cm0_busy_wait(void);
INT32 hal_i2cm0_complete_wait(void);
INT32 hal_i2cm0_ring_wait(void);
INT32 hal_i2cm0_rdara_ready_wait(UINT32 bit_num);
void hal_i2cm0_int_en(UINT32 int0, UINT32 int1, UINT32 int2, UINT32 en);

void hal_i2cm1_enable(void);
void hal_i2cm1_disable(void);
void hal_i2cm1_setpinmux(INT32 cfg);
void hal_i2cm1_reg_set(UINT32 offset, UINT32 val);
void hal_i2cm1_reg_get(UINT32 offset, UINT32 *val);
void hal_i2cm1_reset(void);
void hal_i2cm1_data_set(UINT32 *wdata);
void hal_i2cm1_data_get(UINT32 *rdata);
void hal_i2cm1_data0_set(UINT32 *wdata);
void hal_i2cm1_data_single_get(UINT32 *rdata, UINT32 num, UINT8 bytes_eachtime);
void hal_i2cm1_trans_cnt_set(UINT8 write_cnt, UINT8 read_cnt);
void hal_i2cm1_rw_mode_set(UINT8 rw_mode, UINT8 sub_addr_en,I2C_READ_MODE read_mode);
void hal_i2cm1_active_mode_set(I2C_ACTIVE_MODE mode);
void hal_i2cm1_manual_trigger(void);
void hal_i2cm1_slave_add_set(UINT8 addr);
void hal_i2cm1_clock_freq_set(UINT32 freq);
void hal_i2cm1_status_get(UINT32 *flag);
void hal_i2cm1_status_clear(UINT32 flag);
void hal_i2cm1_rdata_flag_get(UINT32 *flag);
void hal_i2cm1_rdata_flag_clear(UINT32 flag);
void hal_i2cm1_roverflow_flag_get(UINT32 *flag);
void hal_i2cm1_fifo_cnt_get(UINT32 *cnt);
INT32 hal_i2cm1_address_ack_wait(void);
INT32 hal_i2cm1_busy_wait(void);
INT32 hal_i2cm1_complete_wait(void);
INT32 hal_i2cm1_ring_wait(void);
INT32 hal_i2cm1_rdara_ready_wait(UINT32 bit_num);
void hal_i2cm1_int_en(UINT32 int0, UINT32 int1, UINT32 int2, UINT32 en);


#endif // __I2C_HAL_H__


