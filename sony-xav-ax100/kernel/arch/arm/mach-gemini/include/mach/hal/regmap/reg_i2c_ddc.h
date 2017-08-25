/**
 * @file reg_i2c.h
 * @brief i2c master register definition
 * @author zh.l
 */
#ifndef __REG_I2C_DDC_H__
#define __REG_I2C_DDC_H__

#include <mach/hardware.h>

#define I2CDDC0_GRP					55
#define I2CDDC0_BASE				IO0_ADDRESS(I2CDDC0_GRP*32*4)
#define LOGI_ADDR_I2CDDC0_REG		(I2CDDC0_BASE)


/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

typedef	struct regs_i2cddc0_s {
	volatile unsigned int reserved_0[5];			// 00 - 04
	volatile unsigned int i2c_sram_addr;			// 05
	volatile unsigned int reserved_1;			// 06
	volatile unsigned int ddc_finish_flag;			// 07
	volatile unsigned int ddc_ctrl1;				// 08
	volatile unsigned int ddc_slvaddr;			// 09
	volatile unsigned int ddc_ctrl2;				// 0a
	volatile unsigned int ddc_sram_data;		// 0b
	volatile unsigned int ddc_set_sram_addr;	// 0c
	volatile unsigned int ddc_rom_addr;			// 0d
	volatile unsigned int ddc_ddc_addr;			// 0e
	volatile unsigned int ddc_test_data;			// 0f
	volatile unsigned int ddc_ddc_i2cslavefilter;	// 10
	volatile unsigned int ddc_null_msg_source;	// 11
	volatile unsigned int ddc_ddcci_ctrl1;		// 12
	volatile unsigned int ddc_ddcci_ctrl2;		// 13
	volatile unsigned int ddc_ddcci_status;		// 14
	volatile unsigned int ddc_ddcci_ctrl3;		// 15
	volatile unsigned int ddc_ddcci_ctrl0;		// 16
	volatile unsigned int ddc_ddcci_ctrl4;		// 17
	volatile unsigned int ddc_ddcci_ctrl5;		// 18
	volatile unsigned int reserved_2[6];			// 19 - 1e
	volatile unsigned int ddc_sram_prob;		// 1f
  } regs_i2cddc0_t;


#endif

/* __REG_I2C_DDC_H__ */

