/**************************************************************************
 *                                                                        *
 *         Copyright (c) 2015 by Sunplus Inc.                             *
 *                                                                        *
 *  This software is copyrighted by and is the property of Sunplus        *
 *  Inc. All rights are reserved by Sunplus Inc.                          *
 *  This software may only be used in accordance with the                 *
 *  corresponding license agreement. Any unauthorized use, duplication,   *
 *  distribution, or disclosure of this software is expressly forbidden.  *
 *                                                                        *
 *  This Copyright notice MUST not be removed or modified without prior   *
 *  written consent of Sunplus Technology Co., Ltd.                       *
 *                                                                        *
 *  Sunplus Inc. reserves the right to modify this software               *
 *  without notice.                                                       *
 *                                                                        *
 *  Sunplus Inc.                                                          *
 *  19, Innovation First Road, Hsinchu Science Park                       *
 *  Hsinchu City 30078, Taiwan, R.O.C.                                    *
 *                                                                        *
 **************************************************************************/

/**
 * @file sp_gpio.c
 * @brief gpio setting interface
 * @author Nelson Yeh
 */
/********************************************************************************
*                                                            Include File                                                                   *
*********************************************************************************/
#include "hal_gpio.h"
#include "drivers/gpio/sp_gpio.h"


/********************************************************************************
*                                                            Static variables                                                              *
*********************************************************************************/


/********************************************************************************
*                                                            Global variables                                                              *
*********************************************************************************/


/********************************************************************************
*                                                            Macro Function                                                              *
*********************************************************************************/


/********************************************************************************
*                                                            Data Types                                                                   *
*********************************************************************************/


/********************************************************************************
*                                                            Extern Reference                                                           *
*********************************************************************************/


/********************************************************************************
*                                                            Function Declarations                                                     *     
*********************************************************************************/
void GPIO_F_SET(unsigned int a, unsigned int d)
{
	HAL_GPIO_F_SET(a, d);
}

void GPIO_M_SET(unsigned int a, unsigned int d)
{
	HAL_GPIO_M_SET(a, d);
}

void GPIO_E_SET(unsigned int a, unsigned int d)
{
	HAL_GPIO_E_SET(a, d);
}

void GPIO_O_SET(unsigned int a, unsigned int d)
{
	HAL_GPIO_O_SET(a, d);
}

unsigned int GPIO_I_GET(unsigned int a)
{
	return HAL_GPIO_I_GET(a);
}

