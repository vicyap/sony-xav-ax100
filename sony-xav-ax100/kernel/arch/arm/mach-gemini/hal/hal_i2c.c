
#include <linux/delay.h>
#include <mach/hal/regmap/reg_system.h>
#include <mach/hal/hal_i2c.h>
#include <mach/gpio_drv.h>

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

#define I2C_CLK_SOURCE_FREQ	       27000	// KHz (27MHz)
#define I2C_POLLING_DELAY_TIME		30		// ms

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
#define I2C_RESET_MASK				1<<15
#define I2C_SLAVE_ADDR_MASK		0xff
#define I2C_CLKFREQ_MASK		       0x07ff

#define MOON0_I2CM0_MASK			1<<22
#define MOON0_I2CM1_MASK			1<<23


#if 0
#define HAL_BUSY_WAITING(cond, ms)	\
	({	\
		INT32 __timeout = ms * 1000;	\
		while (!(cond)){	\
			if (__timeout < 0){	\
				DIAG_ERROR("[%s] HAL_BUSY_WAITING Time Out!!\n", __FUNCTION__);	\
				break;	\
			}	\
				\
			udelay(1);	\
			__timeout--;	\
		}	\
		__timeout; \
	})
#endif

#define HAL_BUSY_WAITING(cond, ms)	\
	({								\
		INT32 __timeout = ms * 1000;	\
		while (!(cond)){				\
			if (__timeout < 0){			\
				break;				\
			}						\
									\
			udelay(1);				\
			__timeout--;				\
		}							\
		__timeout;					\
	})

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/

regs_i2cm0_t *pI2cM0Reg = (regs_i2cm0_t *)LOGI_ADDR_I2CM0_REG;
regs_i2cm1_t *pI2cM1Reg = (regs_i2cm1_t *)LOGI_ADDR_I2CM1_REG;

static UINT32 M0_REG_0_TMP = 0;
static UINT32 M1_REG_0_TMP = 0;

/**************************************************************************
 *             F U N C T I O N    I M P L E M E N T A T I O N S           *
 **************************************************************************/

#if 0
INT32
hal_i2c_reg_mmap(
	void
)
{
	request_mem_region(I2C_REG_BASE, I2C_REG_SIZE, "I2C_Register_Map");
	pHWRegBase = ioremap(I2C_REG_BASE, I2C_REG_SIZE);
	if (pHWRegBase == NULL)
	{
		ERR("ioremap for HW register failed\n");
		return -1;
	}
	pI2cM0Reg = (i2c_m0_reg *)(pHWRegBase + I2C_M0_REG_OFFSET);
	pI2cM1Reg = (i2c_m1_reg *)(pHWRegBase + I2C_M1_REG_OFFSET);

	return 0;
}
EXPORT_SYMBOL(hal_i2c_reg_mmap);

void
hal_i2c_reg_unmmap(
	void
)
{
	iounmap(pHWRegBase);
}
EXPORT_SYMBOL(hal_i2c_reg_unmmap);
#endif


void hal_i2cm0_enable(void)
{
	moonReg_t *pmoonReg = (moonReg_t *)LOGI_ADDR_MOON_REG;

	pmoonReg->reset[3] &= (~MOON0_I2CM0_MASK);
	pmoonReg->clken[3] |= MOON0_I2CM0_MASK;
	pmoonReg->gclken[3] &= (~MOON0_I2CM0_MASK);
}
EXPORT_SYMBOL(hal_i2cm0_enable);


void hal_i2cm0_disable(void)
{
	moonReg_t *pmoonReg = (moonReg_t *)LOGI_ADDR_MOON_REG;

//	pmoonReg->gclken[3] |= MOON0_I2CM0_MASK;
//	pmoonReg->clken[3] &= (~MOON0_I2CM0_MASK);
	pmoonReg->reset[3] |= MOON0_I2CM0_MASK;
}
EXPORT_SYMBOL(hal_i2cm0_disable);

