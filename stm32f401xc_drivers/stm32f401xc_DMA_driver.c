/*
 * stm32f401xc_DMA_driver.c
 *
 * Created on: Nov 18, 2025
 * Author: Ahmed Ezzat
 */

#include "stm32f401xc_DMA_driver.h"

static void (*G_Fptr[8])(void) = {NULL};

/* Minimal local helpers (no external includes) */
#ifndef GET_BIT
#define GET_BIT(REG, BIT)  (((REG) >> (BIT)) & 1U)
#endif
#ifndef NOP
#define NOP() __asm volatile ("nop")
#endif

void MCAL_DMA_Init(DMA_TypeDef *DMAx, uint8_t StreamID, uint8_t Channel_Num){
    /* Disable stream first and wait until EN = 0 */
    DMAx->Stream[StreamID].CR &= ~(1U << 0);
    while (GET_BIT(DMAx->Stream[StreamID].CR, 0)) { NOP(); }

    /* Enable DMA clock for the selected DMA controller */
    if (DMAx == DMA2) {
	RCC_DMA2_CLK_EN();
    } else {
	RCC_DMA1_CLK_EN();
    }

    /* Clear CHSEL and set new channel */
    DMAx->Stream[StreamID].CR &= ~(0x7U << 25);
    DMAx->Stream[StreamID].CR |= ((uint32_t)(Channel_Num & 0x7U) << 25);

    /* Direction: peripheral -> memory (00) */
    DMAx->Stream[StreamID].CR &= ~(0x3U << 6);

    /* PINC = 0 (peripheral fixed), MINC = 1 (memory increment) */
    DMAx->Stream[StreamID].CR &= ~(1U << 9);   /* PINC = 0 */
    DMAx->Stream[StreamID].CR |=  (1U << 10);  /* MINC = 1 */

    /* Enable circular mode for continuous acquisition (CIRC = bit 8) */
    DMAx->Stream[StreamID].CR |= (1U << 8);

    /* Enable Transfer Complete interrupt (TCIE = bit 4) if desired */
    DMAx->Stream[StreamID].CR |= (1U << 4);
}


void MCAL_DMA_SetStream(DMA_TypeDef *DMAx, uint8_t StreamID,
	uint32_t *SourceAddr, uint32_t *DestAddr,
	SIZE_t PSize, SIZE_t MSize,
	uint16_t BlockSize, THRESHOLD_t
	Threshold){
    /* Disable stream and wait for EN to clear */
    DMAx->Stream[StreamID].CR &= ~(1U << 0);
    while (GET_BIT(DMAx->Stream[StreamID].CR, 0)) { NOP(); }

    /* Clear pending flags for this stream (use LIFCR/HIFCR) */
    if (StreamID <= 3) {
	uint32_t shift = StreamID * 6;
	DMAx->LIFCR = (0x3DUL << shift); /* clears FE/DME/TE/HT/TC for stream */
    } else {
	uint32_t shift = (StreamID - 4) * 6;
	DMAx->HIFCR = (0x3DUL << shift);
    }

    /* Set Source (peripheral) and Destination (memory) addresses (cast pointers) */
    DMAx->Stream[StreamID].PAR  = (uint32_t)SourceAddr;
    DMAx->Stream[StreamID].M0AR = (uint32_t)DestAddr;

    /* Select Size (PSIZE bits 11:12, MSIZE bits 13:14) - clear then set */
    /* Force PSIZE = 01 (half-word) and MSIZE = 01 (half-word) explicitly */
    DMAx->Stream[StreamID].CR &= ~((0x3U << 11) | (0x3U << 13)); // clear
    DMAx->Stream[StreamID].CR |= (1U << 11) | (1U << 13);         // PSIZE = 01, MSIZE = 01

    /* Select Block Size (total size) */
    DMAx->Stream[StreamID].NDTR = (uint32_t)BlockSize;

    /* Select Threshold (FTH bits 0..1 of FCR) */
    DMAx->Stream[StreamID].FCR &= ~0x3U;
    DMAx->Stream[StreamID].FCR |= (Threshold & 0x3U);
}

void MCAL_DMA_EnableStream(uint8_t StreamID){
    /* This function assumes DMA2 (your code uses DMA2 everywhere) */
    DMA2->Stream[StreamID].CR |= (1U << 0);
}

void MCAL_DMA_CallBack(uint8_t StreamID, void(*F_ptr)(void)){
    G_Fptr[StreamID] = F_ptr;
}

void DMA2_Stream0_IRQHandler(void){
    if (G_Fptr[0] != NULL){
	G_Fptr[0]();
    }
    /* Clear all Flags for Stream 0 (FE, DME, TE, HT, TC) */
    DMA2->LIFCR |= (1U<<0) | (1U<<2) | (1U<<3) | (1U<<4) | (1U<<5);
}

void DMA2_Stream1_IRQHandler(void){
    if (G_Fptr[1] != NULL) {
	G_Fptr[1]();
    }

    /* Clear all Flags for Stream 1 */
    DMA2->LIFCR |= (1U<<6) | (1U<<8) | (1U<<9) | (1U<<10) | (1U<<11);
}
