/*
 * stm32f401xc.h
 *
 * Created on: Jul 19, 2025
 * Author: Ahmed Ezzat
 */

#ifndef STM32F401XC_H_
#define STM32F401XC_H_

//------------------------------
// Includes
//------------------------------
#include <stdint.h>
#include <stddef.h>   // NULL (fixes DMA / EXTI driver errors)

//------------------------------------------------------------
// CMSIS core configuration
// These MUST be defined BEFORE core_cm4.h is included.
//------------------------------------------------------------
#define __CM4_REV               0x0001U   /* Core revision r0p1               */
#define __MPU_PRESENT           0U        /* MPU present                      */
#define __NVIC_PRIO_BITS        4U        /* 4 priority bits                  */
#define __Vendor_SysTickConfig  0U        /* Use CMSIS SysTick_Config()       */

#ifndef __FPU_PRESENT                     /* guarded: avoids "redefined" if   */
#define __FPU_PRESENT           1U        /* -D__FPU_PRESENT=1 is also passed */
#endif

//------------------------------------------------------------
// Interrupt numbers (required by core_cm4.h as IRQn_Type)
//------------------------------------------------------------
typedef enum
{
    /* ---- Cortex-M4 processor exceptions ---- */
    NonMaskableInt_IRQn     = -14,
    HardFault_IRQn          = -13,
    MemoryManagement_IRQn   = -12,
    BusFault_IRQn           = -11,
    UsageFault_IRQn         = -10,
    SVCall_IRQn             = -5,
    DebugMonitor_IRQn       = -4,
    PendSV_IRQn             = -2,
    SysTick_IRQn            = -1,

    /* ---- STM32F401xC peripheral interrupts ---- */
    WWDG_IRQn               = 0,
    PVD_IRQn                = 1,
    TAMP_STAMP_IRQn         = 2,
    RTC_WKUP_IRQn           = 3,
    FLASH_IRQn              = 4,
    RCC_IRQn                = 5,
    EXTI0_IRQn              = 6,
    EXTI1_IRQn              = 7,
    EXTI2_IRQn              = 8,
    EXTI3_IRQn              = 9,
    EXTI4_IRQn              = 10,
    DMA1_Stream0_IRQn       = 11,
    DMA1_Stream1_IRQn       = 12,
    DMA1_Stream2_IRQn       = 13,
    DMA1_Stream3_IRQn       = 14,
    DMA1_Stream4_IRQn       = 15,
    DMA1_Stream5_IRQn       = 16,
    DMA1_Stream6_IRQn       = 17,
    ADC_IRQn                = 18,
    EXTI9_5_IRQn            = 23,
    TIM1_BRK_TIM9_IRQn      = 24,
    TIM1_UP_TIM10_IRQn      = 25,
    TIM1_TRG_COM_TIM11_IRQn = 26,
    TIM1_CC_IRQn            = 27,
    TIM2_IRQn               = 28,
    TIM3_IRQn               = 29,
    TIM4_IRQn               = 30,
    I2C1_EV_IRQn            = 31,
    I2C1_ER_IRQn            = 32,
    I2C2_EV_IRQn            = 33,
    I2C2_ER_IRQn            = 34,
    SPI1_IRQn               = 35,
    SPI2_IRQn               = 36,
    USART1_IRQn             = 37,
    USART2_IRQn             = 38,
    EXTI15_10_IRQn          = 40,
    RTC_Alarm_IRQn          = 41,
    OTG_FS_WKUP_IRQn        = 42,
    DMA1_Stream7_IRQn       = 47,
    SDIO_IRQn               = 49,
    TIM5_IRQn               = 50,
    SPI3_IRQn               = 51,
    DMA2_Stream0_IRQn       = 56,
    DMA2_Stream1_IRQn       = 57,
    DMA2_Stream2_IRQn       = 58,
    DMA2_Stream3_IRQn       = 59,
    DMA2_Stream4_IRQn       = 60,
    OTG_FS_IRQn             = 67,
    DMA2_Stream5_IRQn       = 68,
    DMA2_Stream6_IRQn       = 69,
    DMA2_Stream7_IRQn       = 70,
    USART6_IRQn             = 71,
    I2C3_EV_IRQn            = 72,
    I2C3_ER_IRQn            = 73,
    FPU_IRQn                = 81,
    SPI4_IRQn               = 84
} IRQn_Type;

/* CMSIS core header: provides SCB, SysTick, NVIC, SysTick_Config(), NVIC_* ... */
#include "core_cm4.h"

//------------------------------
// Base Addresses for Memories
//------------------------------
#define SYSTEM_MEMORY_BASE                          0x1FFF0000UL
#define FLASH_MEMORY_BASE                           0x08000000UL
#define SRAM_MEMORY_BASE                            0x20000000UL

/* NOTE: SCB_BASE, NVIC_BASE and SysTick_BASE are now provided by core_cm4.h
 *       (same addresses as the ones that used to be defined here). */

#ifndef SCB_CPACR
#define SCB_CPACR                                   (*(volatile uint32_t*)0xE000ED88UL)
#endif

#define Peripherals_BASE                            0x40000000UL

#define Cortex_M4_Internal_Peripherals_BASE         0xE0000000UL

/* Compatibility aliases for the old names used in the drivers */
#define SysTicK_BASE                                SysTick_BASE
typedef SysTick_Type                                SysTick_TypeDef;

//-----------------------------
// NVIC set-enable / clear-enable registers (from CMSIS NVIC_BASE)
//-----------------------------
#define NVIC_ISER0_BASE                             (*(volatile uint32_t *)(NVIC_BASE + 0x00UL))
#define NVIC_ISER1_BASE                             (*(volatile uint32_t *)(NVIC_BASE + 0x04UL))
#define NVIC_ISER2_BASE                             (*(volatile uint32_t *)(NVIC_BASE + 0x08UL))

#define NVIC_ICER0_BASE                             (*(volatile uint32_t *)(NVIC_BASE + 0x80UL))
#define NVIC_ICER1_BASE                             (*(volatile uint32_t *)(NVIC_BASE + 0x84UL))
#define NVIC_ICER2_BASE                             (*(volatile uint32_t *)(NVIC_BASE + 0x88UL))

//============================================================

//-----------------------------------------------------
// Base Addresses for Bus Peripherals
//-----------------------------------------------------

//---------------------------------------
// Base Addresses for AHB1 Peripherals
//---------------------------------------

// FMI
#define FMI_BASE                                    0x40023C00UL

// RCC
#define RCC_BASE                                    0x40023800UL

//---------------------------------------
// Base Addresses for APB1 Peripherals
//---------------------------------------

// DMA 1 & 2
#define DMA1_BASE                                   0x40026000UL
#define DMA2_BASE                                   0x40026400UL

// USART 2
#define USART2_BASE                                 0x40004400UL

// SPI 2 & 3
#define SPI2_BASE                                   0x40003800UL
#define SPI3_BASE                                   0x40003C00UL

// I2C 1 & 2 & 3
#define I2C1_BASE                                   0x40005400UL
#define I2C2_BASE                                   0x40005800UL
#define I2C3_BASE                                   0x40005C00UL

