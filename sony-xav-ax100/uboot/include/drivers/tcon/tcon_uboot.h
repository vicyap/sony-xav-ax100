#ifndef __TCON_UBOOT_H__
#define __TCON_UBOOT_H__

#include <common.h>
#include <malloc.h>
#include <sp_bootinfo.h>
#include <linux/compat.h>
#include <sp_config.h>
#include <drivers/gpio/sp_gpio.h>

typedef unsigned char UINT8;
typedef char INT8;
typedef unsigned short UINT16;
typedef short INT16;
typedef int INT32;
typedef long long INT64;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;

#ifdef DEBUG
	#undef DEBUG
#endif

#define diag_printf		printf

#define delay_1us(x)	ndelay(x)
#define delay_1ms(x)	ndelay(x*1000)

#ifndef PACKED
	#define PACKED		__attribute__((packed))
#endif

#ifndef ENABLE
	#define ENABLE			1
#endif

#ifndef DISABLE
	#define DISABLE			0
#endif

#endif	//__TCON_UBOOT_H__