void hal_i2cm0_setpinmux(int cfg)
{
    switch (cfg){
    case 4:
        GPIO_F_SET(168,0);
        GPIO_F_SET(167,0);
        break;
    case 3:
        GPIO_F_SET(123,0);
        GPIO_F_SET(124,0);
        break;

    case 2:
        GPIO_F_SET(71,0);
        GPIO_F_SET(72,0);
        break;

    case 1:
        GPIO_F_SET(22,0);
        GPIO_F_SET(23,0);
        break;

    case 0:
        break;

    default:
        return;
    }
	GPIO_PIN_MUX_SEL(PMX_I2CM0, cfg);
}
EXPORT_SYMBOL(hal_i2cm0_setpinmux);


void hal_i2cm0_reg_set(UINT32 offset, UINT32 val)
{
	*((UINT32 *)pI2cM0Reg+offset) = val;
}
EXPORT_SYMBOL(hal_i2cm0_reg_set);


void hal_i2cm0_reg_get(UINT32 offset, UINT32 *val)
{
	 *val = *((UINT32 *)pI2cM0Reg+offset);
}
EXPORT_SYMBOL(hal_i2cm0_reg_get);


void hal_i2cm0_reset(void)
{
	pI2cM0Reg->control0 |= I2C_RESET_MASK;
	udelay(2);
//	pI2cM0Reg->control0 &= (~I2C_RESET_MASK);
	M0_REG_0_TMP = 0;
}
EXPORT_SYMBOL(hal_i2cm0_reset);


void hal_i2cm0_data_set(UINT32 *wdata)
{
	pI2cM0Reg->data00_03 = wdata[0];
	pI2cM0Reg->data04_07 = wdata[1];
	pI2cM0Reg->data08_11 = wdata[2];
	pI2cM0Reg->data12_15 = wdata[3];
	pI2cM0Reg->data16_19 = wdata[4];
	pI2cM0Reg->data20_23 = wdata[5];
	pI2cM0Reg->data24_27 = wdata[6];
	pI2cM0Reg->data28_31 = wdata[7];
}
EXPORT_SYMBOL(hal_i2cm0_data_set);


void hal_i2cm0_data_get(UINT32 *rdata)
{
	rdata[0] = pI2cM0Reg->data00_03;
	rdata[1] = pI2cM0Reg->data04_07;
	rdata[2] = pI2cM0Reg->data08_11;
	rdata[3] = pI2cM0Reg->data12_15;
	rdata[4] = pI2cM0Reg->data16_19;
	rdata[5] = pI2cM0Reg->data20_23;
	rdata[6] = pI2cM0Reg->data24_27;
	rdata[7] = pI2cM0Reg->data28_31;
}
EXPORT_SYMBOL(hal_i2cm0_data_get);


void hal_i2cm0_data0_set(UINT32 *wdata)
{
	pI2cM0Reg->data00_03 = wdata[0];
}
EXPORT_SYMBOL(hal_i2cm0_data0_set);


void hal_i2cm0_data_single_get(UINT32 *rdata, UINT32 num, UINT8 bytes_eachtime)
{
	if (bytes_eachtime == 4){
	    switch(num){
	            case 7:
	                *rdata = pI2cM0Reg->data28_31;
	                break;
	            case 6:
	                *rdata = pI2cM0Reg->data24_27;
	                break;
	            case 5:
	                *rdata = pI2cM0Reg->data20_23;
	                break;
	            case 4:
	                *rdata = pI2cM0Reg->data16_19;
	                break;
	            case 3:
	                *rdata = pI2cM0Reg->data12_15;
	                break;
	            case 2:
	                *rdata = pI2cM0Reg->data08_11;
	                break;
	            case 1:
	                *rdata = pI2cM0Reg->data04_07;
	                break;
	            case 0:
	                *rdata = pI2cM0Reg->data00_03;
	                break;
	            default:
	                break;
	        }
	}
	else{
	    switch(num){
	            case 1:
	                rdata[0] = pI2cM0Reg->data16_19;
	                rdata[1] = pI2cM0Reg->data20_23;
	                rdata[2] = pI2cM0Reg->data24_27;
	                rdata[3] = pI2cM0Reg->data28_31;
	                break;
	            case 0:
	                rdata[0] = pI2cM0Reg->data00_03;
	                rdata[1] = pI2cM0Reg->data04_07;
	                rdata[2] = pI2cM0Reg->data08_11;
	                rdata[3] = pI2cM0Reg->data12_15;
	               break;
	            default:
	                break;
	        }
	}

}
EXPORT_SYMBOL(hal_i2cm0_data_single_get);

