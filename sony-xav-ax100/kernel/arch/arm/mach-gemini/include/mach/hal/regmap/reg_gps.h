#ifndef __REG_GPS_H__
#define __REG_GPS_H__

#include <mach/hardware.h>

// Hardware register memory address:
/* CPU base address */
#define CFG_CPUBASE		(0x9c000000)
#define CFG_CPUBASE_SIZE	(64 * 1024)
/* vendor gps base address */
#define CFG_VENDOR_GPSBASE	(0x9c060000)
#define CFG_VENDOR_GPSBASE_SIZE	(64 * 1024)
/* vendor rtc base address */
#define CFG_VENDOR_RTCBASE	(0x9c070000)
#define CFG_VENDOR_RTCBASE_SIZE	(64 * 1024)

// Hardware register memory address by hardware.h:
#define GPS_GRP			115
#define CFG_GPS_OFFSET		IO0_ADDRESS(GPS_GRP * 32 * 4)	//(CFG_CPUBASE + GPS_GRP * 32 * 4)
#define LOGI_ADDR_GPS_REG	CFG_GPS_OFFSET

#ifdef GPS_VENDOR_REGISTER
#define LOGI_ADDR_VENDER_GPS	IO1_ADDRESS(0)	//CFG_VENDOR_GPSBASE
#define LOGI_ADDR_VENDOR_RTC	IO3_ADDRESS(0)	//CFG_VENDOR_RTCBASE
#endif

typedef struct _GPS_REG_ {
	volatile unsigned int RF_RDATA;		// 00
	volatile unsigned int GPS_CTRL;		// 01
	volatile unsigned int GPS_DUMP_CTRL;	// 02
} GPS_REG;
#endif //__REG_GPS_H__