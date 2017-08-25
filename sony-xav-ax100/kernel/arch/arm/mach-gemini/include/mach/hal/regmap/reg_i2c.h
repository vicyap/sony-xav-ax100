/**
 * @file reg_i2c.h
 * @brief i2c master register definition
 * @author zh.l
 */
#ifndef __REG_I2C_H__
#define __REG_I2C_H__

#include <mach/hardware.h>

#define I2CM0_GRP				57
#define I2CM0_BASE				IO0_ADDRESS(I2CM0_GRP*32*4)
#define LOGI_ADDR_I2CM0_REG	(I2CM0_BASE)

#define I2CM1_GRP				58
#define I2CM1_BASE				IO0_ADDRESS(I2CM1_GRP*32*4)
#define LOGI_ADDR_I2CM1_REG	(I2CM1_BASE)

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

typedef	struct regs_i2cm0_s {
	volatile unsigned int control0;			// 00
	volatile unsigned int control1;			// 01
	volatile unsigned int control2;			// 02
	volatile unsigned int control3;			// 03
	volatile unsigned int control4;			// 04
	volatile unsigned int control5;			// 05
	volatile unsigned int i2cm_status0;		// 06
	volatile unsigned int int_status;		// 07
	volatile unsigned int int_en0;			// 08
	volatile unsigned int i2cm_mode;		// 09
	volatile unsigned int i2cm_status1;		// 10
	volatile unsigned int i2cm_status2;		// 11
	volatile unsigned int control6;			// 12
	volatile unsigned int int_en1;			// 13
	volatile unsigned int i2cm_status3;		// 14
	volatile unsigned int i2cm_status4;		// 15
	volatile unsigned int int_en2;			// 16
	volatile unsigned int control7;			// 17
	volatile unsigned int reserved[6];		// 18~23
	volatile unsigned int data00_03;		// 24
	volatile unsigned int data04_07;		// 25
	volatile unsigned int data08_11;		// 26
	volatile unsigned int data12_15;		// 27
	volatile unsigned int data16_19;		// 28
	volatile unsigned int data20_23;		// 29
	volatile unsigned int data24_27;		// 30
	volatile unsigned int data28_31;		// 31
} regs_i2cm0_t;

typedef	struct regs_i2cm1_s {
	volatile unsigned int control0;			// 00
	volatile unsigned int control1;			// 01
	volatile unsigned int control2;			// 02
	volatile unsigned int control3;			// 03
	volatile unsigned int control4;			// 04
	volatile unsigned int control5;			// 05
	volatile unsigned int i2cm_status0;		// 06
	volatile unsigned int int_status;		// 07
	volatile unsigned int int_en0;			// 08
	volatile unsigned int i2cm_mode;		// 09
	volatile unsigned int i2cm_status1;		// 10
	volatile unsigned int i2cm_status2;		// 11
	volatile unsigned int control6;			// 12
	volatile unsigned int int_en1;			// 13
	volatile unsigned int i2cm_status3;		// 14
	volatile unsigned int i2cm_status4;		// 15
	volatile unsigned int int_en2;			// 16
	volatile unsigned int control7;			// 17
	volatile unsigned int reserved[6];		// 18~23
	volatile unsigned int data00_03;		// 24
	volatile unsigned int data04_07;		// 25
	volatile unsigned int data08_11;		// 26
	volatile unsigned int data12_15;		// 27
	volatile unsigned int data16_19;		// 28
	volatile unsigned int data20_23;		// 29
	volatile unsigned int data24_27;		// 30
	volatile unsigned int data28_31;		// 31
} regs_i2cm1_t;

#endif

/* __REG_I2C_H__ */

