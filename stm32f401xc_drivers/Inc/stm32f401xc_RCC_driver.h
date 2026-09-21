/*
 * stm32f401xc_RCC_driver.h
 *
 * Created on: Aug 26, 2025
 * Author: Ahmed Ezzat
 */

#ifndef INC_STM32F401XC_RCC_DRIVER_H_
#define INC_STM32F401XC_RCC_DRIVER_H_

//------------------------------
// Includes
//------------------------------
#include "stm32f401xc.h"

/* RCC_SYS_CLK --- options --- */
#define HSE_CLK		1
#define HSI_CLK		2
#define PLL_CLK		3

/* HSE_BYPASS --- options --- */
#define RC_CLK			0
#define MECHANICAL_CLK  	1

/*RCC_SYS_CLK
 * options:
 * HSE_CLK
 * HSI_CLK
 * PLL_CLK
 *
 * */
#define RCC_SYS_CLK		HSE_CLK

/*HSE_BYPASS
 * options:
 * RC_CLK
 * MECHANICAL_CLK
 *
 * */
#define HSE_BYPASS		MECHANICAL_CLK

//============================================================

/*
 * ======================================================
 * 	 APIs Supported by "MCAL SysTick DRIVER"
 * ======================================================
 */
void MCAL_RCC_Init(void);

#endif /* INC_STM32F401XC_RCC_DRIVER_H_ */
