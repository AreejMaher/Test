/*
 * stm32f401xc_SPI_driver.c
 *
 * Created on: Sep 2, 2025
 * Author: Ahmed Ezzat
 */

//------------------------------
// Includes
//------------------------------
#include "stm32f401xc_SPI_driver.h"

//============================================================

/*
 * =======================================================
 * 					  Generic Variables
 * =======================================================
 */
SPI_PinConfig_t *G_SPI_Config[3] = {NULL, NULL, NULL};
uint8_t G_SPI_Index;

SPI_Typedef *G_SPIx = NULL;

//============================================================

/*
 * =======================================================
 * 			Generic Macros
 * =======================================================
 */
#define SPI1_Index		0
#define SPI2_Index		1
#define SPI3_Index		2

//============================================================

/*
 * =======================================================
 * 			Generic Functions
 * =======================================================
 */
static void MCAL_SPI_SetPins(SPI_Typedef *SPIx);

//============================================================

/*
 * ========================================================================
 * 				 				  ISR
 * ========================================================================
 */
void SPI1_IRQHandler(void){
	S_SPI_IRQ_SRC irq_src;

	irq_src.TXE  = ((SPI1->SR &  (1<<1)) >> 1);
	irq_src.RXNE = ((SPI1->SR &  (1<<0)) >> 0);
	irq_src.ERRI = ((SPI1->SR &  (1<<4)) >> 4);

	G_SPI_Config[SPI1_Index]->P_IRQ_CallBack(irq_src);
}

void SPI2_IRQHandler(void){
	S_SPI_IRQ_SRC irq_src;

	irq_src.TXE  = ((SPI2->SR &  (1<<1)) >> 1);
	irq_src.RXNE = ((SPI2->SR &  (1<<0)) >> 0);
	irq_src.ERRI = ((SPI2->SR &  (1<<4)) >> 4);

	G_SPI_Config[SPI2_Index]->P_IRQ_CallBack(irq_src);
}

void SPI3_IRQHandler(void){
	S_SPI_IRQ_SRC irq_src;

	irq_src.TXE  = ((SPI3->SR &  (1<<1)) >> 1);
	irq_src.RXNE = ((SPI3->SR &  (1<<0)) >> 0);
	irq_src.ERRI = ((SPI3->SR &  (1<<4)) >> 4);

	G_SPI_Config[SPI3_Index]->P_IRQ_CallBack(irq_src);
}

//============================================================

/*
 * =================================================
 * 		   APIs Supported by "SPI DRIVER"
 * =================================================
 */
void MCAL_SPI_Init(SPI_Typedef *SPIx, SPI_PinConfig_t *SPI_Config){
	G_SPIx = SPIx;

	// Safety Registers
	uint16_t temp_CR1 = 0;
	uint16_t temp_CR2 = 0;

	// Enable SPI Clock
	if(SPIx == SPI1){
		G_SPI_Config[SPI1_Index] = SPI_Config;
		RCC_SPI1_CLK_EN();
		MCAL_SPI_SetPins(SPI1);
		G_SPI_Index = SPI1_Index;
	}
	else if(SPIx == SPI2){
		G_SPI_Config[SPI2_Index] = SPI_Config;
		RCC_SPI2_CLK_EN();
		MCAL_SPI_SetPins(SPI2);
		G_SPI_Index = SPI2_Index;
	}
	else if(SPIx == SPI3){
		G_SPI_Config[SPI3_Index] = SPI_Config;
		RCC_SPI3_CLK_EN();
		MCAL_SPI_SetPins(SPI3);
		G_SPI_Index = SPI3_Index;
	}

	SPIx->CR1 = 0;
	SPIx->CR2 = 0;

	// Choose SPI Mode
	temp_CR1 |= SPI_Config->SPI_Mode;

	// Set Communication Mode
	temp_CR1 |= SPI_Config->SPI_Communication_Mode;

	// Set Frame Format
	temp_CR1 |= SPI_Config->SPI_FrameFormat;

	// Select Data Size
	temp_CR1 |= SPI_Config->SPI_DataSize;

	// Set Clock Phase
	temp_CR1 |= SPI_Config->SPI_CPHA;

	// Set Clock Polarity
	temp_CR1 |= SPI_Config->SPI_CPOL;

	// Set NSS (HW or SW)
	if(SPI_NSS_HW_Master_SS_OutPut_DIS == (SPI_Config->SPI_NSS)){
		temp_CR2 &= ~(SPI_Config->SPI_NSS);
	}
	else if(SPI_NSS_HW_Master_SS_OutPut_EN == (SPI_Config->SPI_NSS)){
		temp_CR2 |= (SPI_Config->SPI_NSS);
	}
	else{
		temp_CR1 |= (SPI_Config->SPI_NSS);
	}

	// Set BaudRate
	temp_CR1 |= SPI_Config->SPI_BaudRatePrescaler;

	// Enable / Disable Interrupt
	if(SPI_IRQ_EN_None != (SPI_Config->SPI_IRQ_Enable)){
		temp_CR2 |= SPI_Config->SPI_IRQ_Enable;
		if(SPIx == SPI1){
			NVIC_IRQ_SPI1_EN();
		}
		else if(SPIx == SPI2){
			NVIC_IRQ_SPI2_EN();
		}
		else if(SPIx == SPI3){
			NVIC_IRQ_SPI3_EN();
		}
	}

	SPIx->CR1 = temp_CR1;
	SPIx->CR2 = temp_CR2;

	// Enable SPI
	SPIx->CR1 |= (1 << 6);
}

