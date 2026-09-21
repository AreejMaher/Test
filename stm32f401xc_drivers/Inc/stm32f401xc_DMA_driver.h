/*
 * stm32f401xc_DMA_driver.h
 *
 * Created on: Nov 18, 2025
 * Author: Ahmed Ezzat
 */

#ifndef INC_STM32F401XC_DMA_DRIVER_H_
#define INC_STM32F401XC_DMA_DRIVER_H_

#include "stm32f401xc.h"

typedef enum{
    byte,
    HalfWord,
    Word
}SIZE_t;

typedef enum{
    FIFO_1_4,
    FIF0_1_2,
    FIFO_3_4,
    FULL_FIFO
}THRESHOLD_t;

#define PSIZE	11
#define MSIZE	13

void MCAL_DMA_Init(DMA_TypeDef *DMAx, uint8_t StreamID, uint8_t Channel_Num);
void MCAL_DMA_SetStream(DMA_TypeDef *DMAx, uint8_t StreamID,
		   uint32_t *SourceAddr, uint32_t *DestAddr,
		   SIZE_t PSize, SIZE_t MSize,
		   uint16_t BlockSize, THRESHOLD_t
		   Threshold);

void MCAL_DMA_EnableStream(uint8_t StreamID);
void MCAL_DMA_CallBack(uint8_t StreamID, void(*F_ptr)(void));

#endif /* INC_STM32F401XC_DMA_DRIVER_H_ */