// TIMER 2 & 3 & 4 & 5
#define TIMER2_BASE                                 0x40000000UL
#define TIMER3_BASE                                 0x40000400UL
#define TIMER4_BASE                                 0x40000800UL
#define TIMER5_BASE                                 0x40000C00UL

//---------------------------------------
// Base Addresses for APB2 Peripherals
//---------------------------------------

// SYSCFG BASE
#define SYSCFG_BASE                                 0x40013800UL

// GPIO
// A,B fully included in LQFP48 package
#define GPIOA_BASE                                  0x40020000UL
#define GPIOB_BASE                                  0x40020400UL
// C partial included in LQFP48 package
#define GPIOC_BASE                                  0x40020800UL
// E,D,H not included in LQFP48 package
#define GPIOD_BASE                                  0x40020C00UL
#define GPIOE_BASE                                  0x40021000UL
#define GPIOH_BASE                                  0x40021C00UL

// EXTI
#define EXTI_BASE                                   0x40013C00UL

// USART 1 & 6
#define USART1_BASE                                 0x40011000UL
#define USART6_BASE                                 0x40011400UL

// SPI 1 & 4
#define SPI1_BASE                                   0x40013000UL
#define SPI4_BASE                                   0x40013400UL

// TIMER 1
#define TIMER1_BASE                                 0x40010000UL

// ADC 1
#define ADC1_BASE                                   0x40012000UL

//---------------------------------------
// Base Addresses for AHB2 Peripherals
//---------------------------------------
#define USB_BASE                                    0x50000000UL

//============================================================

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Register
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Register: SCB
// (CMSIS also provides SCB_Type / SCB; this one is kept for your drivers)
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef struct{
    volatile uint32_t CPUID  ;
    volatile uint32_t ICSR   ;
    volatile uint32_t VTOR   ;
    volatile uint32_t AIRCR  ;
    volatile uint32_t SCR    ;
    volatile uint32_t CCR    ;
    volatile uint32_t SHPR1  ;
    volatile uint32_t SHPR2  ;
    volatile uint32_t SHPR3  ;
    volatile uint32_t SHCSR  ;
    volatile uint32_t CFSR   ;
    volatile uint32_t HFSR   ;
    uint32_t RESERVED0       ;
    volatile uint32_t MMAR   ;
    volatile uint32_t BFAR   ;
    volatile uint32_t AFSR   ;
}SCB_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Register: FMI
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef struct{
    volatile uint32_t ACR     ;
    volatile uint32_t KEYR    ;
    volatile uint32_t OPTKEYR ;
    volatile uint32_t SR      ;
    volatile uint32_t CR      ;
    volatile uint32_t OPTCR   ;
}FMI_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Register: SYSCFG
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef struct{
    volatile uint32_t MEMRMP;
    volatile uint32_t PMC;
    volatile uint32_t EXTICR[4];
    uint32_t RESERVED[2];          /* 0x18 - 0x1C (was missing)     */
    volatile uint32_t CMPCR;       /* 0x20                          */
}SYSCFG_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Register: DMA
//-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef struct{
    volatile uint32_t CR;
    volatile uint32_t NDTR;
    volatile uint32_t PAR;
    volatile uint32_t M0AR;
    volatile uint32_t M1AR;
    volatile uint32_t FCR;
}DMA_Stream_t;

typedef struct{
    volatile uint32_t LISR;
    volatile uint32_t HISR;
    volatile uint32_t LIFCR;
    volatile uint32_t HIFCR;
    DMA_Stream_t      Stream[8];
}DMA_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Register: RCC
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef struct{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    uint32_t RESERVED0;
    uint32_t RESERVED1;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED2;
    uint32_t RESERVED3;
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    uint32_t RESERVED4;
    uint32_t RESERVED5;
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
    uint32_t RESERVED6;
    uint32_t RESERVED7;
    volatile uint32_t AHB1LPENR;
    volatile uint32_t AHB2LPENR;
    uint32_t RESERVED8;
    uint32_t RESERVED9;
    volatile uint32_t APB1LPENR;
    volatile uint32_t APB2LPENR;
    uint32_t RESERVED10;
    uint32_t RESERVED11;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    uint32_t RESERVED12;
    uint32_t RESERVED13;
    volatile uint32_t SSCGR;
    volatile uint32_t PLLI2SCFGR;
    uint32_t RESERVED14;
    volatile uint32_t DCKCFGR;
}RCC_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Register: GPIO
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef struct{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
}GPIO_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Register: EXTI
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef struct{
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
}EXTI_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Register: USART
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef struct{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
}USART_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Register: SPI
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef struct{
    volatile uint32_t CR1     ;
    volatile uint32_t CR2     ;
    volatile uint32_t SR      ;
    volatile uint32_t DR      ;
    volatile uint32_t CRCPR   ;
    volatile uint32_t RXCRCR  ;
    volatile uint32_t TXCRCR  ;
    volatile uint32_t I2SCFGR ;
    volatile uint32_t I2SPR   ;
}SPI_Typedef;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Register: I2C
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef struct{
    volatile uint32_t CR1   ;
    volatile uint32_t CR2   ;
    volatile uint32_t OAR1  ;
    volatile uint32_t OAR2  ;
    volatile uint32_t DR    ;
    volatile uint32_t SR1   ;
    volatile uint32_t SR2   ;
    volatile uint32_t CCR   ;
    volatile uint32_t TRISE ;
    volatile uint32_t FLTR  ;
}I2C_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Register: TIMERS
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef struct{
    volatile uint32_t CR1   ;
    volatile uint32_t CR2   ;
    volatile uint32_t SMCR  ;
    volatile uint32_t DIER  ;
    volatile uint32_t SR    ;
    volatile uint32_t EGR   ;
    volatile uint32_t CCMR1 ;
    volatile uint32_t CCMR2 ;
    volatile uint32_t CCER  ;
    volatile uint32_t CNT   ;
    volatile uint32_t PSC   ;
    volatile uint32_t ARR   ;
    volatile uint32_t RCR   ;
    volatile uint32_t CCR1  ;
    volatile uint32_t CCR2  ;
    volatile uint32_t CCR3  ;
    volatile uint32_t CCR4  ;
    volatile uint32_t BDTR  ;
    volatile uint32_t DCR   ;
    volatile uint32_t DMAR  ;
}TIMER_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Register: ADC
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef struct{
    volatile uint32_t SR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMPR1;
    volatile uint32_t SMPR2;
    volatile uint32_t JOFR1;
    volatile uint32_t JOFR2;
    volatile uint32_t JOFR3;
    volatile uint32_t JOFR4;
    volatile uint32_t HTR;
    volatile uint32_t LTR;
    volatile uint32_t SQR1;
    volatile uint32_t SQR2;
    volatile uint32_t SQR3;
    volatile uint32_t JSQR;
    volatile uint32_t JDR1;
    volatile uint32_t JDR2;
    volatile uint32_t JDR3;
    volatile uint32_t JDR4;
    volatile uint32_t DR;
}ADC_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Register: USB
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Endpoints Sub-Structs
typedef struct {
    volatile uint32_t DIEPCTL;
    uint32_t Reserved04;
    volatile uint32_t DIEPINT;
    uint32_t Reserved0C;
    volatile uint32_t DIEPTSIZ;
    volatile uint32_t DIEPDMA;
    volatile uint32_t DTXFSTS;
    uint32_t Reserved1C;
} USB_OTG_INEP_TypeDef;

