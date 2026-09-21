/*
 * stm32f401xc_SysTick_driver.h
 *
 * Created on: Aug 26, 2025
 * Author: Ahmed Ezzat
 */

#ifndef INC_STM32F401XC_SYSTICK_DRIVER_H_
#define INC_STM32F401XC_SYSTICK_DRIVER_H_

//------------------------------
// Includes
//------------------------------
#include "stm32f401xc.h"

//============================================================

//------------------------------------------
// User Type Definitions (Structures)
//------------------------------------------
typedef struct{
	uint8_t SysTick_InterruptEnable;   // Specifies GPIO pin to configure
 					   // This Parameter can be set based on @ref SysTick_Interrupt_define

	uint8_t SysTick_CLKSource;         // Specifies GPIO pin to configure
					   // This Parameter can be set based on @ref SysTick_CLK_define
}SysTick_Config_t;

//============================================================

// @ref SysTick_Interrupt_define
// Bit 1 TICKINT: SysTick exception request enable
//	0: Counting down to zero does not assert the SysTick exception request
//	1: Counting down to zero to asserts the SysTick exception request.
#define SysTick_Interrupt_DIS				0
#define SysTick_Interrupt_EN				1

// @ref SysTick_CLK_define
// Bit 2 CLKSOURCE: Clock source selection
//	0: AHB/8
//	1: Processor clock (AHB)
#define SysTick_CLK_AHB_8  				0
#define SysTick_CLK_AHB	   				1

#define COUNTFLAG					16

//============================================================

/*
 * ======================================================
 * 	 APIs Supported by "MCAL SysTick DRIVER"
 * ======================================================
 */

void MCAL_SysTicK_Init(SysTick_Config_t *SysTick_CFG);

void MCAL_SysTicK_StartTimer(uint32_t ReloadValue);
void MCAL_SysTicK_StopTimer(void);

void MCAL_SysTicK_SetDelay_ms(uint32_t Delay_Time_ms);
void MCAL_SysTicK_SetDelay_us(uint32_t Delay_Time_us);

uint32_t MCAL_SysTicK_GetElapsedTime_SingleShot(void);
uint32_t MCAL_SysTicK_GetRemainingTime_SingleShot(void);

uint8_t MCAL_SysTicK_SetInterval_Single(uint32_t Delay_Time_ms, void(* P_Function_CallBack)(void));
void MCAL_SysTicK_SetInterval_Multi(uint32_t Delay_Time_ms, void(* P_Function_CallBack)(void));

#endif /* INC_STM32F401XC_SYSTICK_DRIVER_H_ */