void hal_i2cm0_trans_cnt_set(UINT8 write_cnt, UINT8 read_cnt)
{
	pI2cM0Reg->control7 = ((UINT32)read_cnt<<16) | (UINT32)write_cnt ;
}
EXPORT_SYMBOL(hal_i2cm0_trans_cnt_set);


void hal_i2cm0_rw_mode_set(UINT8 rw_mode, UINT8 sub_addr_en, I2C_READ_MODE read_mode)
{
	UINT32 r_tmp;

//	r_tmp = pI2cM0Reg->control0 & (~0x00070000);
	r_tmp = M0_REG_0_TMP & (~0x00070000);

	if (rw_mode){
		if (read_mode == RESTART_MODE)
			r_tmp |= (1<<17);
		if (sub_addr_en)
			r_tmp |= (1<<16);
		r_tmp |= (1<<18);
	}

//	pI2cM0Reg->control0 = r_tmp;
	M0_REG_0_TMP = r_tmp;
}
EXPORT_SYMBOL(hal_i2cm0_rw_mode_set);


void hal_i2cm0_active_mode_set(I2C_ACTIVE_MODE mode)
{
	switch(mode){
		default:
		case I2C_TRIGGER:
			pI2cM0Reg->i2cm_mode &= (~0x00000003);
			break;

		case I2C_AUTO:
			pI2cM0Reg->i2cm_mode &= (~0x00000003);
			pI2cM0Reg->i2cm_mode |= 0x00000002;
			break;
	}
}
EXPORT_SYMBOL(hal_i2cm0_active_mode_set);


void hal_i2cm0_manual_trigger(void)
{
	pI2cM0Reg->control0 = M0_REG_0_TMP;
	udelay(4);
	pI2cM0Reg->i2cm_mode |= 0x00000001;
}
EXPORT_SYMBOL(hal_i2cm0_manual_trigger);


void hal_i2cm0_slave_add_set(UINT8 addr)
{
//	pI2cM0Reg->control0 &= (~I2C_SLAVE_ADDR_MASK);
//	pI2cM0Reg->control0 |= ((UINT32)addr << 1);
	M0_REG_0_TMP &= (~I2C_SLAVE_ADDR_MASK);
	M0_REG_0_TMP |= ((UINT32)addr << 1);
}
EXPORT_SYMBOL(hal_i2cm0_slave_add_set);


void hal_i2cm0_clock_freq_set(UINT32 freq)
{
	UINT32 div;

	div = I2C_CLK_SOURCE_FREQ/freq;
	if (0 != I2C_CLK_SOURCE_FREQ%freq)
		div += 1;

	if (div > 0x7ff)
		div = 0x7ff;

//	pI2cM0Reg->control0 &= 0xf8ffffff;
	M0_REG_0_TMP &= 0xf8ffffff;
	pI2cM0Reg->control2 = ((pI2cM0Reg->control2 & (~I2C_CLKFREQ_MASK)) | (div  & I2C_CLKFREQ_MASK));
}
EXPORT_SYMBOL(hal_i2cm0_clock_freq_set);