typedef struct {
    volatile uint32_t DOEPCTL;
    uint32_t Reserved04;
    volatile uint32_t DOEPINT;
    uint32_t Reserved0C;
    volatile uint32_t DOEPTSIZ;
    volatile uint32_t DOEPDMA;
    uint32_t Reserved18[2];
} USB_OTG_OUTEP_TypeDef;

// The Master Unified USB OTG Struct
typedef struct {
    // --- GLOBAL REGISTERS (0x000) ---
    volatile uint32_t GOTGCTL;      // 0x000
    volatile uint32_t GOTGINT;      // 0x004
    volatile uint32_t GAHBCFG;      // 0x008
    volatile uint32_t GUSBCFG;      // 0x00C
    volatile uint32_t GRSTCTL;      // 0x010
    volatile uint32_t GINTSTS;      // 0x014
    volatile uint32_t GINTMSK;      // 0x018
    volatile uint32_t GRXSTSR;      // 0x01C
    volatile uint32_t GRXSTSP;      // 0x020
    volatile uint32_t GRXFSIZ;      // 0x024
    volatile uint32_t DIEPTXF0;     // 0x028
    volatile uint32_t HNPTXSTS;     // 0x02C

    uint32_t Reserved030[2];        // 0x030 - 0x034 (CRITICAL PADDING)

    volatile uint32_t GCCFG;        // 0x038
    volatile uint32_t CID;          // 0x03C

    uint32_t Reserved040[48];       // 0x040 - 0x0FF: Padding

    volatile uint32_t HPTXFSIZ;     // 0x100
    volatile uint32_t DIEPTXF[3];   // 0x104 - 0x10F

    uint32_t Reserved110[444];      // 0x110 - 0x7FF: Padding to bridge to Device Regs

    // --- DEVICE REGISTERS (0x800) ---
    volatile uint32_t DCFG;         // 0x800
    volatile uint32_t DCTL;         // 0x804
    volatile uint32_t DSTS;         // 0x808
    uint32_t Reserved80C;           // 0x80C
    volatile uint32_t DIEPMSK;      // 0x810
    volatile uint32_t DOEPMSK;      // 0x814
    volatile uint32_t DAINT;        // 0x818
    volatile uint32_t DAINTMSK;     // 0x81C
    uint32_t Reserved820[2];        // 0x820 - 0x824
    volatile uint32_t DVBUSDIS;     // 0x828
    volatile uint32_t DVBUSPULSE;   // 0x82C
    volatile uint32_t DTHRCTL;      // 0x830
    volatile uint32_t DIEPEMPMSK;   // 0x834

    uint32_t Reserved838[50];       // 0x838 - 0x8FF: Padding

    // --- ENDPOINT REGISTERS (0x900 & 0xB00) ---
    USB_OTG_INEP_TypeDef IN_EP[4];  // 0x900 - 0x97F
    uint32_t Reserved980[96];       // 0x980 - 0xAFF
    USB_OTG_OUTEP_TypeDef OUT_EP[4];// 0xB00 - 0xB7F
    uint32_t ReservedB80[160];      // 0xB80 - 0xDFF

    volatile uint32_t PCGCCTL;      // 0xE00
    uint32_t ReservedE04[127];      // 0xE04 - 0xFFF

    // --- DATA FIFOS (0x1000) ---
    volatile uint32_t DFIFO[4][1024];

} USB_TypeDef;

//============================================================

//-*-*-*-*-*-*-*-*-*-*-*-
// Peripheral Instants:
//-*-*-*-*-*-*-*-*-*-*-*-
/* SysTick, SCB and NVIC instances come from core_cm4.h */

#define FMI                     ((FMI_TypeDef *)FMI_BASE)

#define SYSCFG                  ((SYSCFG_TypeDef *)SYSCFG_BASE)

#define DMA1                    ((DMA_TypeDef *)DMA1_BASE)
#define DMA2                    ((DMA_TypeDef *)DMA2_BASE)

#define RCC                     ((RCC_TypeDef *)RCC_BASE)

#define GPIOA                   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB                   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC                   ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD                   ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE                   ((GPIO_TypeDef *)GPIOE_BASE)
#define GPIOH                   ((GPIO_TypeDef *)GPIOH_BASE)

#define EXTI                    ((EXTI_TypeDef *)EXTI_BASE)

#define USART1                  ((USART_TypeDef *)USART1_BASE)
#define USART2                  ((USART_TypeDef *)USART2_BASE)
#define USART6                  ((USART_TypeDef *)USART6_BASE)

#define SPI1                    ((SPI_Typedef *)SPI1_BASE)
#define SPI2                    ((SPI_Typedef *)SPI2_BASE)
#define SPI3                    ((SPI_Typedef *)SPI3_BASE)
#define SPI4                    ((SPI_Typedef *)SPI4_BASE)

#define I2C1                    ((I2C_TypeDef *)I2C1_BASE)
#define I2C2                    ((I2C_TypeDef *)I2C2_BASE)
#define I2C3                    ((I2C_TypeDef *)I2C3_BASE)

#define TIMER1                  ((TIMER_TypeDef *)TIMER1_BASE)
#define TIMER2                  ((TIMER_TypeDef *)TIMER2_BASE)
#define TIMER3                  ((TIMER_TypeDef *)TIMER3_BASE)
#define TIMER4                  ((TIMER_TypeDef *)TIMER4_BASE)
#define TIMER5                  ((TIMER_TypeDef *)TIMER5_BASE)

#define ADC1                    ((ADC_TypeDef *)ADC1_BASE)

#define USB_OTG_FS              ((USB_TypeDef *) USB_BASE)

//-*-*-*-*-*-*-*-*-*-*-*-
// Clock Enable Macros:
//-*-*-*-*-*-*-*-*-*-*-*-

// Clock Enable
#define RCC_SYSCFG_CLK_EN()             (RCC->APB2ENR |= (1UL << 14))

#define RCC_DMA1_CLK_EN()               (RCC->AHB1ENR |= (1UL << 21))
#define RCC_DMA2_CLK_EN()               (RCC->AHB1ENR |= (1UL << 22))

#define RCC_GPIOA_CLK_EN()              (RCC->AHB1ENR |= (1UL << 0))
#define RCC_GPIOB_CLK_EN()              (RCC->AHB1ENR |= (1UL << 1))
#define RCC_GPIOC_CLK_EN()              (RCC->AHB1ENR |= (1UL << 2))
#define RCC_GPIOD_CLK_EN()              (RCC->AHB1ENR |= (1UL << 3))
#define RCC_GPIOE_CLK_EN()              (RCC->AHB1ENR |= (1UL << 4))
#define RCC_GPIOH_CLK_EN()              (RCC->AHB1ENR |= (1UL << 7))

#define RCC_USART1_CLK_EN()             (RCC->APB2ENR |= (1UL << 4))
#define RCC_USART2_CLK_EN()             (RCC->APB1ENR |= (1UL << 17))
#define RCC_USART6_CLK_EN()             (RCC->APB2ENR |= (1UL << 5))

