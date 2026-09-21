/*
 * stm32f401xc_USART_driver.c
 *
 * Created on: Aug 31, 2025
 * Author: Ahmed Ezzat
 */

//------------------------------
// Includes
//------------------------------
#include "stm32f401xc_USART_driver.h"

//============================================================

/*
 * =======================================================
 * 		   Generic Variables
 * =======================================================
 */
USART_PinConfig_t *G_UART_config = NULL;

//============================================================

/*
 * =======================================================
 * 		   Generic Macros
 * =======================================================
 */

//============================================================

/*
 * =======================================================
 * 					Generic Functions
 * =======================================================
 *
 */
static void MCAL_USART_SetPins(USART_TypeDef *USARTx);

//============================================================

/*
 * ========================================================================
 * 				 				  ISR
 * ========================================================================
 */
void USART1_IRQHandler(void){
    S_USART_IRQ_SRC irq_src;

    irq_src.TXE  = ((USART1->SR &  (1<<7)) >> 7);
    irq_src.RXNE = ((USART1->SR &  (1<<5)) >> 5);
    irq_src.TCE  = ((USART1->SR &  (1<<6)) >> 6);

    G_UART_config->P_IRQ_CallBack(irq_src);
}

//void USART2_IRQHandler(void){
//	G_UART_config->P_IRQ_CallBack();
//}
//
//void USART6_IRQHandler(void){
//	G_UART_config->P_IRQ_CallBack();
//}

//============================================================

/*
 * =====================================================
 * 		  APIs Supported by "MCAL USART DRIVER"
 * =====================================================
 */
void MCAL_UART_Init(USART_TypeDef *UARTx, USART_PinConfig_t *UART_config){
    G_UART_config = UART_config;

    // Enable USART Clock
    if(UARTx == USART1){
	RCC_USART1_CLK_EN();
	MCAL_USART_SetPins(USART1);
    }
    else if(UARTx == USART2){
	RCC_USART2_CLK_EN();
	MCAL_USART_SetPins(USART2);
    }
    else if(UARTx == USART6){
	RCC_USART6_CLK_EN();
	MCAL_USART_SetPins(USART6);
    }

    UARTx->CR1 = 0;
    UARTx->CR2 = 0;
    UARTx->CR3 = 0;

    // Enable UART
    UARTx->CR1 |= (1 << 13);

    // Choose Mode (TX/RX)
    UARTx->CR1 |= UART_config->USART_Mode;

    // Select Word Length
    UARTx->CR1 |= UART_config->USART_PayLoad_Lenght;

    // Choose Parity Mode
    UARTx->CR1 |= UART_config->USART_ParityMode;

    // Choose Number of Stop Bits
    UARTx->CR2 |= UART_config->USART_StopBits;

    // Set BaudRate
    UARTx->BRR = (104 << 4) | 3;

    // Choose Sample Mode
    UARTx->CR1 |= UART_config->USART_Sampling;

    // Configure Hardware Flow Control
    UARTx->CR3 |= UART_config->USART_HW_FlowCTRL;

    // Enable Interrupt
    if(UART_config->USART_IRQ_Enable != UART_IRQ_ENABLE_NONE){
	UARTx->CR1 |= UART_config->USART_IRQ_Enable;

	if(UARTx == USART1){
	    NVIC_IRQ_USART1_EN();
	}
	else if(UARTx == USART2){
	    NVIC_IRQ_USART2_EN();
	}
	else if(UARTx == USART6){
	    NVIC_IRQ_USART6_EN();
	}
    }
    else{
	UARTx->CR1 &= ~( (1<<5) | (1<<6) | (1<<7) | (1<<8) ); // clear RXNEIE,TCIE,TXEIE,PEIE
    }
}

void MCAL_UART_DeInit(USART_TypeDef *UARTx){
    if(UARTx == USART1){
	RCC_USART1_CLK_RST_SET();
	NVIC_IRQ_USART1_DIS();
    }
    else if(UARTx == USART2){
	RCC_USART2_CLK_RST_SET();
	NVIC_IRQ_USART1_DIS();
    }
    else if(UARTx == USART6){
	RCC_USART6_CLK_RST_SET();
	NVIC_IRQ_USART1_DIS();
    }
}

