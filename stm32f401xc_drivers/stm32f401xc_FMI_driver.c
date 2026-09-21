/*
 * stm32f401xc_FMI_driver.c
 *
 * Created on: Sep 24, 2025
 * Author: Ahmed Ezzat
 */

//------------------------------
// Includes
//------------------------------
#include "stm32f401xc_FMI_driver.h"

//============================================================

/*
 * =================================================
 * 		   APIs Supported by "FMI DRIVER"
 * =================================================
 */
void MCAL_FMI_MassErase(void){
	// Wait on Busy Flag
	// Bit 16 BSY: Busy
	//	0: no Flash memory operation ongoing
	//	1: Flash memory operation ongoing
	while(GET_BIT(FMI->SR, 16));

	// If Locked CR Register -> unlock
	// Bit 31 LOCK: Lock
	/* Write to 1 only. When it is set, this bit indicates that the FLASH_CR register is locked. It is
	cleared by hardware after detecting the unlock sequence. */
	if(GET_BIT(FMI->CR, 31)){
		// Bits 31:0 FKEYR[31:0]: FPEC key
		//	a) KEY1 = 0x45670123
		//	b) KEY2 = 0xCDEF89AB
		FMI->KEYR = KEY1;
		FMI->KEYR = KEY2;
	}

	// Set The MER Bit in The FLASH_CR
	// Bit 2 MER: Mass Erase
	/*  Erase activated for all user sectors. */
	FMI->CR |= (1 << 2);

	// Set The STRT Bit in The FLASH_CR
	// Bit 16 STRT: Start
	/* This bit triggers an erase operation when set. It is set only by software and cleared when the
	BSY bit is cleared. */
	FMI->CR |= (1 << 16);

	// Wait For The BSY Bit to be Cleared
	while(GET_BIT(FMI->SR, 16));

	// Turn off Mass Erase
	FMI->CR &= ~(1 << 2);
}

void MCAL_FMI_SectorErase(Sector_t sector_num){
	// Wait on Busy Flag
	// Bit 16 BSY: Busy
	//	0: no Flash memory operation ongoing
	//	1: Flash memory operation ongoing
	while(GET_BIT(FMI->SR, 16));

	// If Locked CR Register -> unlock
	// Bit 31 LOCK: Lock
	/* Write to 1 only. When it is set, this bit indicates that the FLASH_CR register is locked. It is
	cleared by hardware after detecting the unlock sequence. */
	if(GET_BIT(FMI->CR, 31)){
		// Bits 31:0 FKEYR[31:0]: FPEC key
		//	a) KEY1 = 0x45670123
		//	b) KEY2 = 0xCDEF89AB
		FMI->KEYR = KEY1;
		FMI->KEYR = KEY2;
	}

	// Select sector to Erase
	// Bits 6:3 SNB[3:0]: Sector number
	/* These bits select the sector to erase. */
	//	0000 sector 0
	//	0001 sector 1
	//	...
	FMI->CR &= ~(0xF << 3);
	FMI->CR |= (sector_num << 3);

	// Bit 1 SER: Sector Erase
	/* Sector Erase activated. */
	FMI->CR |= (1 << 1);

	// Set The STRT Bit in The FLASH_CR
	// Bit 16 STRT: Start
	/* This bit triggers an erase operation when set. It is set only by software and cleared when the
	BSY bit is cleared. */
	FMI->CR |= (1 << 16);

	// Wait For The BSY Bit to be Cleared
	while(GET_BIT(FMI->SR, 16));

	// Turn off Sector Erase
	FMI->CR &= ~(1 << 1);
}

void MCAL_FMI_ProgramFlash(uint32_t Add, uint16_t *Data, uint16_t DataLen){
	// Wait on Busy Flag
	// Bit 16 BSY: Busy
	//	0: no Flash memory operation ongoing
	//	1: Flash memory operation ongoing
	while(GET_BIT(FMI->SR, 16));

	// If Locked CR Register -> unlock
	// Bit 31 LOCK: Lock
	/* Write to 1 only. When it is set, this bit indicates that the FLASH_CR register is locked. It is
	cleared by hardware after detecting the unlock sequence. */
	if(GET_BIT(FMI->CR, 31)){
		// Bits 31:0 FKEYR[31:0]: FPEC key
		//	a) KEY1 = 0x45670123
		//	b) KEY2 = 0xCDEF89AB
		FMI->KEYR = KEY1;
		FMI->KEYR = KEY2;
	}

	// Select Width (16 bit) to be Programmed
	// Bits 9:8 PSIZE[1:0]: Program size
	/* These bits select the program parallelism. */
	//	00 program x8
	//	01 program x16
	//	10 program x32
	//	11 program x64
	FMI->CR &= ~(0x3 << 8);
	FMI->CR |= (1 << 8);

	// Select Programming Mode
	// Bit 0 PG: Programming
	/* Flash programming activated. */
	FMI->CR |= (1 << 0);

	for(uint16_t i=0; i < DataLen; i++){
		// Save Data in Address
		*((volatile uint16_t *)Add) = Data[i];
		Add += 2;

		// Wait on Busy Flag
		while(GET_BIT(FMI->SR, 16));
	}

	// Stop Programming Mode
	FMI->CR &= ~(1 << 0);
}