#define RCC_SPI1_CLK_EN()               (RCC->APB2ENR |= (1UL << 12))
#define RCC_SPI2_CLK_EN()               (RCC->APB1ENR |= (1UL << 14))
#define RCC_SPI3_CLK_EN()               (RCC->APB1ENR |= (1UL << 15))
#define RCC_SPI4_CLK_EN()               (RCC->APB2ENR |= (1UL << 13))

#define RCC_I2C1_CLK_EN()               (RCC->APB1ENR |= (1UL << 21))
#define RCC_I2C2_CLK_EN()               (RCC->APB1ENR |= (1UL << 22))
#define RCC_I2C3_CLK_EN()               (RCC->APB1ENR |= (1UL << 23))

#define RCC_TIMER1_CLK_EN()             (RCC->APB2ENR |= (1UL << 0))
#define RCC_TIMER2_CLK_EN()             (RCC->APB1ENR |= (1UL << 0))
#define RCC_TIMER3_CLK_EN()             (RCC->APB1ENR |= (1UL << 1))
#define RCC_TIMER4_CLK_EN()             (RCC->APB1ENR |= (1UL << 2))
#define RCC_TIMER5_CLK_EN()             (RCC->APB1ENR |= (1UL << 3))

#define RCC_ADC1_CLK_EN()               (RCC->APB2ENR |= (1UL << 8))

#define RCC_USB_CLK_EN()                (RCC->AHB2ENR |= (1UL << 7))

// Reset
// Each *_RST_SET() now PULSES the reset bit (set, then clear). Previously the
// bit was only set, which left the peripheral held in reset forever.
#define RCC_RST_PULSE(REG, BIT)         ((REG) |= (1UL << (BIT)), (REG) &= ~(1UL << (BIT)))

#define RCC_SYSCFG_CLK_RST_SET()        RCC_RST_PULSE(RCC->APB2RSTR, 14)

#define RCC_DMA1_CLK_RST_SET()          RCC_RST_PULSE(RCC->AHB1RSTR, 21)
#define RCC_DMA2_CLK_RST_SET()          RCC_RST_PULSE(RCC->AHB1RSTR, 22)

#define RCC_GPIOA_CLK_RST_SET()         RCC_RST_PULSE(RCC->AHB1RSTR, 0)
#define RCC_GPIOB_CLK_RST_SET()         RCC_RST_PULSE(RCC->AHB1RSTR, 1)
#define RCC_GPIOC_CLK_RST_SET()         RCC_RST_PULSE(RCC->AHB1RSTR, 2)
#define RCC_GPIOD_CLK_RST_SET()         RCC_RST_PULSE(RCC->AHB1RSTR, 3)
#define RCC_GPIOE_CLK_RST_SET()         RCC_RST_PULSE(RCC->AHB1RSTR, 4)
#define RCC_GPIOH_CLK_RST_SET()         RCC_RST_PULSE(RCC->AHB1RSTR, 7)   /* was bit 4 (GPIOE) */

#define RCC_USART1_CLK_RST_SET()        RCC_RST_PULSE(RCC->APB2RSTR, 4)
#define RCC_USART2_CLK_RST_SET()        RCC_RST_PULSE(RCC->APB1RSTR, 17)
#define RCC_USART6_CLK_RST_SET()        RCC_RST_PULSE(RCC->APB2RSTR, 5)

#define RCC_SPI1_CLK_RST_SET()          RCC_RST_PULSE(RCC->APB2RSTR, 12)
#define RCC_SPI2_CLK_RST_SET()          RCC_RST_PULSE(RCC->APB1RSTR, 14)
#define RCC_SPI3_CLK_RST_SET()          RCC_RST_PULSE(RCC->APB1RSTR, 15)
#define RCC_SPI4_CLK_RST_SET()          RCC_RST_PULSE(RCC->APB2RSTR, 13)

#define RCC_I2C1_CLK_RST_SET()          RCC_RST_PULSE(RCC->APB1RSTR, 21)
#define RCC_I2C2_CLK_RST_SET()          RCC_RST_PULSE(RCC->APB1RSTR, 22)
#define RCC_I2C3_CLK_RST_SET()          RCC_RST_PULSE(RCC->APB1RSTR, 23)

#define RCC_TIMER1_CLK_RST_SET()        RCC_RST_PULSE(RCC->APB2RSTR, 0)
#define RCC_TIMER2_CLK_RST_SET()        RCC_RST_PULSE(RCC->APB1RSTR, 0)
#define RCC_TIMER3_CLK_RST_SET()        RCC_RST_PULSE(RCC->APB1RSTR, 1)
#define RCC_TIMER4_CLK_RST_SET()        RCC_RST_PULSE(RCC->APB1RSTR, 2)
#define RCC_TIMER5_CLK_RST_SET()        RCC_RST_PULSE(RCC->APB1RSTR, 3)

#define RCC_ADC1_CLK_RST_SET()          RCC_RST_PULSE(RCC->APB2RSTR, 8)

#define RCC_USB_CLK_RST_SET()           RCC_RST_PULSE(RCC->AHB2RSTR, 7)

//-*-*-*-*-
// IVT
//-*-*-*-*-

// DMA
#define DMA1_Stream0_IRQ        11
#define DMA1_Stream1_IRQ        12
#define DMA1_Stream2_IRQ        13
#define DMA1_Stream3_IRQ        14
#define DMA1_Stream4_IRQ        15
#define DMA1_Stream5_IRQ        16
#define DMA1_Stream6_IRQ        17
#define DMA1_Stream7_IRQ        47

#define DMA2_Stream0_IRQ        56
#define DMA2_Stream1_IRQ        57
#define DMA2_Stream2_IRQ        58
#define DMA2_Stream3_IRQ        59
#define DMA2_Stream4_IRQ        60
#define DMA2_Stream5_IRQ        68
#define DMA2_Stream6_IRQ        69
#define DMA2_Stream7_IRQ        70

// EXTI
#define EXTI0_IRQ               6
#define EXTI1_IRQ               7
#define EXTI2_IRQ               8
#define EXTI3_IRQ               9
#define EXTI4_IRQ               10
#define EXTI5_IRQ               23
#define EXTI6_IRQ               23
#define EXTI7_IRQ               23
#define EXTI8_IRQ               23
#define EXTI9_IRQ               23
#define EXTI10_IRQ              40
#define EXTI11_IRQ              40
#define EXTI12_IRQ              40
#define EXTI13_IRQ              40
#define EXTI14_IRQ              40
#define EXTI15_IRQ              40

// USART
#define USART1_IRQ              37
#define USART2_IRQ              38
#define USART6_IRQ              71

// SPI
#define SPI1_IRQ                35
#define SPI2_IRQ                36
#define SPI3_IRQ                51
#define SPI4_IRQ                84

// I2C
#define I2C1_EV                 31
#define I2C1_ER                 32

#define I2C2_EV                 33
#define I2C2_ER                 34

#define I2C3_EV                 72
#define I2C3_ER                 73

// TIMER
#define TIMER1_BRK_IRQ          24
#define TIMER1_UP_IRQ           25
#define TIMER1_TRIG_COM_IRQ     26
#define TIMER1_CC_IRQ           27

