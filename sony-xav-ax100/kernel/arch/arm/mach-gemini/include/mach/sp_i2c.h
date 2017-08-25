
#ifndef __MACH_letb123_I2C_H
#define __MACH_letb123_I2C_H

#include <mach/hal/hal_i2c.h>

// I2C IOCTL define:
#define I2C_CMD_TYPE		'I'

#define I2C_CMD_WRITE		_IOWR(I2C_CMD_TYPE, 0x01, unsigned int)
#define I2C_CMD_READ		_IOWR(I2C_CMD_TYPE, 0x02, unsigned int)

/*
#define I2C_CMD_CURRENT_READ					_IOWR(I2C_CMD_TYPE, 0x01, unsigned int)
#define I2C_CMD_CURRENT_WRITE					_IOWR(I2C_CMD_TYPE, 0x02, unsigned int)
#define I2C_CMD_RANDOM_READ					_IOWR(I2C_CMD_TYPE, 0x03, unsigned int)
#define I2C_CMD_RANDOM_WRITE					_IOWR(I2C_CMD_TYPE, 0x04, unsigned int)

#define I2C_CMD_CURREN_SEQUENTIAL_READ		_IOWR(I2C_CMD_TYPE, 0x05, unsigned int)
#define I2C_CMD_CURREN_SEQUENTIAL_WRITE		_IOWR(I2C_CMD_TYPE, 0x06, unsigned int)
#define I2C_CMD_RANDOM_SEQUENTIAL_READ		_IOWR(I2C_CMD_TYPE, 0x07, unsigned int)
#define I2C_CMD_RANDOM_SEQUENTIAL_WRITE		_IOWR(I2C_CMD_TYPE, 0x08, unsigned int)
*/


#define ERR(fmt, arg...)		printk(KERN_ERR "[%s:%d] Error! "fmt, __FUNCTION__, __LINE__, ##arg)
#define WARNING(fmt, arg...)	printk(KERN_WARNING "[%s:%d] Warning! "fmt, __FUNCTION__, __LINE__, ##arg)
#define NOTICE(fmt, arg...)	printk(KERN_NOTICE "[%s:%d] "fmt, __FUNCTION__, __LINE__, ##arg)
#define INFO(fmt, arg...)		printk(KERN_INFO "[%s:%d] "fmt, __FUNCTION__, __LINE__, ##arg)
#define DEBUG(fmt, arg...)		printk(KERN_DEBUG "[%s:%d] "fmt, __FUNCTION__, __LINE__, ##arg)


typedef struct
{
	UINT8 rw;//0-idle, 1-write, 2-read.
	UINT8 i;
	UINT8 k;
	UINT8 ret;
	UINT8 *data_buf;
	UINT32 data_cnt;
	UINT32 burst_cnt;
	UINT32 burst_r;
}I2C_COM_STATE;


INT32 sp_i2cm0_config(void);
void sp_i2cm0_suspend(void);
void sp_i2cm0_resume(void);
void sp_i2cm0_remove(void);
 int sp_i2cm0_write (I2C_CMD_INFO cmd_info);
 int sp_i2cm0_read (I2C_CMD_INFO cmd_info);

INT32 sp_i2cm1_config(void);
void sp_i2cm1_suspend(void);
void sp_i2cm1_resume(void);
void sp_i2cm1_remove(void);
 int sp_i2cm1_write (I2C_CMD_INFO cmd_info);
 int sp_i2cm1_read (I2C_CMD_INFO cmd_info);


#endif

