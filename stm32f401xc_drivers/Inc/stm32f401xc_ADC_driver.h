/*
 * stm32f401xc_ADC_driver.c
 *
 * Created on: Nov 18, 2025
 * Author: Ahmed Ezzat
 */

#ifndef INC_STM32F401XC_ADC_DRIVER_C_
#define INC_STM32F401XC_ADC_DRIVER_C_

#include "stm32f401xc.h"
#include "stm32f401xc_gpio_driver.h"
#include "stm32f401xc_DMA_driver.h"

extern uint8_t G_ADC_pin;

void ADC_Init(GPIO_TypeDef *ADC_port, uint8_t ADC_pin);
void ADC_DeInit(void);
uint16_t ADC_READ(void);

#endif /* INC_STM32F401XC_ADC_DRIVER_C_ */
