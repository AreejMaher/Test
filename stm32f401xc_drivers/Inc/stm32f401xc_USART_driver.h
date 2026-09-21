/*
 * stm32f401xc_USART_driver.h
 *
 * Created on: Aug 31, 2025
 * Author: Ahmed Ezzat
 */

#ifndef INC_STM32F401XC_USART_DRIVER_H_
#define INC_STM32F401XC_USART_DRIVER_H_

//------------------------------
// Includes
//------------------------------
#include "stm32f401xc.h"
#include "stm32f401xc_gpio_driver.h"

//============================================================

//------------------------------------------
// User Type Definitions (Structures)
//------------------------------------------
typedef struct{
    uint8_t TXE  	 : 1; // TX buffer empty interrupt
    uint8_t RXNE 	 : 1; // RX buffer not empty interrupt
    uint8_t TCE 	 : 1; // Transmission complete interrupt
    uint8_t Reserved : 5;
}S_USART_IRQ_SRC;

typedef struct {
    uint8_t USART_Mode;				// Specifies TX/RX Enable/Disable
    // This parameter must be set base on @ref UART_MODE_define

    uint8_t USART_PayLoad_Lenght;	// Specifies Number of data bits transmitted or received in data frame
    // This parameter must be set base on @ref UART_PayLoad_Length_define

    uint8_t USART_ParityMode;		// Specifies parity mode
    // This parameter must be set base on @ref UART_Parity_define

    uint8_t USART_StopBits;			// Specifies the number of stop bits transmitted
    // This parameter must be set base on @ref UART_StopBits_define

    uint32_t USART_BaudRate;		// This member configures UART communication baudrate
    // This parameter must be set base on @ref UART_BAUDRATE_define

    uint8_t USART_Sampling;			// This member configures UART Sampling Mode
    // This parameter must be set base on @ref UART_Sampling_define

    uint8_t USART_HW_FlowCTRL;		// Specifies whether hardware flow control enabled or disabled
    // This parameter must be set base on @ref UART_HW_FLW_CTRL_define

    uint8_t USART_IRQ_Enable;		// Specifies whether IRQ of USART is Enabled or Disabled
    // This parameter must be set base on @ref UART_IRQ_ENABLE_define

    void (*P_IRQ_CallBack)(S_USART_IRQ_SRC irq_src);	// Set the C function which will be call when interrupt happens
} USART_PinConfig_t;

typedef enum {
    Disable, Enable
} PollingMechanism_t;

//============================================================

// @ref UART_MODE_define
// Bit 2 RE: Receiver enable
//	0: Receiver is disabled
//	1: Receiver is enabled and begins searching for a start bit
// Bit 3 TE: Transmitter enable
//	0: Transmitter is disabled
//	1: Transmitter is enabled
#define UART_MODE_RX						(uint32_t)(1 << 2)
#define UART_MODE_TX						(uint32_t)(1 << 3)
#define UART_MODE_TX_RX						(uint32_t)(3 << 2)

// @ref UART_PayLoad_Length_define
// Bit 12 M: Word length
//	0: 1 Start bit, 8 Data bits, n Stop bit
//	1: 1 Start bit, 9 Data bits, n Stop bit
#define UART_PayLoad_Length_8Bits			(uint32_t)(0)
#define UART_PayLoad_Length_9Bits			(uint32_t)(1 << 12)

// @ref UART_Parity_define
// Bit 10 PCE: Parity control enable
//	0: Parity control disabled
//	1: Parity control enabled
// Bit 9 PS: Parity selection
/* This bit selects the odd or even parity when the parity generation/detection is enabled (PCE
 bit set). It is set and cleared by software. The parity will be selected after the current byte.*/
//	0: Even parity
//	1: Odd parity
#define UART_Parity_DIS						(uint32_t)(0)
#define UART_Parity_Even					(uint32_t)((1 << 10) | (0))
#define UART_Parity_Odd						(uint32_t)((1 << 10) | (1 << 9))