#define TIMER2_IRQ              28
#define TIMER3_IRQ              29
#define TIMER4_IRQ              30
#define TIMER5_IRQ              50

// ADC
#define ADC1_IRQ                18

// USB OTG
#define USB_WKUP_IRQ            42
#define USB_OTG_FS_IRQ          67

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//NVIC IRQ Enable/Disable macros
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
// Enable
#define NVIC_IRQ_DMA1_S0_EN()               (NVIC_ISER0_BASE |= (1UL << 11))
#define NVIC_IRQ_DMA1_S1_EN()               (NVIC_ISER0_BASE |= (1UL << 12))
#define NVIC_IRQ_DMA1_S2_EN()               (NVIC_ISER0_BASE |= (1UL << 13))
#define NVIC_IRQ_DMA1_S3_EN()               (NVIC_ISER0_BASE |= (1UL << 14))
#define NVIC_IRQ_DMA1_S4_EN()               (NVIC_ISER0_BASE |= (1UL << 15))
#define NVIC_IRQ_DMA1_S5_EN()               (NVIC_ISER0_BASE |= (1UL << 16))
#define NVIC_IRQ_DMA1_S6_EN()               (NVIC_ISER0_BASE |= (1UL << 17))
#define NVIC_IRQ_DMA1_S7_EN()               (NVIC_ISER1_BASE |= (1UL << (DMA1_Stream7_IRQ - 32)))

#define NVIC_IRQ_DMA2_S0_EN()               (NVIC_ISER1_BASE |= (1UL << (DMA2_Stream0_IRQ - 32)))
#define NVIC_IRQ_DMA2_S1_EN()               (NVIC_ISER1_BASE |= (1UL << (DMA2_Stream1_IRQ - 32)))
#define NVIC_IRQ_DMA2_S2_EN()               (NVIC_ISER1_BASE |= (1UL << (DMA2_Stream2_IRQ - 32)))
#define NVIC_IRQ_DMA2_S3_EN()               (NVIC_ISER1_BASE |= (1UL << (DMA2_Stream3_IRQ - 32)))
#define NVIC_IRQ_DMA2_S4_EN()               (NVIC_ISER1_BASE |= (1UL << (DMA2_Stream4_IRQ - 32)))
#define NVIC_IRQ_DMA2_S5_EN()               (NVIC_ISER2_BASE |= (1UL << (DMA2_Stream5_IRQ - 64)))
#define NVIC_IRQ_DMA2_S6_EN()               (NVIC_ISER2_BASE |= (1UL << (DMA2_Stream6_IRQ - 64)))
#define NVIC_IRQ_DMA2_S7_EN()               (NVIC_ISER2_BASE |= (1UL << (DMA2_Stream7_IRQ - 64)))   /* was DMA1_Stream7_IRQ */

#define NVIC_IRQ6_EXTI0_EN()                (NVIC_ISER0_BASE |= (1UL << 6))
#define NVIC_IRQ7_EXTI1_EN()                (NVIC_ISER0_BASE |= (1UL << 7))
#define NVIC_IRQ8_EXTI2_EN()                (NVIC_ISER0_BASE |= (1UL << 8))
#define NVIC_IRQ9_EXTI3_EN()                (NVIC_ISER0_BASE |= (1UL << 9))
#define NVIC_IRQ10_EXTI4_EN()               (NVIC_ISER0_BASE |= (1UL << 10))
#define NVIC_IRQ23_EXTI_5_9_EN()            (NVIC_ISER0_BASE |= (1UL << 23))
#define NVIC_IRQ40_EXTI_10_15_EN()          (NVIC_ISER1_BASE |= (1UL << (EXTI10_IRQ - 32)))

#define NVIC_IRQ_USART1_EN()                (NVIC_ISER1_BASE |= (1UL << (USART1_IRQ - 32)))
#define NVIC_IRQ_USART2_EN()                (NVIC_ISER1_BASE |= (1UL << (USART2_IRQ - 32)))
#define NVIC_IRQ_USART6_EN()                (NVIC_ISER2_BASE |= (1UL << (USART6_IRQ - 64)))

#define NVIC_IRQ_SPI1_EN()                  (NVIC_ISER1_BASE |= (1UL << (SPI1_IRQ - 32)))
#define NVIC_IRQ_SPI2_EN()                  (NVIC_ISER1_BASE |= (1UL << (SPI2_IRQ - 32)))
#define NVIC_IRQ_SPI3_EN()                  (NVIC_ISER1_BASE |= (1UL << (SPI3_IRQ - 32)))
#define NVIC_IRQ_SPI4_EN()                  (NVIC_ISER2_BASE |= (1UL << (SPI4_IRQ - 64)))

#define NVIC_IRQ_I2C1_EV_EN()               (NVIC_ISER0_BASE |= (1UL << I2C1_EV))
#define NVIC_IRQ_I2C1_ER_EN()               (NVIC_ISER1_BASE |= (1UL << (I2C1_ER - 32)))   /* was ISER0 (hit WWDG) */

#define NVIC_IRQ_I2C2_EV_EN()               (NVIC_ISER1_BASE |= (1UL << (I2C2_EV - 32)))
#define NVIC_IRQ_I2C2_ER_EN()               (NVIC_ISER1_BASE |= (1UL << (I2C2_ER - 32)))

#define NVIC_IRQ_I2C3_EV_EN()               (NVIC_ISER2_BASE |= (1UL << (I2C3_EV - 64)))
#define NVIC_IRQ_I2C3_ER_EN()               (NVIC_ISER2_BASE |= (1UL << (I2C3_ER - 64)))

#define NVIC_IRQ_TIMER1_BRK_EN()            (NVIC_ISER0_BASE |= (1UL << 24))
#define NVIC_IRQ_TIMER1_UP_EN()             (NVIC_ISER0_BASE |= (1UL << 25))
#define NVIC_IRQ_TIMER1_TRIG_COM_EN()       (NVIC_ISER0_BASE |= (1UL << 26))
#define NVIC_IRQ_TIMER1_CC_EN()             (NVIC_ISER0_BASE |= (1UL << 27))
#define NVIC_IRQ_TIMER2_EN()                (NVIC_ISER0_BASE |= (1UL << 28))
#define NVIC_IRQ_TIMER3_EN()                (NVIC_ISER0_BASE |= (1UL << 29))
#define NVIC_IRQ_TIMER4_EN()                (NVIC_ISER0_BASE |= (1UL << 30))
#define NVIC_IRQ_TIMER5_EN()                (NVIC_ISER1_BASE |= (1UL << (TIMER5_IRQ - 32)))

#define NVIC_IRQ_ADC1_EN()                  (NVIC_ISER0_BASE |= (1UL << ADC1_IRQ))

#define NVIC_IRQ_OTG_WKUP_EN()              (NVIC_ISER1_BASE |= (1UL << (USB_WKUP_IRQ - 32)))
#define NVIC_IRQ_OTG_FS_EN()                (NVIC_ISER2_BASE |= (1UL << (USB_OTG_FS_IRQ - 64)))

