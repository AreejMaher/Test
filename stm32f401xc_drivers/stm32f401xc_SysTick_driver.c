/*
 * stm32f401xc_SysTick_driver.c
 *
 * Created on: Aug 26, 2025
 * Author: Ahmed Ezzat
 */


#include "stm32f401xc_SysTick_driver.h"

static void(* GP_Function_CallBack)(void) = NULL;
static uint8_t G_SingleFlag = 0;

void MCAL_SysTicK_Init(SysTick_Config_t *SysTick_CFG){
    // Clear SysTick Timer by clear Enable
    SysTick->CTRL &= ~(1 << 0);

    /// Interrupt Enable/Disable
    if(SysTick_Interrupt_EN == (SysTick_CFG->SysTick_InterruptEnable)){
	SysTick->CTRL |= (1 << 1);
    }
    else{
	SysTick->CTRL &= ~(1 << 1);
    }

    // Select Clock source AHB or AHB/8
    if(SysTick_CLK_AHB == (SysTick_CFG->SysTick_CLKSource)){
	SysTick->CTRL |= (1 << 2);
    }
    else if(SysTick_CLK_AHB_8 == (SysTick_CFG->SysTick_CLKSource)){
	SysTick->CTRL &= ~(1 << 2);
    }
}

void MCAL_SysTicK_StartTimer(uint32_t ReloadValue){
    // Load Timer with ReloadValue
    SysTick->LOAD = ReloadValue;

    // Reset Timer
    SysTick->VAL = 0;

    // Start Timer
    SysTick->CTRL |= (1 << 0);
}

void MCAL_SysTicK_StopTimer(void){
    // Stop SysTick Counter
    SysTick->CTRL &= ~(1 << 0);

    // Reset Timer
    SysTick->VAL = 0;
}

void MCAL_SysTicK_SetDelay_ms(uint32_t Delay_Time_ms){
    // When Select  Clock Source = 25Mhz / 8
    uint32_t ticks = (uint32_t)(Delay_Time_ms * 2000.0);

    // Reset Timer
    SysTick->VAL = 0;

    if((ticks >= 0x00000001) && (ticks < 0x00FFFFFF)){
	MCAL_SysTicK_StartTimer(ticks);

	// Wait till Timer Flag is raised
	while(0 == GET_BIT(SysTick->CTRL, COUNTFLAG));
	MCAL_SysTicK_StopTimer();
    }
}

void MCAL_SysTicK_SetDelay_us(uint32_t Delay_Time_us){
    // When Select  Clock Source = 25Mhz / 8
    uint32_t ticks = (uint32_t)(Delay_Time_us * 2);

    // Reset Timer
    SysTick->VAL = 0;

    if((ticks >= 0x00000001) && (ticks < 0x00FFFFFF)){
	MCAL_SysTicK_StartTimer(ticks);

	// Wait till Timer Flag is raised
	while(0 == GET_BIT(SysTick->CTRL, COUNTFLAG));
	MCAL_SysTicK_StopTimer();
    }
}

uint32_t MCAL_SysTicK_GetElapsedTime_SingleShot(void){
    return (SysTick->LOAD) - (SysTick->VAL);
}

uint32_t MCAL_SysTicK_GetRemainingTime_SingleShot(void){
    return (SysTick->VAL);
}

uint8_t MCAL_SysTicK_SetInterval_Single(uint32_t Delay_Time_ms, void(* P_Function_CallBack)(void)){
    G_SingleFlag = 1;

    uint32_t ticks = Delay_Time_ms * 3125;

    GP_Function_CallBack = P_Function_CallBack;

    // Reset Timer
    SysTick->VAL = 0;

    if((ticks >= 0x00000001) && (ticks < 0x00FFFFFF)){
	MCAL_SysTicK_StartTimer(ticks);
    }

    return 1;
}

void MCAL_SysTicK_SetInterval_Multi(uint32_t Delay_Time_ms, void(* P_Function_CallBack)(void)){
    G_SingleFlag = 0;

    uint32_t ticks = Delay_Time_ms * 3125;

    GP_Function_CallBack = P_Function_CallBack;

    // Reset Timer
    SysTick->VAL = 0;

    if((ticks >= 0x00000001) && (ticks < 0x00FFFFFF)){
	MCAL_SysTicK_StartTimer(ticks);
    }
}

// void SysTick_Handler(void){
//     if(GP_Function_CallBack != NULL){
// 	GP_Function_CallBack();
//     }

//     if(1 == G_SingleFlag){
// 	MCAL_SysTicK_StopTimer();
//     }
// }
