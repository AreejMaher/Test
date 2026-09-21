/*
 * stm32f401xc_SPI_driver.h
 *
 * Created on: Sep 2, 2025
 * Author: Ahmed Ezzat
 */

#ifndef INC_STM32F401XC_SPI_DRIVER_H_
#define INC_STM32F401XC_SPI_DRIVER_H_

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
	uint8_t ERRI 	 : 1; // Error interrupt
	uint8_t Reserved : 5;
}S_SPI_IRQ_SRC;

typedef struct{
	uint16_t SPI_Mode 						;	// Specifies the mode of SPI (Master / Slave)
												// This parameter must be set base on @ref SPI_MODE_define

	uint16_t SPI_Communication_Mode			;	// Specifies the bidirectional mode of SPI
												// This parameter must be set base on @ref SPI_Communication_MODE_define

	uint16_t SPI_FrameFormat				;	// Specifies the shape of frame of SPI (MSB / LSB) first
												// This parameter must be set base on @ref SPI_FrameFormat_define

	uint16_t SPI_DataSize					;	// Specifies the size of transmitted data of SPI (8 / 16) bit
												// This parameter must be set base on @ref SPI_DataSize_define

	uint16_t SPI_CPHA						;	// Specifies the Phase shape of SPI
												// This parameter must be set base on @ref SPI_CPHA_define

	uint16_t SPI_CPOL						;	// Specifies the Polarity of SPI
												// This parameter must be set base on @ref SPI_CPOL_define

	uint16_t SPI_NSS						;	// Specifies how NSS been managed (SW/ HW)
												// This parameter must be set base on @ref SPI_NSS_define

	uint16_t SPI_BaudRatePrescaler			;	// Specifies the BaudRate of SPI
												// This parameter must be set base on @ref SPI_BaudRate_define

	uint16_t SPI_IRQ_Enable					;	// Enable Interrupt
												// This parameter must be set base on @ref SPI_IRQ_EN_define

	void(* P_IRQ_CallBack)(S_SPI_IRQ_SRC irq_src)		;	// Pointer of CallBack function
}SPI_PinConfig_t;

typedef enum{
	Polling_Enable,
	Polling_Disable
}Polling_Mechanism_t;

//============================================================

//-------------------------
// Reference Macros
//-------------------------

// @ref SPI_MODE_define
// Bit 2 MSTR: Master selection
//	0: Slave configuration
//	1: Master configuration
#define SPI_MODE_Slave									(uint32_t)(0)
#define SPI_MODE_Master									(uint32_t)(1 << 2)

// @ref SPI_Communication_MODE_define
// Bit 15 BIDIMODE: Bidirectional data mode enable
//	0: 2-line unidirectional data mode selected
//	1: 1-line bidirectional data mode selected

// Bit 14 BIDIOE: Output enable in bidirectional mode
//	0: Output disabled (receive-only mode)
//	1: Output enabled (transmit-only mode)
/* This bit combined with the BIDImode bit selects the direction of transfer in bidirectional mode */
/* In master mode, the MOSI pin is used while the MISO pin is used in slave mode  */

// Bit 10 RXONLY: Receive only
/* This bit combined with the BIDImode bit selects the direction of transfer in 2-line unidirectional mode */
//	0: Full duplex (Transmit and receive)
//	1: Output disabled (Receive-only mode)
/* This bit combined with the BIDImode bit selects the direction of transfer in 2-line unidirectional mode */
#define SPI_Communication_MODE_2Lines					(uint32_t)(0)
#define SPI_Communication_MODE_2Lines_RXONLY			(uint32_t)(1 << 10)
#define SPI_Communication_MODE_1Line_Receive_Only		(uint32_t)(1 << 15)
#define SPI_Communication_MODE_1Line_Transmit_Only		(uint32_t)((1 << 15) | (1 << 14))

// @ref SPI_FrameFormat_define
// Bit 7 LSBFIRST: Frame format
//	0: MSB transmitted first
//	1: LSB transmitted first
#define SPI_FrameFormat_MSB								(uint32_t)(0)
#define SPI_FrameFormat_LSB								(uint32_t)(1 << 7)

// @ref SPI_DataSize_define
// Bit 11 DFF: Data frame format
//	0: 8-bit data frame format is selected for transmission/reception
//	1: 16-bit data frame format is selected for transmission/reception
/* This bit should be written only when SPI is disabled (SPE = ‘0’) for correct operation */
#define SPI_MODE_8Bit									(uint32_t)(0)
#define SPI_MODE_16Bit									(uint32_t)(1 << 11)

// @ref SPI_CPHA_define
// Bit 0 CPHA: Clock phase
//	0: The first clock transition is the first data capture edge
//	1: The second clock transition is the first data capture edge
/* Note: This bit should not be changed when communication is ongoing */
#define SPI_CPHA_FirstEdge								(uint32_t)(0)
#define SPI_CPHA_SecondEdge								(uint32_t)(1 << 0)