void MCAL_SPI_DeInit(SPI_Typedef *SPIx){
	// Disable SPI Clock & Interrupt
	if(SPIx == SPI1){
		RCC_SPI1_CLK_RST_SET();
		NVIC_IRQ_SPI1_DIS();
	}
	else if(SPIx == SPI2){
		RCC_SPI2_CLK_RST_SET();
		NVIC_IRQ_SPI2_DIS();
	}
	else if(SPIx == SPI3){
		RCC_SPI3_CLK_RST_SET();
		NVIC_IRQ_SPI3_DIS();
	}
}

void MCAL_SPI_SendData(uint8_t *PxBuffer, Polling_Mechanism_t pollin_status){
	if((G_SPI_Config[G_SPI_Index]->SPI_Communication_Mode == SPI_Communication_MODE_2Lines) || (G_SPI_Config[G_SPI_Index]->SPI_Communication_Mode == SPI_Communication_MODE_1Line_Transmit_Only)){
		if(Polling_Enable == pollin_status){
			while(!(G_SPIx->SR & (1 << 1)));
			G_SPIx->DR =  (*PxBuffer);
		}
		else{
			G_SPIx->DR =  (*PxBuffer);
		}
	}
	else{
		/* To Do */
	}
}

void MCAL_SPI_ReceiveData(uint8_t *PxBuffer, Polling_Mechanism_t pollin_status){
	if((G_SPI_Config[G_SPI_Index]->SPI_Communication_Mode == SPI_Communication_MODE_2Lines_RXONLY) | (G_SPI_Config[G_SPI_Index]->SPI_Communication_Mode == SPI_Communication_MODE_1Line_Receive_Only)){
		if(Polling_Enable == pollin_status){
			while(!(G_SPIx->SR & (1 << 0)));
			(*PxBuffer) = G_SPIx->DR;
		}
		else{
			(*PxBuffer) = G_SPIx->DR;
		}
	}
	else{
		/* To Do */
	}
}

void MCAL_SPI_Transceive1(uint8_t *PxBuffer, Polling_Mechanism_t pollin_status){
    MCAL_SPI_SendData(PxBuffer, pollin_status);
    MCAL_SPI_ReceiveData(PxBuffer, pollin_status);
}

void MCAL_SPI_Transceive2(uint8_t *PxBuffer, Polling_Mechanism_t pollin_status){
    // 1. Wait for TX Buffer Empty
    if (Polling_Enable == pollin_status)
    {
        while(!(G_SPIx->SR & (1 << 1)));
    }

    // 2. Send Data
    G_SPIx->DR = (*PxBuffer);

    // 3. Wait for RX Buffer Not Empty (Data received from nRF)
    if (Polling_Enable == pollin_status)
    {
        while(!(G_SPIx->SR & (1 << 0)));
    }

    // 4. READ the data (This clears the RXNE flag and gets the value)
    (*PxBuffer) = G_SPIx->DR;

}