// Disable
#define NVIC_IRQ_DMA1_S0_DIS()              (NVIC_ICER0_BASE |= (1UL << 11))
#define NVIC_IRQ_DMA1_S1_DIS()              (NVIC_ICER0_BASE |= (1UL << 12))
#define NVIC_IRQ_DMA1_S2_DIS()              (NVIC_ICER0_BASE |= (1UL << 13))
#define NVIC_IRQ_DMA1_S3_DIS()              (NVIC_ICER0_BASE |= (1UL << 14))
#define NVIC_IRQ_DMA1_S4_DIS()              (NVIC_ICER0_BASE |= (1UL << 15))
#define NVIC_IRQ_DMA1_S5_DIS()              (NVIC_ICER0_BASE |= (1UL << 16))
#define NVIC_IRQ_DMA1_S6_DIS()              (NVIC_ICER0_BASE |= (1UL << 17))
#define NVIC_IRQ_DMA1_S7_DIS()              (NVIC_ICER1_BASE |= (1UL << (DMA1_Stream7_IRQ - 32)))

#define NVIC_IRQ_DMA2_S0_DIS()              (NVIC_ICER1_BASE |= (1UL << (DMA2_Stream0_IRQ - 32)))
#define NVIC_IRQ_DMA2_S1_DIS()              (NVIC_ICER1_BASE |= (1UL << (DMA2_Stream1_IRQ - 32)))
#define NVIC_IRQ_DMA2_S2_DIS()              (NVIC_ICER1_BASE |= (1UL << (DMA2_Stream2_IRQ - 32)))
#define NVIC_IRQ_DMA2_S3_DIS()              (NVIC_ICER1_BASE |= (1UL << (DMA2_Stream3_IRQ - 32)))
#define NVIC_IRQ_DMA2_S4_DIS()              (NVIC_ICER1_BASE |= (1UL << (DMA2_Stream4_IRQ - 32)))
#define NVIC_IRQ_DMA2_S5_DIS()              (NVIC_ICER2_BASE |= (1UL << (DMA2_Stream5_IRQ - 64)))
#define NVIC_IRQ_DMA2_S6_DIS()              (NVIC_ICER2_BASE |= (1UL << (DMA2_Stream6_IRQ - 64)))
#define NVIC_IRQ_DMA2_S7_DIS()              (NVIC_ICER2_BASE |= (1UL << (DMA2_Stream7_IRQ - 64)))   /* was DMA1_Stream7_IRQ */

#define NVIC_IRQ6_EXTI0_DIS()               (NVIC_ICER0_BASE |= (1UL << 6))
#define NVIC_IRQ7_EXTI1_DIS()               (NVIC_ICER0_BASE |= (1UL << 7))
#define NVIC_IRQ8_EXTI2_DIS()               (NVIC_ICER0_BASE |= (1UL << 8))
#define NVIC_IRQ9_EXTI3_DIS()               (NVIC_ICER0_BASE |= (1UL << 9))
#define NVIC_IRQ10_EXTI4_DIS()              (NVIC_ICER0_BASE |= (1UL << 10))
#define NVIC_IRQ23_EXTI_5_9_DIS()           (NVIC_ICER0_BASE |= (1UL << 23))
#define NVIC_IRQ40_EXTI_10_15_DIS()         (NVIC_ICER1_BASE |= (1UL << (EXTI10_IRQ - 32)))

#define NVIC_IRQ_USART1_DIS()               (NVIC_ICER1_BASE |= (1UL << (USART1_IRQ - 32)))
#define NVIC_IRQ_USART2_DIS()               (NVIC_ICER1_BASE |= (1UL << (USART2_IRQ - 32)))
#define NVIC_IRQ_USART6_DIS()               (NVIC_ICER2_BASE |= (1UL << (USART6_IRQ - 64)))

#define NVIC_IRQ_SPI1_DIS()                 (NVIC_ICER1_BASE |= (1UL << (SPI1_IRQ - 32)))
#define NVIC_IRQ_SPI2_DIS()                 (NVIC_ICER1_BASE |= (1UL << (SPI2_IRQ - 32)))
#define NVIC_IRQ_SPI3_DIS()                 (NVIC_ICER1_BASE |= (1UL << (SPI3_IRQ - 32)))
#define NVIC_IRQ_SPI4_DIS()                 (NVIC_ICER2_BASE |= (1UL << (SPI4_IRQ - 64)))   /* was SPI1_IRQ */

#define NVIC_IRQ_I2C1_EV_DIS()              (NVIC_ICER0_BASE |= (1UL << I2C1_EV))
#define NVIC_IRQ_I2C1_ER_DIS()              (NVIC_ICER1_BASE |= (1UL << (I2C1_ER - 32)))   /* was ICER0 (hit WWDG) */

#define NVIC_IRQ_I2C2_EV_DIS()              (NVIC_ICER1_BASE |= (1UL << (I2C2_EV - 32)))
#define NVIC_IRQ_I2C2_ER_DIS()              (NVIC_ICER1_BASE |= (1UL << (I2C2_ER - 32)))

#define NVIC_IRQ_I2C3_EV_DIS()              (NVIC_ICER2_BASE |= (1UL << (I2C3_EV - 64)))
#define NVIC_IRQ_I2C3_ER_DIS()              (NVIC_ICER2_BASE |= (1UL << (I2C3_ER - 64)))

#define NVIC_IRQ_TIMER1_BRK_DIS()           (NVIC_ICER0_BASE |= (1UL << 24))
#define NVIC_IRQ_TIMER1_UP_DIS()            (NVIC_ICER0_BASE |= (1UL << 25))
#define NVIC_IRQ_TIMER1_TRIG_COM_DIS()      (NVIC_ICER0_BASE |= (1UL << 26))
#define NVIC_IRQ_TIMER1_CC_DIS()            (NVIC_ICER0_BASE |= (1UL << 27))
#define NVIC_IRQ_TIMER2_DIS()               (NVIC_ICER0_BASE |= (1UL << 28))
#define NVIC_IRQ_TIMER3_DIS()               (NVIC_ICER0_BASE |= (1UL << 29))
#define NVIC_IRQ_TIMER4_DIS()               (NVIC_ICER0_BASE |= (1UL << 30))
#define NVIC_IRQ_TIMER5_DIS()               (NVIC_ICER1_BASE |= (1UL << (TIMER5_IRQ - 32)))

#define NVIC_IRQ_ADC1_DIS()                 (NVIC_ICER0_BASE |= (1UL << ADC1_IRQ))

#define NVIC_IRQ_OTG_WKUP_DIS()             (NVIC_ICER1_BASE |= (1UL << (USB_WKUP_IRQ - 32)))
#define NVIC_IRQ_OTG_FS_DIS()               (NVIC_ICER2_BASE |= (1UL << (USB_OTG_FS_IRQ - 64)))

//-*-*-*-*-*-*-*-*-*-*-*-
// Generic Macros:
//-*-*-*-*-*-*-*-*-*-*-*-
//#define ENABLE                        1
//#define DISABLE                       0

#define HIGH                        1
#define LOW                         0

#define GET_BIT(VAL, BIT)           (((VAL) >> (BIT)) & 1U)

/********************************************************/
/*******************  Bit definition  ********************/
/********************************************************/

