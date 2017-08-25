#ifndef __I2C_DDC_HAL_H__
#define __I2C_DDC_HAL_H__

#include <linux/types.h>
#include <linux/module.h>
#include <mach/common.h>
#include <mach/hal/regmap/reg_i2c_ddc.h>

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
#define SP_OK			0
#define SP_FAIL			1
#define SP_SUCCESS		0
#define SP_TRUE			1
#define SP_FALSE		0

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/


/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

void hal_i2cddc0_reg_set(UINT32 offset, UINT32 val);
void hal_i2cddc0_reg_get(UINT32 offset, UINT32 *val);
void hal_i2cddc0_sram_write(UINT8 addr, UINT8 value);
void hal_i2cddc0_sram_read(UINT8 addr, UINT8 *value);
void hal_i2cddc0_slave_add_config(UINT8 addr);
void hal_i2cddc0_init(void);
void hal_i2cddc0_enable(void);
void hal_i2cddc0_disable(void);


#endif // __I2C_DDC_HAL_H__