void hal_i2cm0_status_get(UINT32 *flag)
{
	*flag = (pI2cM0Reg->int_status & I2C_ALL_FLAG);
}
EXPORT_SYMBOL(hal_i2cm0_status_get);


void hal_i2cm0_status_clear(UINT32 flag)
{
	pI2cM0Reg->control1 = ((pI2cM0Reg->control1 & (~I2C_ALL_FLAG)) | (flag & I2C_ALL_FLAG));
	pI2cM0Reg->control1 = 0x00;
}
EXPORT_SYMBOL(hal_i2cm0_status_clear);


void hal_i2cm0_rdata_flag_get(UINT32 *flag)
{
	*flag = (pI2cM0Reg->i2cm_status3);
}
EXPORT_SYMBOL(hal_i2cm0_rdata_flag_get);


void hal_i2cm0_rdata_flag_clear(UINT32 flag)
{
	pI2cM0Reg->control6 = flag;
	pI2cM0Reg->control6 = 0;
}
EXPORT_SYMBOL(hal_i2cm0_rdata_flag_clear);


void hal_i2cm0_roverflow_flag_get(UINT32 *flag)
{
	*flag = (pI2cM0Reg->i2cm_status4);
}
EXPORT_SYMBOL(hal_i2cm0_roverflow_flag_get);


void hal_i2cm0_fifo_cnt_get(UINT32 *cnt)
{
	*cnt = (pI2cM0Reg->i2cm_status0);
}
EXPORT_SYMBOL(hal_i2cm0_fifo_cnt_get);


INT32 hal_i2cm0_address_ack_wait(void)
{
	INT32 ret;

	ret = HAL_BUSY_WAITING(((pI2cM0Reg->int_status & I2C_ADDRESS_NACK_FLAG) != I2C_ADDRESS_NACK_FLAG), I2C_POLLING_DELAY_TIME);

	if (ret >= 0) {
  		ret = SP_OK;
  	} else {
  		ret = SP_FAIL;
  	}

	return ret;
}
EXPORT_SYMBOL(hal_i2cm0_address_ack_wait);


INT32 hal_i2cm0_busy_wait(void)
{
	INT32 ret;

	ret = HAL_BUSY_WAITING(((pI2cM0Reg->int_status & I2C_SIFBUSY_FLAG) != I2C_SIFBUSY_FLAG), I2C_POLLING_DELAY_TIME);

	if (ret >= 0) {
  		ret = SP_OK;
  	} else {
  		ret = SP_FAIL;
  	}

	return ret;
}
EXPORT_SYMBOL(hal_i2cm0_busy_wait);


INT32 hal_i2cm0_complete_wait(void)
{
	INT32 ret;

	ret = HAL_BUSY_WAITING(((pI2cM0Reg->int_status & I2C_ACTIVE_DONE_FLAG) == I2C_ACTIVE_DONE_FLAG), I2C_POLLING_DELAY_TIME);

	if (ret >= 0) {
        printk("[I2C_M0] over flow. %x\n", pI2cM0Reg->i2cm_status4);
 	    ret = SP_OK;
  	} else {
		printk("[I2C_M0] wait complete timeout.\n");
  		ret = SP_FAIL;
  	}

	return ret;
}
EXPORT_SYMBOL(hal_i2cm0_complete_wait);


INT32 hal_i2cm0_ring_wait(void)
{
	INT32 ret;

//	ret = HAL_BUSY_WAITING(((pI2cM0Reg->control5 & 0x00e00000) >= (byte_4<<21)), I2C_POLLING_DELAY_TIME);
	ret = HAL_BUSY_WAITING(((pI2cM0Reg->control5 & 0x00e00000)>0), I2C_POLLING_DELAY_TIME);

	if (ret >= 0) {
  		ret = SP_OK;
  	} else {
		printk("[I2C_M0] wait ring fifo ready timeout.\n");
  		ret = SP_FAIL;
  	}

	return ret;
}
EXPORT_SYMBOL(hal_i2cm0_ring_wait);