/*******************  Bit definition for I2C_CR1 register  ********************/
#define I2C_CR1_PE_Pos                      (0U)
#define I2C_CR1_PE_Msk                      (0x1UL << I2C_CR1_PE_Pos)           /* 0x00000001 */
#define I2C_CR1_PE                          I2C_CR1_PE_Msk                      /* Peripheral Enable */

#define I2C_CR1_SMBUS_Pos                   (1U)
#define I2C_CR1_SMBUS_Msk                   (0x1UL << I2C_CR1_SMBUS_Pos)        /* 0x00000002 */
#define I2C_CR1_SMBUS                       I2C_CR1_SMBUS_Msk                   /* SMBus Mode */

#define I2C_CR1_SMBTYPE_Pos                 (3U)
#define I2C_CR1_SMBTYPE_Msk                 (0x1UL << I2C_CR1_SMBTYPE_Pos)      /* 0x00000008 */
#define I2C_CR1_SMBTYPE                     I2C_CR1_SMBTYPE_Msk                 /* SMBus Type */

#define I2C_CR1_ENARP_Pos                   (4U)
#define I2C_CR1_ENARP_Msk                   (0x1UL << I2C_CR1_ENARP_Pos)        /* 0x00000010 */
#define I2C_CR1_ENARP                       I2C_CR1_ENARP_Msk                   /* ARP Enable */

#define I2C_CR1_ENPEC_Pos                   (5U)
#define I2C_CR1_ENPEC_Msk                   (0x1UL << I2C_CR1_ENPEC_Pos)        /* 0x00000020 */
#define I2C_CR1_ENPEC                       I2C_CR1_ENPEC_Msk                   /* PEC Enable */

#define I2C_CR1_ENGC_Pos                    (6U)
#define I2C_CR1_ENGC_Msk                    (0x1UL << I2C_CR1_ENGC_Pos)         /* 0x00000040 */
#define I2C_CR1_ENGC                        I2C_CR1_ENGC_Msk                    /* General Call Enable */

#define I2C_CR1_NOSTRETCH_Pos               (7U)
#define I2C_CR1_NOSTRETCH_Msk               (0x1UL << I2C_CR1_NOSTRETCH_Pos)    /* 0x00000080 */
#define I2C_CR1_NOSTRETCH                   I2C_CR1_NOSTRETCH_Msk               /* Clock Stretching Disable (Slave mode) */

#define I2C_CR1_START_Pos                   (8U)
#define I2C_CR1_START_Msk                   (0x1UL << I2C_CR1_START_Pos)        /* 0x00000100 */
#define I2C_CR1_START                       I2C_CR1_START_Msk                   /* Start Generation */
#define I2C_CR1_STOP_Pos                    (9U)
#define I2C_CR1_STOP_Msk                    (0x1UL << I2C_CR1_STOP_Pos)         /* 0x00000200 */
#define I2C_CR1_STOP                        I2C_CR1_STOP_Msk                    /* Stop Generation */

#define I2C_CR1_ACK_Pos                     (10U)
#define I2C_CR1_ACK_Msk                     (0x1UL << I2C_CR1_ACK_Pos)          /* 0x00000400 */
#define I2C_CR1_ACK                         I2C_CR1_ACK_Msk                     /* Acknowledge Enable */

#define I2C_CR1_POS_Pos                     (11U)
#define I2C_CR1_POS_Msk                     (0x1UL << I2C_CR1_POS_Pos)          /* 0x00000800 */
#define I2C_CR1_POS                         I2C_CR1_POS_Msk                     /* Acknowledge/PEC Position (for data reception) */

#define I2C_CR1_PEC_Pos                     (12U)
#define I2C_CR1_PEC_Msk                     (0x1UL << I2C_CR1_PEC_Pos)          /* 0x00001000 */
#define I2C_CR1_PEC                         I2C_CR1_PEC_Msk                     /* Packet Error Checking */

#define I2C_CR1_ALERT_Pos                   (13U)
#define I2C_CR1_ALERT_Msk                   (0x1UL << I2C_CR1_ALERT_Pos)        /* 0x00002000 */
#define I2C_CR1_ALERT                       I2C_CR1_ALERT_Msk                   /* SMBus Alert */

#define I2C_CR1_SWRST_Pos                   (15U)
#define I2C_CR1_SWRST_Msk                   (0x1UL << I2C_CR1_SWRST_Pos)        /* 0x00008000 */
#define I2C_CR1_SWRST                       I2C_CR1_SWRST_Msk                   /* Software Reset */

/*******************  Bit definition for I2C_CR2 register  ********************/
#define I2C_CR2_FREQ_Pos                    (0U)
#define I2C_CR2_FREQ_Msk                    (0x3FUL << I2C_CR2_FREQ_Pos)        /* 0x0000003F */
#define I2C_CR2_FREQ                        I2C_CR2_FREQ_Msk                    /* FREQ[5:0] bits (Peripheral Clock Frequency) */

#define I2C_CR2_ITERREN_Pos                 (8U)
#define I2C_CR2_ITERREN_Msk                 (0x1UL << I2C_CR2_ITERREN_Pos)      /* 0x00000100 */
#define I2C_CR2_ITERREN                     I2C_CR2_ITERREN_Msk                 /* Error Interrupt Enable */

#define I2C_CR2_ITEVTEN_Pos                 (9U)
#define I2C_CR2_ITEVTEN_Msk                 (0x1UL << I2C_CR2_ITEVTEN_Pos)      /* 0x00000200 */
#define I2C_CR2_ITEVTEN                     I2C_CR2_ITEVTEN_Msk                 /* Event Interrupt Enable */

#define I2C_CR2_ITBUFEN_Pos                 (10U)
#define I2C_CR2_ITBUFEN_Msk                 (0x1UL << I2C_CR2_ITBUFEN_Pos)      /* 0x00000400 */
#define I2C_CR2_ITBUFEN                     I2C_CR2_ITBUFEN_Msk                 /* Buffer Interrupt Enable */

#define I2C_CR2_DMAEN_Pos                   (11U)
#define I2C_CR2_DMAEN_Msk                   (0x1UL << I2C_CR2_DMAEN_Pos)        /* 0x00000800 */
#define I2C_CR2_DMAEN                       I2C_CR2_DMAEN_Msk                   /* DMA Requests Enable */
#define I2C_CR2_LAST_Pos                    (12U)
#define I2C_CR2_LAST_Msk                    (0x1UL << I2C_CR2_LAST_Pos)         /* 0x00001000 */
#define I2C_CR2_LAST                        I2C_CR2_LAST_Msk                    /* DMA Last Transfer */

/*******************  Bit definition for I2C_OAR2 register  *******************/
#define I2C_OAR2_ENDUAL_Pos                 (0U)
#define I2C_OAR2_ENDUAL_Msk                 (0x1UL << I2C_OAR2_ENDUAL_Pos)      /* 0x00000001 */
#define I2C_OAR2_ENDUAL                     I2C_OAR2_ENDUAL_Msk                 /* Dual addressing mode enable */

#define I2C_OAR2_ADD2_Pos                   (1U)

