#ifndef __GPS_HAL_H__
#define __GPS_HAL_H__

#include <mach/hal/regmap/reg_gps.h>
#include <mach/hal/regmap/reg_system.h>

/* Use 'h' as magic number */
#define GPS_IOC_MAGIC  'k'
#define GPS_GET_CMD 	_IOR(GPS_IOC_MAGIC,  1, int)
#define GPS_SET_CMD 	_IOW(GPS_IOC_MAGIC,  2, int)
#define GPS_LOCK_ISR 	_IOW(GPS_IOC_MAGIC,  3, int)
#define GPS_GET_SHARE_MEM _IOW(GPS_IOC_MAGIC,  4, int)

// define GPS DEV INFO
#define GPS_DEVNAME 		"gps"
#define GPS_TIMER_NAME		"gps_timer"

#define GPS_TIMER_IRQ_NO 	75// 43+32


struct gps_cmd_t
{
	u32 addr;
	u32 value;
	u32 size;
	unsigned char buf[0];
};




static GPS_REG* gps_reg = ((GPS_REG*)(LOGI_ADDR_GPS_REG));

static moon1Reg_t* moon1_reg=((moon1Reg_t*)(LOGI_ADDR_MOON1_REG));

#ifdef GPS_VENDOR_REGISTER
#define VENDOR_GPS_ADDR  LOGI_ADDR_VENDER_GPS
#define VENDOR_RTC_ADDR  LOGI_ADDR_VENDOR_RTC
#else
#define VENDOR_GPS_ADDR  ioremap(SP_APB1_BASE, CFG_VENDOR_GPSBASE_SIZE)
#define VENDOR_RTC_ADDR  ioremap(SP_APB3_BASE, CFG_VENDOR_RTCBASE_SIZE)
#endif

#endif // __GPS_HAL_H__