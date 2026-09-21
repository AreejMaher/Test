/*
 * stm32f401xc_RCC_driver.c
 *
 * Created on: Aug 26, 2025
 * Author: Ahmed Ezzat
 */

#include "stm32f401xc_RCC_driver.h"

void MCAL_RCC_Init(void){

#if RCC_SYS_CLK == HSE_CLK

    // Choose Bypass
#if HSE_BYPASS == RC_CLK
    RCC->CR |= (1 << 18);

#elif HSE_BYPASS == MECHANICAL_CLK
    RCC->CR &= ~(1 << 18);
#endif
    // HSE on
    RCC->CR |= (1 << 16);

    // Wait Till Flag is Set
    while(GET_BIT(RCC->CR,17) == 0);

    // Switch to HSE
    RCC->CFGR &= ~(1 << 1);
    RCC->CFGR |= (1 << 0);

#elif RCC_SYS_CLK == HSI_CLK
    // HSI on
    RCC->CR |= (1 << 0);

    // Wait Till Flag is Set
    while(GET_BIT(RCC->CR,1) == 0);

    // Switch to HSI
    RCC->CFGR &= ~(1 << 1);
    RCC->CFGR &= ~(1 << 0);

#elif RCC_SYS_CLK == PLL_CLK

    // 1. Turn on HSI (Internal 16 MHz Oscillator)
    RCC->CR |= (1 << 0);
    while(GET_BIT(RCC->CR, 1) == 0);

    // 2. Configure Flash Latency (CRITICAL for 84 MHz)
    // We write directly to the FLASH_ACR memory address (0x40023C00)
    // to enable Instruction/Data Cache, Prefetch, and 2 Wait States.
    *((volatile uint32_t*)0x40023C00) = (1 << 8) | (1 << 9) | (1 << 10) | (2 << 0);

    // 3. Configure Bus Prescalers
    RCC->CFGR &= ~(0xF << 4); // AHB  = SYSCLK / 1 (84 MHz)
    RCC->CFGR |=  (4 << 10);  // APB1 = SYSCLK / 2 (42 MHz) - Max for APB1
    RCC->CFGR &= ~(7 << 13);  // APB2 = SYSCLK / 1 (84 MHz) - Max for APB2

    // 4. Configure the Main PLL
    // Input: HSI (16 MHz) | Output: 84 MHz SYSCLK | USB: 48 MHz
    uint32_t pll_m = 16;   // 16 / 16 = 1 MHz VCO In
    uint32_t pll_n = 336;  // 1 * 336 = 336 MHz VCO Out
    uint32_t pll_p = 1;    // 01 = divide by 4. (336 / 4 = 84 MHz CPU Clock)
    uint32_t pll_q = 7;    // 336 / 7 = 48 MHz USB Clock

    // Write configuration to PLLCFGR (Bit 22 is 0 to select HSI as source)
    RCC->PLLCFGR = (pll_q << 24) | (0 << 22) | (pll_p << 16) | (pll_n << 6) | (pll_m << 0);

    // 5. Turn on the PLL
    RCC->CR |= (1 << 24);
    while(GET_BIT(RCC->CR, 25) == 0); // Wait for PLL to lock

    // 6. Switch System Clock to PLL
    RCC->CFGR &= ~(3 << 0); // Clear SW bits
    RCC->CFGR |=  (2 << 0); // Set SW to PLL

    // 7. Wait until the hardware confirms PLL is the System Clock
    while((RCC->CFGR & (3 << 2)) != (2 << 2));

#else
#error "Invalid option"
#endif
}