/*******************  Bit definition for I2C_SR1 register  ********************/
#define I2C_SR1_SB_Pos                      (0U)
#define I2C_SR1_SB_Msk                      (0x1UL << I2C_SR1_SB_Pos)           /* 0x00000001 */
#define I2C_SR1_SB                          I2C_SR1_SB_Msk                      /* Start Bit (Master mode) */

#define I2C_SR1_ADDR_Pos                    (1U)
#define I2C_SR1_ADDR_Msk                    (0x1UL << I2C_SR1_ADDR_Pos)         /* 0x00000002 */
#define I2C_SR1_ADDR                        I2C_SR1_ADDR_Msk                    /* Address sent (master mode)/matched (slave mode) */

#define I2C_SR1_BTF_Pos                     (2U)
#define I2C_SR1_BTF_Msk                     (0x1UL << I2C_SR1_BTF_Pos)          /* 0x00000004 */
#define I2C_SR1_BTF                         I2C_SR1_BTF_Msk                     /* Byte Transfer Finished */

#define I2C_SR1_ADD10_Pos                   (3U)
#define I2C_SR1_ADD10_Msk                   (0x1UL << I2C_SR1_ADD10_Pos)        /* 0x00000008 */
#define I2C_SR1_ADD10                       I2C_SR1_ADD10_Msk                   /* 10-bit header sent (Master mode) */

#define I2C_SR1_STOPF_Pos                   (4U)
#define I2C_SR1_STOPF_Msk                   (0x1UL << I2C_SR1_STOPF_Pos)        /* 0x00000010 */
#define I2C_SR1_STOPF                       I2C_SR1_STOPF_Msk                   /* Stop detection (Slave mode) */

#define I2C_SR1_RXNE_Pos                    (6U)
#define I2C_SR1_RXNE_Msk                    (0x1UL << I2C_SR1_RXNE_Pos)         /* 0x00000040 */
#define I2C_SR1_RXNE                        I2C_SR1_RXNE_Msk                    /* Data Register not Empty (receivers) */

#define I2C_SR1_TXE_Pos                     (7U)
#define I2C_SR1_TXE_Msk                     (0x1UL << I2C_SR1_TXE_Pos)          /* 0x00000080 */
#define I2C_SR1_TXE                         I2C_SR1_TXE_Msk                     /* Data Register Empty (transmitters) */

#define I2C_SR1_BERR_Pos                    (8U)
#define I2C_SR1_BERR_Msk                    (0x1UL << I2C_SR1_BERR_Pos)         /* 0x00000100 */
#define I2C_SR1_BERR                        I2C_SR1_BERR_Msk                    /* Bus Error */

#define I2C_SR1_ARLO_Pos                    (9U)
#define I2C_SR1_ARLO_Msk                    (0x1UL << I2C_SR1_ARLO_Pos)         /* 0x00000200 */
#define I2C_SR1_ARLO                        I2C_SR1_ARLO_Msk                    /* Arbitration Lost (master mode) */

#define I2C_SR1_AF_Pos                      (10U)
#define I2C_SR1_AF_Msk                      (0x1UL << I2C_SR1_AF_Pos)           /* 0x00000400 */
#define I2C_SR1_AF                          I2C_SR1_AF_Msk                      /* Acknowledge Failure */

#define I2C_SR1_OVR_Pos                     (11U)
#define I2C_SR1_OVR_Msk                     (0x1UL << I2C_SR1_OVR_Pos)          /* 0x00000800 */
#define I2C_SR1_OVR                         I2C_SR1_OVR_Msk                     /* Overrun/Underrun */

#define I2C_SR1_PECERR_Pos                  (12U)
#define I2C_SR1_PECERR_Msk                  (0x1UL << I2C_SR1_PECERR_Pos)       /* 0x00001000 */
#define I2C_SR1_PECERR                      I2C_SR1_PECERR_Msk                  /* PEC Error in reception */

#define I2C_SR1_TIMEOUT_Pos                 (14U)
#define I2C_SR1_TIMEOUT_Msk                 (0x1UL << I2C_SR1_TIMEOUT_Pos)      /* 0x00004000 */
#define I2C_SR1_TIMEOUT                     I2C_SR1_TIMEOUT_Msk                 /* Timeout or Tlow Error */

#define I2C_SR1_SMBALERT_Pos                (15U)
#define I2C_SR1_SMBALERT_Msk                (0x1UL << I2C_SR1_SMBALERT_Pos)     /* 0x00008000 */
#define I2C_SR1_SMBALERT                    I2C_SR1_SMBALERT_Msk                /* SMBus Alert */

/*******************  Bit definition for I2C_SR2 register  ********************/
#define I2C_SR2_MSL_Pos                     (0U)
#define I2C_SR2_MSL_Msk                     (0x1UL << I2C_SR2_MSL_Pos)          /* 0x00000001 */
#define I2C_SR2_MSL                         I2C_SR2_MSL_Msk                     /* Master/Slave */

#define I2C_SR2_BUSY_Pos                    (1U)
#define I2C_SR2_BUSY_Msk                    (0x1UL << I2C_SR2_BUSY_Pos)         /* 0x00000002 */
#define I2C_SR2_BUSY                        I2C_SR2_BUSY_Msk                    /* Bus Busy */

#define I2C_SR2_TRA_Pos                     (2U)
#define I2C_SR2_TRA_Msk                     (0x1UL << I2C_SR2_TRA_Pos)          /* 0x00000004 */
#define I2C_SR2_TRA                         I2C_SR2_TRA_Msk                     /* Transmitter/Receiver */

#define I2C_SR2_GENCALL_Pos                 (4U)
#define I2C_SR2_GENCALL_Msk                 (0x1UL << I2C_SR2_GENCALL_Pos)      /* 0x00000010 */
#define I2C_SR2_GENCALL                     I2C_SR2_GENCALL_Msk                 /* General Call Address (Slave mode) */

#define I2C_SR2_SMBDEFAULT_Pos              (5U)
#define I2C_SR2_SMBDEFAULT_Msk              (0x1UL << I2C_SR2_SMBDEFAULT_Pos)   /* 0x00000020 */
#define I2C_SR2_SMBDEFAULT                  I2C_SR2_SMBDEFAULT_Msk              /* SMBus Device Default Address (Slave mode) */

#define I2C_SR2_SMBHOST_Pos                 (6U)
#define I2C_SR2_SMBHOST_Msk                 (0x1UL << I2C_SR2_SMBHOST_Pos)      /* 0x00000040 */
#define I2C_SR2_SMBHOST                     I2C_SR2_SMBHOST_Msk                 /* SMBus Host Header (Slave mode) */

#define I2C_SR2_DUALF_Pos                   (7U)
#define I2C_SR2_DUALF_Msk                   (0x1UL << I2C_SR2_DUALF_Pos)        /* 0x00000080 */
#define I2C_SR2_DUALF                       I2C_SR2_DUALF_Msk                   /* Dual Flag (Slave mode) */

#define I2C_SR2_PEC_Pos                     (8U)
#define I2C_SR2_PEC_Msk                     (0xFFUL << I2C_SR2_PEC_Pos)         /* 0x0000FF00 */
#define I2C_SR2_PEC                         I2C_SR2_PEC_Msk                     /* Packet Error Checking Register */

#endif /* STM32F401XC_H_ */