void MCAL_UART_SendData(USART_TypeDef *USARTx, uint8_t *PxBuffer, PollingMechanism_t polling_status){
    if(Enable == polling_status){
	while(!((USARTx->SR) & (1 << 7)));

	if((G_UART_config->USART_PayLoad_Lenght) == UART_PayLoad_Length_8Bits){
	    // This for Data sent 8-bit
	    USARTx->DR = (*PxBuffer & (uint16_t)0xFF);
	}
	else{
	    // This for Data sent 9-bit
	    USARTx->DR = (*PxBuffer & (uint16_t)0x01FF);

	    // Wait for TC (Transmission Complete)
	    while(!((USARTx->SR) & (1 << 6)));
	}
    }
    else{
	if((G_UART_config->USART_PayLoad_Lenght) == UART_PayLoad_Length_8Bits){
	    // This for Data sent 8-bit
	    USARTx->DR = (*PxBuffer & (uint16_t)0xFF);
	}
	else{
	    // This for Data sent 9-bit
	    USARTx->DR = (*PxBuffer & (uint16_t)0x01FF);
	}

	USARTx->CR1 |= (1 << 7);
    }
}

void MCAL_UART_ReceiveData(USART_TypeDef *USARTx, uint8_t *PxBuffer, PollingMechanism_t polling_status){
    if(Enable == polling_status){
	while (!(USARTx->SR & (1 << 5)));

	if(G_UART_config->USART_PayLoad_Lenght == UART_PayLoad_Length_9Bits){
	    // This for Data sent 9-bit
	    if(G_UART_config->USART_ParityMode == UART_Parity_DIS){
		*(PxBuffer) = USARTx->DR;
	    }
	    else{
		*(PxBuffer) = (USARTx->DR & (uint8_t)0xFF);
	    }
	}
	else{
	    // This for Data sent 8-bit
	    if(G_UART_config->USART_ParityMode == UART_Parity_DIS){
		*(PxBuffer) = USARTx->DR;
		// No parity So all 8bit are considered data
		*(PxBuffer) = (USARTx->DR  & (uint8_t)0xFF) ;
	    }
	    else{
		// Parity is used, so 7 bits will be of user data and 1 bit is parity
		*(PxBuffer) = (USARTx->DR  & (uint8_t)0X7F);
	    }
	}

    }
    else{
	if(G_UART_config->USART_PayLoad_Lenght == UART_PayLoad_Length_9Bits){
	    // This for Data sent 9-bit
	    if(G_UART_config->USART_ParityMode == UART_Parity_DIS){
		*(PxBuffer) = USARTx->DR;
	    }
	    else{
		*(PxBuffer) = (USARTx->DR & (uint8_t)0xFF);
	    }
	}
	else{
	    // This for Data sent 8-bit
	    if(G_UART_config->USART_ParityMode == UART_Parity_DIS){
		*(PxBuffer) = USARTx->DR;
		// No parity So all 8bit are considered data
		*(PxBuffer) = (USARTx->DR  & (uint8_t)0xFF) ;
	    }
	    else{
		// Parity is used, so 7 bits will be of user data and 1 bit is parity
		*(PxBuffer) = (USARTx->DR  & (uint8_t)0X7F);
	    }
	}
    }
}

/**
 * @brief Sends a string via UART
 * @param USARTx: UART peripheral (USART1, USART2, or USART6)
 * @param str: Pointer to null-terminated string
 * @param polling_status: Enable for blocking mode, Disable for non-blocking
 */
void MCAL_UART_SendString(USART_TypeDef *USARTx, uint8_t *str, PollingMechanism_t polling_status){
    uint16_t i = 0;

    while(str[i] != '\0'){
        // Wait for TXE (Transmit Data Register Empty)
        while(!(USARTx->SR & (1 << 7)));

        // Write data to DR
        USARTx->DR = (str[i] & 0xFF);

        // Wait for TC (Transmission Complete) before next character
        while(!(USARTx->SR & (1 << 6)));

        i++;
    }
}