INT32 hal_i2cm0_rdara_ready_wait(UINT32 bit_num)
{
	INT32 ret;

	ret = HAL_BUSY_WAITING(((pI2cM0Reg->i2cm_status3 & (1<<bit_num))>0), 150);

	if (ret >= 0) {
  		ret = SP_OK;
  	} else {
		printk("[I2C_M0] wait rdata ready timeout. status = 0x%x\n", pI2cM0Reg->i2cm_status3);
  		ret = SP_FAIL;
  	}

	pI2cM0Reg->control6 |= (0x0f<<(bit_num-3));
	pI2cM0Reg->control6 = 0;

	return ret;
}
EXPORT_SYMBOL(hal_i2cm0_rdara_ready_wait);


void hal_i2cm0_int_en(
	UINT32 int0,	//0x08
	UINT32 int1,	//0x0d
	UINT32 int2,	//0x10
	UINT32 en
)
{
	if (en){
		pI2cM0Reg->int_en0 = int0;
		pI2cM0Reg->int_en1 = int1;
		pI2cM0Reg->int_en2 = int2;
	}
	else
		pI2cM0Reg->int_en0 = pI2cM0Reg->int_en1 = pI2cM0Reg->int_en2 = 0;
}
EXPORT_SYMBOL(hal_i2cm0_int_en);


/*----------------------------------------------------------------------------------------------------------------------------------*/


void hal_i2cm1_enable(void)
{
	moonReg_t *pmoonReg = (moonReg_t *)LOGI_ADDR_MOON_REG;

	pmoonReg->reset[3] &= (~MOON0_I2CM1_MASK);
	pmoonReg->clken[3] |= MOON0_I2CM1_MASK;
	pmoonReg->gclken[3] &= (~MOON0_I2CM1_MASK);
}
EXPORT_SYMBOL(hal_i2cm1_enable);


void hal_i2cm1_disable(void)
{
	moonReg_t *pmoonReg = (moonReg_t *)LOGI_ADDR_MOON_REG;

//	pmoonReg->gclken[3] |= MOON0_I2CM1_MASK;
//	pmoonReg->clken[3] &= (~MOON0_I2CM1_MASK);
	pmoonReg->reset[3] |= MOON0_I2CM1_MASK;
}
EXPORT_SYMBOL(hal_i2cm1_disable);

void hal_i2cm1_setpinmux(int cfg)
{
	//touchReg_t *touchreg = (touchReg_t *)LOGI_ADDR_TOUCH_REG;

	switch (cfg){
	    case 4:
	        GPIO_F_SET(125,0);
	        GPIO_F_SET(126,0);
	        break;

		case 3:
			GPIO_F_SET(167,0);
			GPIO_F_SET(168,0);
			break;

		case 2:
            GPIO_F_SET(22,0);
            GPIO_F_SET(23,0);
			break;

		case 1:
            GPIO_F_SET(71,0);
            GPIO_F_SET(72,0);
			break;

		case 0:
			break;

		default:
			return;
	}
	GPIO_PIN_MUX_SEL(PMX_I2CM1, cfg);
}
EXPORT_SYMBOL(hal_i2cm1_setpinmux);

void hal_i2cm1_reg_set(UINT32 offset, UINT32 val)
{
	*((UINT32 *)pI2cM1Reg+offset) = val;
}
EXPORT_SYMBOL(hal_i2cm1_reg_set);


void hal_i2cm1_reg_get(UINT32 offset, UINT32 *val)
{
	 *val = *((UINT32 *)pI2cM1Reg+offset);
}
EXPORT_SYMBOL(hal_i2cm1_reg_get);


