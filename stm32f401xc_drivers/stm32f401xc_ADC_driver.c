/*
 * stm32f401xc_ADC_driver.c
 *
 * Created on: Nov 18, 2025
 * Author: Ahmed Ezzat
 */

#include "stm32f401xc_ADC_driver.h"

//================ Polling Driver ================
uint8_t G_ADC_pin = 0;

void ADC_Init(GPIO_TypeDef *ADC_port, uint8_t ADC_pin){
    G_ADC_pin = ADC_pin;

    GPIO_PinConfig_t gpio;
    gpio.GPIO_PinNumber = ADC_pin;
    gpio.GPIO_MODE = GPIO_MODE_Analog;
    gpio.GPIO_PU_PD = GPIO_PU_PD_NONE;
    MCAL_GPIO_Init(ADC_port, &gpio);

    // Enable ADC1 Clock
    RCC_ADC1_CLK_EN();

    // Turn ADC ON (ADON = 1)
    ADC1->CR2 |= (1U << 0);

    // Ensure single conversion default (we'll change CONT later if needed)
    ADC1->CR2 &= ~(1U << 1);

    // <<< DO NOT DISABLE ADC or ADC clock here >>>
}

void ADC_DeInit(void){
    RCC_ADC1_CLK_RST_SET();

    // Disable ADC
    ADC1->CR2 &= ~(1U << 0);
}

uint16_t ADC_READ(void){

    /* Set the channel for single conversion (clear then set) */
    ADC1->SQR3 &= ~(0x1FU << 0);
    ADC1->SQR3 |= ((uint32_t)(G_ADC_pin & 0x1FU) << 0);

    /* Start conversion (software) */
    ADC1->CR2 |= (1U << 30); // SWSTART

    /* Wait for EOC (bit 1) */
    while(! ((ADC1->SR >> 1) & 1U));

    return ((uint16_t)ADC1->DR);
}

void ADC_DMA_TC_Callback(void)
{
    /* keep empty or set a flag here if you want */
}
