/*
 * stm32f401xc_FMI_driver.h
 *
 * Created on: Sep 24, 2025
 * Author: Ahmed Ezzat
 */

#ifndef INC_STM32F401XC_FMI_DRIVER_H_
#define INC_STM32F401XC_FMI_DRIVER_H_

//------------------------------
// Includes
//------------------------------
#include "stm32f401xc.h"

//============================================================

//---------------------------
// User Type Definitions
//---------------------------
typedef enum{
	SECTOR0,
	SECTOR1,
	SECTOR2,
	SECTOR3,
	SECTOR4,
	SECTOR5,
}Sector_t;

//============================================================

/*
 * =======================================================
 * 					  Generic Macros
 * =======================================================
 */
#define KEY1	0x45670123
#define KEY2	0xCDEF89AB

//============================================================

/*
 * =================================================
 * 		   APIs Supported by "FMI DRIVER"
 * =================================================
 */
void MCAL_FMI_MassErase(void);
void MCAL_FMI_SectorErase(Sector_t sector_num);
void MCAL_FMI_ProgramFlash(uint32_t Add, uint16_t *Data, uint16_t DataLen);

#endif /* INC_STM32F401XC_FMI_DRIVER_H_ */
