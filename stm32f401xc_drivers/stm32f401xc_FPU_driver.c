/*
 * stm32f401xc_FPU_driver.c
 *
 * Created on: Oct 3, 2025
 * Author: Ahmed Ezzat
 */


#include "stm32f401xc_FPU_driver.h"

void MCAL_FPU_Enable(void){
    SCB_CPACR |= (0xF << 20);
}
