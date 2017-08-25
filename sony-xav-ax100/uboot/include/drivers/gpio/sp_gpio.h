#ifndef __SP_GPIO_H__
#define __SP_GPIO_H__

/**
 * @file hal_gpio.h
 * @brief HAL interface for gpio setting
 * @author Nelson Yeh
 */
/********************************************************************************
*                                                            Include File                                                                   *
*********************************************************************************/


/********************************************************************************
*                                                            Macro Function                                                             *
*********************************************************************************/


/********************************************************************************
*                                                            Data Types                                                                   *
*********************************************************************************/
typedef enum
{
	eHW_GPIO_FIRST_FUNC = 0,
	eHW_GPIO_FIRST_GPIO = 1,
	eHW_GPIO_FIRST_NULL
} eHW_GPIO_FIRST;

typedef enum
{
	eHW_GPIO_IOP = 0,
	eHW_GPIO_RISC = 1,
	eHW_GPIO_MASTER_NULL
} eHW_GPIO_Master;

typedef enum
{
	eHW_GPIO_IN = 0,
	eHW_GPIO_OUT = 1,
	eHW_GPIO_OE_NULL
} eHW_GPIO_OE;

typedef enum
{
	eHW_GPIO_STS_LOW = 0,
	eHW_GPIO_STS_HIGH = 1,
	eHW_GPIO_STS_NULL
} eHW_IO_STS;


/********************************************************************************
*                                                            Extern Reference                                                           *
*********************************************************************************/


/********************************************************************************
*                                                            Function Declarations                                                     *     
*********************************************************************************/
extern void GPIO_F_SET(unsigned int a, unsigned int d);
extern void GPIO_M_SET(unsigned int a, unsigned int d);
extern void GPIO_E_SET(unsigned int a, unsigned int d);
extern void GPIO_O_SET(unsigned int a, unsigned int d);
extern unsigned int  GPIO_I_GET(unsigned int a);

#endif