// @ref SPI_CPOL_define
// Bit1 CPOL: Clock polarity
//	0: CK to 0 when idle
//	1: CK to 1 when idle
/* Note: This bit should not be changed when communication is ongoing */
#define SPI_CPOL_Idle_0									(uint32_t)(0)
#define SPI_CPOL_Idle_1									(uint32_t)(1 << 1)

// @ref SPI_NSS_define
// Bit 9 SSM: Software slave management
//	0: Software slave management disabled
//	1: Software slave management enabled
/* When the SSM bit is set, the NSS pin input is replaced with the value from the SSI bit */

// Bit 8 SSI: Internal slave select
/* This bit has an effect only when the SSM bit is set. The value of this bit is forced onto the NSS pin and the IO value of the NSS pin is ignored */

// Bit 2 SSOE: SS output enable
//	0: SS output is disabled in master mode and the cell can work in multimaster configuration
//	1: SS output is enabled in master mode and when the cell is enabled. The cell cannot work in a multimaster environment

// HardWare NSS
#define SPI_NSS_HW_Slave								(uint32_t)(0)
#define SPI_NSS_HW_Master_SS_OutPut_DIS					(uint32_t)(0)
#define SPI_NSS_HW_Master_SS_OutPut_EN					(uint32_t)(1 << 2)

// SoftWare NSS
#define SPI_NSS_SW_SSI_DIS								(uint32_t)(1 << 9)
#define SPI_NSS_SW_SSI_EN								(uint32_t)((1 << 9) | (1 << 8))

// @ref SPI_BaudRate_define
// Bits 5:3 BR[2:0]: Baud rate control
//	000: fPCLK/2
//	001: fPCLK/4
//	010: fPCLK/8
//	011: fPCLK/16
//	100: fPCLK/32
//	101: fPCLK/64
//	110: fPCLK/128
//	111: fPCLK/256
#define SPI_BaudRate_Prescaler_2						(uint32_t)(0)
#define SPI_BaudRate_Prescaler_4						(uint32_t)(1 << 3)
#define SPI_BaudRate_Prescaler_8						(uint32_t)(2 << 3)
#define SPI_BaudRate_Prescaler_16						(uint32_t)(3 << 3)
#define SPI_BaudRate_Prescaler_32						(uint32_t)(4 << 3)
#define SPI_BaudRate_Prescaler_64						(uint32_t)(5 << 3)
#define SPI_BaudRate_Prescaler_128						(uint32_t)(6 << 3)
#define SPI_BaudRate_Prescaler_256						(uint32_t)(7 << 3)

// @ref SPI_IRQ_EN_define
// Bit 7 TXEIE: Tx buffer empty interrupt enable
//	0: TXE interrupt masked
//	1: TXE interrupt not masked. Used to generate an interrupt request when the TXE flag is set.

// Bit 6 RXNEIE: RX buffer not empty interrupt enable
//	0: RXNE interrupt masked
//	1: RXNE interrupt not masked. Used to generate an interrupt request when the RXNE flag is set.

// Bit 5 ERRIE: Error interrupt enable
//	0: Error interrupt is masked
//  1: Error interrupt is enabled
/* This bit controls the generation of an interrupt when an error condition occurs )(CRCERR, OVR, MODF in SPI mode, FRE in TI mode and UDR, OVR, and FRE in I2S mode) */
#define SPI_IRQ_EN_None									(uint32_t)(0)
#define SPI_IRQ_EN_TXEIE								(uint32_t)(1 << 7)
#define SPI_IRQ_EN_RXNEIE								(uint32_t)(1 << 6)
#define SPI_IRQ_EN_ERRIE								(uint32_t)(1 << 5)

//============================================================

/*
 * =================================================
 * 	     APIs Supported by "SPI DRIVER"
 * =================================================
 */
void MCAL_SPI_Init(SPI_Typedef *SPIx, SPI_PinConfig_t *SPI_Config);
void MCAL_SPI_DeInit(SPI_Typedef *SPIx);
void MCAL_SPI_Select(SPI_Typedef *SPIx);

void MCAL_SPI_SendData(uint8_t *PxBuffer, Polling_Mechanism_t pollin_status);
void MCAL_SPI_ReceiveData(uint8_t *PxBuffer, Polling_Mechanism_t pollin_status);

void MCAL_SPI_Transceive1(uint8_t *PxBuffer, Polling_Mechanism_t pollin_status);
void MCAL_SPI_Transceive2(uint8_t *PxBuffer, Polling_Mechanism_t pollin_status);

#endif /* INC_STM32F401XC_SPI_DRIVER_H_ */
