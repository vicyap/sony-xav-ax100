
#include <linux/delay.h>
#include <mach/hal/regmap/reg_system.h>
#include <mach/hal/hal_i2c_ddc.h>
#include <mach/gpio_drv.h>

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/


/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
#define DDC_IMP_MASK		       	0x03<<2
#define DDC_RW_MASK		       	0x01

#define MOON0_I2CDDC0_MASK		1<<30


/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/
regs_i2cddc0_t *pI2cDdc0Reg = (regs_i2cddc0_t *)LOGI_ADDR_I2CDDC0_REG;


/**************************************************************************
 *             F U N C T I O N    I M P L E M E N T A T I O N S           *
 **************************************************************************/

void hal_i2cddc0_reg_set(UINT32 offset, UINT32 val)
{
	*((UINT32 *)pI2cDdc0Reg+offset) = val;
}
EXPORT_SYMBOL(hal_i2cddc0_reg_set);


void hal_i2cddc0_reg_get(UINT32 offset, UINT32 *val)
{
	 *val = *((UINT32 *)pI2cDdc0Reg+offset);
}
EXPORT_SYMBOL(hal_i2cddc0_reg_get);


void hal_i2cddc0_sram_write(UINT8 addr, UINT8 value)
{
	UINT32 tmp;

	tmp = pI2cDdc0Reg->ddc_slvaddr;
	pI2cDdc0Reg->ddc_slvaddr = ((tmp & (~DDC_IMP_MASK)) | 0x04);
	pI2cDdc0Reg->ddc_set_sram_addr = (UINT32)addr;
	pI2cDdc0Reg->ddc_sram_data = (UINT32)value;
	pI2cDdc0Reg->ddc_slvaddr = tmp;
}
EXPORT_SYMBOL(hal_i2cddc0_sram_write);


void hal_i2cddc0_sram_read(UINT8 addr, UINT8 *value)
{
	UINT32 tmp, backup;
	
	tmp = pI2cDdc0Reg->ddc_ddc_addr;
    pI2cDdc0Reg->ddc_ddc_addr = (((tmp & ~(0xff<<16))|(1<<8))|(((UINT32)addr)<<16));
	*value = (UINT8)pI2cDdc0Reg->ddc_test_data;
    pI2cDdc0Reg->ddc_ddc_addr = tmp;
}
EXPORT_SYMBOL(hal_i2cddc0_sram_read);


void hal_i2cddc0_slave_add_config(UINT8 addr)
{
	if (addr <= 0x7f)
		pI2cDdc0Reg->ddc_rom_addr = (UINT32)addr;
}
EXPORT_SYMBOL(hal_i2cddc0_slave_add_config);


void hal_i2cddc0_init(void)
{
	UINT32 tmp;

	tmp = pI2cDdc0Reg->ddc_slvaddr;
	tmp &= (~DDC_IMP_MASK);
	pI2cDdc0Reg->ddc_slvaddr = (tmp | DDC_RW_MASK);
}
EXPORT_SYMBOL(hal_i2cddc0_init);


#if 0 // MOON reg need remap
void hal_i2cddc0_enable(void)
{
	moonReg_t *pmoonReg = (moonReg_t *)LOGI_ADDR_MOON_REG;

	pmoonReg->reset[3] &= (~MOON0_I2CDDC0_MASK);
	pmoonReg->clken[3] |= MOON0_I2CDDC0_MASK;
	pmoonReg->gclken[3] &= (~MOON0_I2CDDC0_MASK);
}
EXPORT_SYMBOL(hal_i2cddc0_enable);


void hal_i2cddc0_disable(void)
{
	moonReg_t *pmoonReg = (moonReg_t *)LOGI_ADDR_MOON_REG;

//	pmoonReg->gclken[3] |= MOON0_I2CDDC0_MASK;
//	pmoonReg->clken[3] &= (~MOON0_I2CDDC0_MASK);
	pmoonReg->reset[3] |= MOON0_I2CDDC0_MASK;
}
EXPORT_SYMBOL(hal_i2cddc0_disable);
#endif