// @ref UART_StopBits_define
// Bits 13:12 STOP: STOP bits
//	00: 1 Stop bit
//	01: 0.5 Stop bit
//	10: 2 Stop bits
//	11: 1.5 Stop bit
#define UART_StopBits_Half_Bit				(uint32_t)(1 << 12)
#define UART_StopBits_1Bit					(uint32_t)(0)
#define UART_StopBits_1_Half_Bit			(uint32_t)(3 << 12)
#define UART_StopBits_2_Bit					(uint32_t)(2 << 12)

// @ref UART_BAUDRATE_define
#define UART_BaudRate_2400						2400
#define UART_BaudRate_9600						9600
#define UART_BaudRate_19200						19200
#define UART_BaudRate_57600						57600
#define UART_BaudRate_115200					115200
#define UART_BaudRate_230400					230400
#define UART_BaudRate_460800					460800
#define UART_BaudRate_921600					921600
#define UART_BaudRate_2250000					2250000
#define UART_BaudRate_4500000					4500000

// @ref UART_Sampling_define
// Bit 15 OVER8: Oversampling mode
//  0: oversampling by 16
//	1: oversampling by 8
#define UART_Sampling_16					(uint32_t)(0)
#define UART_Sampling_8						(uint32_t)(1 << 15)

// @ref UART_HW_FLW_CTRL_define
// Bit 8 RTSE: RTS enable
//  0: RTS hardware flow control disabled
//	1: RTS interrupt enabled
// Bit 9 CTSE: CTS enable
//	0: CTS hardware flow control disabled
//	1: CTS mode enabled
#define UART_HW_FLW_CTRL_RTS_DIS			(uint32_t)(0)
#define UART_HW_FLW_CTRL_CTS_DIS			(uint32_t)(0)
#define UART_HW_FLW_CTRL_RTS				(uint32_t)(1 << 8)
#define UART_HW_FLW_CTRL_CTS				(uint32_t)(1 << 9)
#define UART_HW_FLW_CTRL_CTS_RTS			(uint32_t)((1 << 8) | (1 << 9))

// @ref UART_IRQ_ENABLE_define
// Bit 5 RXNEIE: RXNE interrupt enable
/* This bit is set and cleared by software */

//	0: Interrupt is inhibited
//	1: An USART interrupt is generated whenever ORE=1 or RXNE=1 in the USART_SR
// Bit 6 TCIE: Transmission complete interrupt enable
/* This bit is set and cleared by software */

//	0: Interrupt is inhibited
//	1: An USART interrupt is generated whenever TC=1 in the USART_SR register
// Bit 7 TXEIE: TXE interrupt enable
/* This bit is set and cleared by software */

//	0: Interrupt is inhibited
//	1: An USART interrupt is generated whenever TXE=1 in the USART_SR register
// Bit 8 PEIE: PE interrupt enable
/* This bit is set and cleared by software */

//	0: Interrupt is inhibited
//	1: An USART interrupt is generated whenever PE=1 in the USART_SR register
#define UART_IRQ_ENABLE_NONE				(uint32_t)(0)
#define UART_IRQ_ENABLE_RXNEIE				(uint32_t)(1 << 5)
#define UART_IRQ_ENABLE_TCIE				(uint32_t)(1 << 6)
#define UART_IRQ_ENABLE_TXEIE				(uint32_t)(1 << 7)
#define UART_IRQ_ENABLE_TXEIE_RXNEIE			(uint32_t)((1 << 7) | (1 << 5))
#define UART_IRQ_ENABLE_PEIE				(uint32_t)(1 << 8)

//============================================================

/*
 * =====================================================
 * 		  APIs Supported by "MCAL USART DRIVER"
 * =====================================================
 */
void MCAL_UART_Init(USART_TypeDef *UARTx, USART_PinConfig_t *UART_config);
void MCAL_UART_DeInit(USART_TypeDef *UARTx);

void MCAL_UART_SendData(USART_TypeDef *USARTx, uint8_t *PxBuffer, PollingMechanism_t polling_status);
void MCAL_UART_ReceiveData(USART_TypeDef *USARTx, uint8_t *PxBuffer, PollingMechanism_t polling_status);

void MCAL_UART_SendString(USART_TypeDef *USARTx, uint8_t *str, PollingMechanism_t polling_status);

#endif /* INC_STM32F401XC_USART_DRIVER_H_ */