static void MCAL_USART_SetPins(USART_TypeDef *USARTx){
    GPIO_PinConfig_t USART_PinConfig;

    if(USARTx == USART1){
	// TX  ===> PA9
	// RX  ===> PA10
	// CLK ===> PA8
	// CTS ===> PA11
	// RTS ===> PA12

	// TX  ===> PA9
	USART_PinConfig.GPIO_PinNumber = GPIO_PIN_9;
	USART_PinConfig.GPIO_MODE = GPIO_MODE_AF;
	USART_PinConfig.GPIO_AFx = GPIO_AF7;
	MCAL_GPIO_Init(GPIOA, &USART_PinConfig);

	// RX  ===> PA10
	USART_PinConfig.GPIO_PinNumber = GPIO_PIN_10;
	USART_PinConfig.GPIO_MODE = GPIO_MODE_AF;
	USART_PinConfig.GPIO_AFx = GPIO_AF7;
	MCAL_GPIO_Init(GPIOA, &USART_PinConfig);

	// CLK ===> PA8
	USART_PinConfig.GPIO_PinNumber = GPIO_PIN_8;
	USART_PinConfig.GPIO_MODE = GPIO_MODE_AF;
	USART_PinConfig.GPIO_AFx = GPIO_AF7;
	MCAL_GPIO_Init(GPIOA, &USART_PinConfig);

	// CTS ===> PA11
	if((G_UART_config->USART_HW_FlowCTRL == UART_HW_FLW_CTRL_CTS) | (G_UART_config->USART_HW_FlowCTRL == UART_HW_FLW_CTRL_RTS)){
	    USART_PinConfig.GPIO_PinNumber = GPIO_PIN_11;
	    USART_PinConfig.GPIO_MODE = GPIO_MODE_AF;
	    USART_PinConfig.GPIO_AFx = GPIO_AF7;
	    MCAL_GPIO_Init(GPIOA, &USART_PinConfig);
	}
	// RTS ===> PA12
	if((G_UART_config->USART_HW_FlowCTRL == UART_HW_FLW_CTRL_CTS) | (G_UART_config->USART_HW_FlowCTRL == UART_HW_FLW_CTRL_RTS)){
	    USART_PinConfig.GPIO_PinNumber = GPIO_PIN_12;
	    USART_PinConfig.GPIO_MODE = GPIO_MODE_AF;
	    USART_PinConfig.GPIO_AFx = GPIO_AF7;
	    MCAL_GPIO_Init(GPIOA, &USART_PinConfig);
	}
    }

    else if(USARTx == USART2){
	// TX  ===> PA2
	// RX  ===> PA3
	// CLK ===> PA4
	// CTS ===> PA0
	// RTS ===> PA1

	// TX  ===> PA2
	USART_PinConfig.GPIO_PinNumber = 2;
	USART_PinConfig.GPIO_MODE = GPIO_MODE_AF;
	USART_PinConfig.GPIO_AFx = GPIO_AF7;
	MCAL_GPIO_Init(GPIOA, &USART_PinConfig);

	// RX  ===> PA3
	USART_PinConfig.GPIO_PinNumber = 3;
	USART_PinConfig.GPIO_MODE = GPIO_MODE_AF;
	USART_PinConfig.GPIO_AFx = GPIO_AF7;
	MCAL_GPIO_Init(GPIOA, &USART_PinConfig);

	// CLK ===> PA4
	USART_PinConfig.GPIO_PinNumber = 4;
	USART_PinConfig.GPIO_MODE = GPIO_MODE_AF;
	USART_PinConfig.GPIO_AFx = GPIO_AF7;
	MCAL_GPIO_Init(GPIOA, &USART_PinConfig);

	// CTS ===> PA0
	if((G_UART_config->USART_HW_FlowCTRL == UART_HW_FLW_CTRL_CTS) | (G_UART_config->USART_HW_FlowCTRL == UART_HW_FLW_CTRL_RTS)){
	    USART_PinConfig.GPIO_PinNumber = 0;
	    USART_PinConfig.GPIO_MODE = GPIO_MODE_AF;
	    USART_PinConfig.GPIO_AFx = GPIO_AF7;
	    MCAL_GPIO_Init(GPIOA, &USART_PinConfig);
	}
	// RTS ===> PA1
	if((G_UART_config->USART_HW_FlowCTRL == UART_HW_FLW_CTRL_CTS) | (G_UART_config->USART_HW_FlowCTRL == UART_HW_FLW_CTRL_RTS)){
	    USART_PinConfig.GPIO_PinNumber = 1;
	    USART_PinConfig.GPIO_MODE = GPIO_MODE_AF;
	    USART_PinConfig.GPIO_AFx = GPIO_AF7;
	    MCAL_GPIO_Init(GPIOA, &USART_PinConfig);
	}
    }

    else if(USARTx == USART6){
	// TX  ===> PA11
	// RX  ===> PA12

	// TX  ===> PA11
	USART_PinConfig.GPIO_PinNumber = 11;
	USART_PinConfig.GPIO_MODE = GPIO_MODE_AF;
	USART_PinConfig.GPIO_AFx = GPIO_AF8;
	MCAL_GPIO_Init(GPIOA, &USART_PinConfig);

	// RX  ===> PA12
	USART_PinConfig.GPIO_PinNumber = 12;
	USART_PinConfig.GPIO_MODE = GPIO_MODE_AF;
	USART_PinConfig.GPIO_AFx = GPIO_AF8;
	MCAL_GPIO_Init(GPIOA, &USART_PinConfig);
    }
}
