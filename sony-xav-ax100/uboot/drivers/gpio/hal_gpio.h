#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

/**
 * @file hal_gpio.h
 * @brief HAL interface for gpio setting
 * @author Nelson Yeh
 */
/********************************************************************************
*                                                            Include File                                                                   *
*********************************************************************************/
#include <common.h>
#include <asm/io.h>
#include <asm/arch/regmap.h>


/********************************************************************************
*                                                            Macro Function                                                             *
*********************************************************************************/
//#define GPIO_DEBUG
#if defined(GPIO_DEBUG)
	#define DEBUG		diag_printf
#else
	#define DEBUG(...)
#endif

#define DERROR			diag_printf

/********************************************************************************
*                                                            Data Types                                                                   *
*********************************************************************************/


/********************************************************************************
*                                                            Extern Reference                                                           *
*********************************************************************************/


/********************************************************************************
*                                                            Function Declarations                                                     *     
*********************************************************************************/
static inline void HAL_GPIO_F_SET(unsigned int a, unsigned int d)
{
	DEBUG("[%s:%d], Param = %d %d\n", __FUNCTION__, __LINE__, a, d);

	if (d)
		GPIO_FIRST_REG->gpio_first[a/32] |= 1<<(a%32);
	else
		GPIO_FIRST_REG->gpio_first[a/32] &= ~(1<<(a%32));
}

static inline void HAL_GPIO_M_SET(unsigned int a, unsigned int d)
{
	DEBUG("[%s:%d], Param = %d %d\n", __FUNCTION__, __LINE__, a, d);

	if (d)
		GPIO_MASTER_REG->gpio_master[a/32] |= 1<<(a%32);
	else
		GPIO_MASTER_REG->gpio_master[a/32] &= ~(1<<(a%32));
}

static inline void HAL_GPIO_E_SET(unsigned int a, unsigned int d)
{
	DEBUG("[%s:%d], Param = %d %d\n", __FUNCTION__, __LINE__, a, d);

	if (d)
		GPIO_OE_REG->gpio_oe[a/32] |= 1<<(a%32);
	else
		GPIO_OE_REG->gpio_oe[a/32] &= ~(1<<(a%32));
}

static inline void HAL_GPIO_O_SET(unsigned int a, unsigned int d)
{
	DEBUG("[%s:%d], Param = %d %d\n", __FUNCTION__, __LINE__, a, d);

	if (d)
		GPIO_OUT_REG->gpio_out[a/32] |= 1<<(a%32);
	else
		GPIO_OUT_REG->gpio_out[a/32] &= ~(1<<(a%32));
}

static inline unsigned int HAL_GPIO_I_GET(unsigned int a)
{
	unsigned int d = (GPIO_IN_REG->gpio_in[a/32] >> (a%32)) & 0x1;

	DEBUG("[%s:%d], Param = %d %d\n", __FUNCTION__, __LINE__, a, d);

	return d;
}

#endif


