
#ifndef __MACH_letb123_I2C_DDC_H
#define __MACH_letb123_I2C_DDC_H

#include <mach/hal/hal_i2c_ddc.h>

// I2C IOCTL define:
#define I2C_DDC_CMD_TYPE		'I'

#define I2C_DDC_SRAM_WRITE	_IOWR(I2C_DDC_CMD_TYPE, 0x01, unsigned int)
#define I2C_DDC_SRAM_READ		_IOWR(I2C_DDC_CMD_TYPE, 0x02, unsigned int)

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


typedef struct _I2C_DDC_SRAM_
{
	UINT8 addr;
	UINT8 *data;
	UINT8 reverse[2];
}I2C_DDC_SRAM;




#endif