void hal_i2cm1_reset(void)
{
	pI2cM1Reg->control0 |= I2C_RESET_MASK;
	udelay(2);
//	pI2cM1Reg->control0 &= (~I2C_RESET_MASK);
	M1_REG_0_TMP = 0;
}
EXPORT_SYMBOL(hal_i2cm1_reset);


void hal_i2cm1_data_set(UINT32 *wdata)
{
	pI2cM1Reg->data00_03 = wdata[0];
	pI2cM1Reg->data04_07 = wdata[1];
	pI2cM1Reg->data08_11 = wdata[2];
	pI2cM1Reg->data12_15 = wdata[3];
	pI2cM1Reg->data16_19 = wdata[4];
	pI2cM1Reg->data20_23 = wdata[5];
	pI2cM1Reg->data24_27 = wdata[6];
	pI2cM1Reg->data28_31 = wdata[7];
}
EXPORT_SYMBOL(hal_i2cm1_data_set);


void hal_i2cm1_data_get(UINT32 *rdata)
{
	rdata[0] = pI2cM1Reg->data00_03;
	rdata[1] = pI2cM1Reg->data04_07;
	rdata[2] = pI2cM1Reg->data08_11;
	rdata[3] = pI2cM1Reg->data12_15;
	rdata[4] = pI2cM1Reg->data16_19;
	rdata[5] = pI2cM1Reg->data20_23;
	rdata[6] = pI2cM1Reg->data24_27;
	rdata[7] = pI2cM1Reg->data28_31;
}
EXPORT_SYMBOL(hal_i2cm1_data_get);


void hal_i2cm1_data0_set(UINT32 *wdata)
{
	pI2cM1Reg->data00_03 = wdata[0];
}
EXPORT_SYMBOL(hal_i2cm1_data0_set);


void hal_i2cm1_data_single_get(UINT32 *rdata, UINT32 num,UINT8 bytes_eachtime)
{
	if (bytes_eachtime==4){
	    switch(num){
	    case 7:
	        *rdata = pI2cM1Reg->data28_31;
	        break;
	    case 6:
	        *rdata = pI2cM1Reg->data24_27;
	        break;
	    case 5:
	        *rdata = pI2cM1Reg->data20_23;
	        break;
	    case 4:
	        *rdata = pI2cM1Reg->data16_19;
	        break;
	    case 3:
	        *rdata = pI2cM1Reg->data12_15;
	        break;
	    case 2:
	        *rdata = pI2cM1Reg->data08_11;
	        break;
	    case 1:
	        *rdata = pI2cM1Reg->data04_07;
	        break;
	    case 0:
	        *rdata = pI2cM1Reg->data00_03;
	        break;
	    default:
	        break;
	    }
	}else{
	    switch(num){
	    case 1:
	        rdata[0] = pI2cM1Reg->data16_19;
	        rdata[1] = pI2cM1Reg->data20_23;
	        rdata[2] = pI2cM1Reg->data24_27;
	        rdata[3] = pI2cM1Reg->data28_31;
	        break;
	    case 0:
	        rdata[0] = pI2cM1Reg->data00_03;
	        rdata[1] = pI2cM1Reg->data04_07;
	        rdata[2] = pI2cM1Reg->data08_11;
	        rdata[3] = pI2cM1Reg->data12_15;
	        break;
	    default:
	        break;
	    }
	}


}
EXPORT_SYMBOL(hal_i2cm1_data_single_get);


void hal_i2cm1_trans_cnt_set(UINT8 write_cnt, UINT8 read_cnt)
{
	pI2cM1Reg->control7 = ((UINT32)read_cnt<<16) | (UINT32)write_cnt ;
}
EXPORT_SYMBOL(hal_i2cm1_trans_cnt_set);