static void MCAL_SPI_SetPins(SPI_Typedef *SPIx){
	GPIO_PinConfig_t Pin_Config;

	// ========================================================
	// [FIX] Initialize common settings to prevent garbage values
	// ========================================================
	Pin_Config.GPIO_TYPE = GPIO_TYPE_PP;        // Push-Pull (Critical for clean signals)
	Pin_Config.GPIO_Output_Speed = GPIO_SPEED_HIGH; // High Speed for SPI communication
	Pin_Config.GPIO_PU_PD = GPIO_PU_PD_NONE;    // No Pull-up/down (Let the driver drive the line)
	// ========================================================

	if(SPIx == SPI1){
		// MOSI ===> PA7
		// MISO ===> PA6
		// SCK  ===> PA5
		// NSS  ===> PA4

		// MOSI ===> A7
		Pin_Config.GPIO_PinNumber = GPIO_PIN_7;
		Pin_Config.GPIO_MODE = GPIO_MODE_AF;
		Pin_Config.GPIO_AFx = GPIO_AF5;
		MCAL_GPIO_Init(GPIOA, &Pin_Config);

		// MISO ===> A6
		Pin_Config.GPIO_PinNumber = GPIO_PIN_6;
		Pin_Config.GPIO_MODE = GPIO_MODE_AF;
		Pin_Config.GPIO_AFx = GPIO_AF5;
		MCAL_GPIO_Init(GPIOA, &Pin_Config);

		// SCK  ===> A5
		Pin_Config.GPIO_PinNumber = GPIO_PIN_5;
		Pin_Config.GPIO_MODE = GPIO_MODE_AF;
		Pin_Config.GPIO_AFx = GPIO_AF5;
		MCAL_GPIO_Init(GPIOA, &Pin_Config);

		// NSS  ===> A4
		Pin_Config.GPIO_PinNumber = GPIO_PIN_4;
		Pin_Config.GPIO_MODE = GPIO_MODE_AF;
		Pin_Config.GPIO_AFx = GPIO_AF5;
		MCAL_GPIO_Init(GPIOA, &Pin_Config);
	}
	else if(SPIx == SPI2){
		// MOSI ===> PB15
		// MISO ===> PB14
		// SCK  ===> PB13
		// NSS  ===> PB12

		// MOSI ===> PB15
		Pin_Config.GPIO_PinNumber = GPIO_PIN_15;
		Pin_Config.GPIO_MODE = GPIO_MODE_AF;
		Pin_Config.GPIO_AFx = GPIO_AF5;
		MCAL_GPIO_Init(GPIOB, &Pin_Config);

		// MISO ===> PB14
		Pin_Config.GPIO_PinNumber = GPIO_PIN_14;
		Pin_Config.GPIO_MODE = GPIO_MODE_AF;
		Pin_Config.GPIO_AFx = GPIO_AF5;
		MCAL_GPIO_Init(GPIOB, &Pin_Config);

		// SCK  ===> PB13
		Pin_Config.GPIO_PinNumber = GPIO_PIN_13;
		Pin_Config.GPIO_MODE = GPIO_MODE_AF;
		Pin_Config.GPIO_AFx = GPIO_AF5;
		MCAL_GPIO_Init(GPIOB, &Pin_Config);

		// NSS  ===> PB12
		Pin_Config.GPIO_PinNumber = GPIO_PIN_12;
		Pin_Config.GPIO_MODE = GPIO_MODE_AF;
		Pin_Config.GPIO_AFx = GPIO_AF5;
		MCAL_GPIO_Init(GPIOB, &Pin_Config);

	}
	else if(SPIx == SPI3){

		// MOSI ===> PB5
		// MISO ===> PB4
		// SCK  ===> PB3
		// NSS  ===> PA15

		// MOSI ===> PB5
		Pin_Config.GPIO_PinNumber = GPIO_PIN_5;
		Pin_Config.GPIO_MODE = GPIO_MODE_AF;
		Pin_Config.GPIO_AFx = GPIO_AF6;
		MCAL_GPIO_Init(GPIOB, &Pin_Config);

		// MISO ===> PB4
		Pin_Config.GPIO_PinNumber = GPIO_PIN_4;
		Pin_Config.GPIO_MODE = GPIO_MODE_AF;
		Pin_Config.GPIO_AFx = GPIO_AF6;
		MCAL_GPIO_Init(GPIOB, &Pin_Config);

		// SCK  ===> PB3
		Pin_Config.GPIO_PinNumber = GPIO_PIN_3;
		Pin_Config.GPIO_MODE = GPIO_MODE_AF;
		Pin_Config.GPIO_AFx = GPIO_AF6;
		MCAL_GPIO_Init(GPIOB, &Pin_Config);

//		// NSS  ===> PA15
//		Pin_Config.GPIO_PinNumber = GPIO_PIN_15;
//		Pin_Config.GPIO_MODE = GPIO_MODE_AF;
//		Pin_Config.GPIO_AFx = GPIO_AF6;
//		MCAL_GPIO_Init(GPIOA, &Pin_Config);
	}
}


void MCAL_SPI_Select(SPI_Typedef *SPIx){
    if (SPIx == SPI1) {
        G_SPIx = SPI1;
        G_SPI_Index = SPI1_Index;
    } else if (SPIx == SPI2) {
        G_SPIx = SPI2;
        G_SPI_Index = SPI2_Index;
    } else if (SPIx == SPI3) {
        G_SPIx = SPI3;
        G_SPI_Index = SPI3_Index;
    } else {
        /* invalid SPIx: do nothing or set to NULL */
        G_SPIx = NULL;
    }
}