void hal_i2cm1_rw_mode_set(UINT8 rw_mode, UINT8 sub_addr_en, I2C_READ_MODE read_mode)
{
	UINT32 r_tmp;

//	r_tmp = pI2cM1Reg->control0 & (~0x00070000);
	r_tmp = M1_REG_0_TMP & (~0x00070000);

	if (rw_mode){
		if (read_mode == RESTART_MODE)
			r_tmp |= (1<<17);
		if (sub_addr_en)
			r_tmp |= (1<<16);
		r_tmp |= (1<<18);
	}

//	pI2cM1Reg->control0 = r_tmp;
	M1_REG_0_TMP = r_tmp;
}
EXPORT_SYMBOL(hal_i2cm1_rw_mode_set);


void hal_i2cm1_active_mode_set(I2C_ACTIVE_MODE mode)
{
	switch(mode){
		default:
		case I2C_TRIGGER:
			pI2cM1Reg->i2cm_mode &= (~0x00000003);
			break;

		case I2C_AUTO:
			pI2cM1Reg->i2cm_mode &= (~0x00000003);
			pI2cM1Reg->i2cm_mode |= 0x00000002;
			break;
	}
}
EXPORT_SYMBOL(hal_i2cm1_active_mode_set);


void hal_i2cm1_manual_trigger(void)
{
	pI2cM1Reg->control0 = M1_REG_0_TMP;
	udelay(4);
	pI2cM1Reg->i2cm_mode |= 0x00000001;
}
EXPORT_SYMBOL(hal_i2cm1_manual_trigger);


void hal_i2cm1_slave_add_set(UINT8 addr)
{
//	pI2cM1Reg->control0 &= (~I2C_SLAVE_ADDR_MASK);
//	pI2cM1Reg->control0 |= ((UINT32)addr << 1);
	M1_REG_0_TMP &= (~I2C_SLAVE_ADDR_MASK);
	M1_REG_0_TMP |= ((UINT32)addr << 1);
}
EXPORT_SYMBOL(hal_i2cm1_slave_add_set);


void hal_i2cm1_clock_freq_set(UINT32 freq)
{
	UINT32 div;

	div = I2C_CLK_SOURCE_FREQ/freq;
	if (0 != I2C_CLK_SOURCE_FREQ%freq)
		div += 1;

	if (div > 0x7ff)
		div = 0x7ff;

//	pI2cM1Reg->control0 &= 0xf8ffffff;
	M1_REG_0_TMP &= 0xf8ffffff;
	pI2cM1Reg->control2 = ((pI2cM1Reg->control2 & (~I2C_CLKFREQ_MASK)) | (div  & I2C_CLKFREQ_MASK));
}
EXPORT_SYMBOL(hal_i2cm1_clock_freq_set);


void hal_i2cm1_status_get(UINT32 *flag)
{
	*flag = (pI2cM1Reg->int_status & I2C_ALL_FLAG);
}
EXPORT_SYMBOL(hal_i2cm1_status_get);


void hal_i2cm1_status_clear(UINT32 flag)
{
	pI2cM1Reg->control1 = ((pI2cM1Reg->control1 & (~I2C_ALL_FLAG)) | (flag & I2C_ALL_FLAG));
	pI2cM1Reg->control1 = 0x00;
}
EXPORT_SYMBOL(hal_i2cm1_status_clear);


void hal_i2cm1_rdata_flag_get(UINT32 *flag)
{
	*flag = (pI2cM1Reg->i2cm_status3);
}
EXPORT_SYMBOL(hal_i2cm1_rdata_flag_get);


void hal_i2cm1_rdata_flag_clear(UINT32 flag)
{
	pI2cM1Reg->control6 = flag;
	pI2cM1Reg->control6 = 0;
}
EXPORT_SYMBOL(hal_i2cm1_rdata_flag_clear);


void hal_i2cm1_roverflow_flag_get(UINT32 *flag)
{
	*flag = (pI2cM1Reg->i2cm_status4);
}
EXPORT_SYMBOL(hal_i2cm1_roverflow_flag_get);


void hal_i2cm1_fifo_cnt_get(UINT32 *cnt)
{
	*cnt = (pI2cM1Reg->i2cm_status0);
}
EXPORT_SYMBOL(hal_i2cm1_fifo_cnt_get);


INT32 hal_i2cm1_address_ack_wait(void)
{
	INT32 ret;

	ret = HAL_BUSY_WAITING(((pI2cM1Reg->int_status & I2C_ADDRESS_NACK_FLAG) != I2C_ADDRESS_NACK_FLAG), I2C_POLLING_DELAY_TIME);

	if (ret >= 0) {
  		ret = SP_OK;
  	} else {
  		ret = SP_FAIL;
  	}

	return ret;
}
EXPORT_SYMBOL(hal_i2cm1_address_ack_wait);


INT32 hal_i2cm1_busy_wait(void)
{
	INT32 ret;

	ret = HAL_BUSY_WAITING(((pI2cM1Reg->int_status & I2C_SIFBUSY_FLAG) != I2C_SIFBUSY_FLAG), I2C_POLLING_DELAY_TIME);

	if (ret >= 0) {
  		ret = SP_OK;
  	} else {
  		ret = SP_FAIL;
  	}

	return ret;
}
EXPORT_SYMBOL(hal_i2cm1_busy_wait);


INT32 hal_i2cm1_complete_wait(void)
{
	INT32 ret;

	ret = HAL_BUSY_WAITING(((pI2cM1Reg->int_status & I2C_ACTIVE_DONE_FLAG) == I2C_ACTIVE_DONE_FLAG), I2C_POLLING_DELAY_TIME);

	if (ret >= 0) {
  		ret = SP_OK;
  	} else {
		printk("[I2C_M1] wait complete timeout.\n");
  		ret = SP_FAIL;
  	}

	return ret;
}
EXPORT_SYMBOL(hal_i2cm1_complete_wait);


INT32 hal_i2cm1_ring_wait(void)
{
	INT32 ret;

//	ret = HAL_BUSY_WAITING(((pI2cM1Reg->control5 & 0x00e00000) >= (byte_4<<21)), I2C_POLLING_DELAY_TIME);
	ret = HAL_BUSY_WAITING(((pI2cM1Reg->control5 & 0x00e00000)>0), I2C_POLLING_DELAY_TIME);

	if (ret >= 0) {
  		ret = SP_OK;
  	} else {
		printk("[I2C_M1] wait ring fifo ready timeout.\n");
  		ret = SP_FAIL;
  	}

	return ret;
}
EXPORT_SYMBOL(hal_i2cm1_ring_wait);


INT32 hal_i2cm1_rdara_ready_wait(UINT32 bit_num)
{
	INT32 ret;

	ret = HAL_BUSY_WAITING(((pI2cM1Reg->i2cm_status3 & (1<<bit_num))>0), 150);

	if (ret >= 0) {
  		ret = SP_OK;
  	} else {
		printk("[I2C_M1] wait rdata ready timeout. status = 0x%x\n", pI2cM1Reg->i2cm_status3);
  		ret = SP_FAIL;
  	}

	pI2cM1Reg->control6 |= (0x0f<<(bit_num-3));
	pI2cM1Reg->control6 = 0;

	return ret;
}
EXPORT_SYMBOL(hal_i2cm1_rdara_ready_wait);


void hal_i2cm1_int_en(
	UINT32 int0,	//0x08
	UINT32 int1,	//0x0d
	UINT32 int2,	//0x10
	UINT32 en
)
{
	if (en){
		pI2cM1Reg->int_en0 = int0;
		pI2cM1Reg->int_en1 = int1;
		pI2cM1Reg->int_en2 = int2;
	}
	else
		pI2cM1Reg->int_en0 = pI2cM1Reg->int_en1 = pI2cM1Reg->int_en2 = 0;
}
EXPORT_SYMBOL(hal_i2cm1_int_en);

