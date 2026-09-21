/*
 * stm32f103x6_gpio_driver.h
 *
 * Created on: Aug 10, 2025
 * Author: Ahmed Ezzat
 */

#ifndef STM32F401XC_GPIO_DRIVER_H_
#define STM32F401XC_GPIO_DRIVER_H_

//------------------------------
// Includes
//------------------------------
#include "stm32f401xc.h"

//============================================================

//------------------------------------------
// User Type Definitions (Structures)
//------------------------------------------
typedef struct{
	uint16_t GPIO_PinNumber;        // Specifies GPIO pin to configure
					// This Parameter can be set based on @ref GPIO_PIN_define

	uint16_t GPIO_MODE;	        // Specifies the Mode of GPIO
					// This Parameter can be value at @ref GPIO_MODE_define

	uint32_t GPIO_AFx;		// Specifies the Alternate Function Number
					// This Parameter can be value at @ref GPIO_AFx_define

	uint16_t GPIO_TYPE;	        // Specifies the Type of GPIO
					// This Parameter can be value at @ref GPIO_TYPE_define

	uint16_t GPIO_PU_PD;	        // Specifies if the GPIO is Pull-Up of Pull-Down
					// This Parameter can be value at @ref GPIO_PU_PD_define

	uint16_t GPIO_Output_Speed;	// Specifies the output speed of the GPIO
					// This Parameter can be value at @ref GPIO_SPEED_define
}GPIO_PinConfig_t;

//============================================================

// @ref GPIO_PIN_STATE
#define GPIO_PIN_SET					1
#define GPIO_PIN_RST					0

// @ref GPIO_RET_LOCK
#define GPIO_RET_LOCK_EN				1
#define GPIO_RET_LOCK_DIS				0

// Returned Wrong value
#define Wrong_RET_Val					-1

//============================================================

//------------------------------------------
// Macros Configuration References
//------------------------------------------

// @ref GPIO_PIN_define
#define GPIO_PIN_0								0
#define GPIO_PIN_1								1
#define GPIO_PIN_2								2
#define GPIO_PIN_3								3
#define GPIO_PIN_4								4
#define GPIO_PIN_5								5
#define GPIO_PIN_6								6
#define GPIO_PIN_7								7
#define GPIO_PIN_8								8
#define GPIO_PIN_9								9
#define GPIO_PIN_10								10
#define GPIO_PIN_11								11
#define GPIO_PIN_12								12
#define GPIO_PIN_13								13
#define GPIO_PIN_14								14
#define GPIO_PIN_15								15
#define GPIO_PIN_ALL								16

#define GPIO_PIN_MASK								(0x0000FFFFu)

// @ref GPIO_MODE_define
// 0: Input (reset state)
// 1: General purpose output mode
// 2: Alternate function mode
// 3: Analog mode
#define GPIO_MODE_IP								0x00000000u      // Input (reset state)
#define GPIO_MODE_OP								0x00000001u      // General purpose output mode
#define GPIO_MODE_AF								0x00000002u      // Alternate function mode
#define GPIO_MODE_Analog							0x00000003u      // Analog mode

// @ref GPIO_TYPE_define
// 0: Output push-pull (reset state)
// 1: Output open-drain
#define GPIO_TYPE_PP								0x00000000u      // Output push-pull (reset state)
#define GPIO_TYPE_OD								0x00000001u      // Output open-drain

// @ref GPIO_PU_PD_define
// 0: No pull-up, pull-down
// 1: Pull-up
// 2: Pull-down
// 3: Reserved
#define GPIO_PU_PD_NONE								0x00000000u	 // No pull-up, pull-down (Floating)
#define GPIO_PU_PD_PU								0x00000001u      // Pull-up
#define GPIO_PU_PD_PD								0x00000002u      // Pull-down

// @ref GPIO_AFx_define
#define GPIO_AF0								0
#define GPIO_AF1								1
#define GPIO_AF2								2
#define GPIO_AF3								3
#define GPIO_AF4								4
#define GPIO_AF5								5
#define GPIO_AF6								6
#define GPIO_AF7								7
#define GPIO_AF8								8
#define GPIO_AF9								9
#define GPIO_AF10								10
#define GPIO_AF11								11
#define GPIO_AF12								12
#define GPIO_AF13								13
#define GPIO_AF14								14
#define GPIO_AF15								15

// @ref GPIO_SPEED_define
// 0: Low speed
// 1: Medium speed
// 2: High speed
// 3: Very high speed
#define GPIO_SPEED_LOW								0x00000000u      // Low speed
#define GPIO_SPEED_MEDIUM 							0x00000001u      // Medium speed
#define GPIO_SPEED_HIGH								0x00000002u      // High speed
#define GPIO_SPEED_VERY_HIGH							0x00000003u      // Very high speed

//============================================================

/*
 * ==================================================
 * 		 APIs Supported by "MCAL GPIO DRIVER"
 * ==================================================
 */
void MCAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_PinConfig_t *PinConfig);
void MCAL_GPIO_Denit(GPIO_TypeDef *GPIOx);


uint8_t MCAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t PinNumber);
uint16_t MCAL_GPIO_ReadPort(GPIO_TypeDef *GPIOx);

void MCAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t PinNumber, uint16_t value);
void MCAL_GPIO_WritePinAtomic(GPIO_TypeDef *GPIOx, uint16_t PinNumber, uint16_t value);
void MCAL_GPIO_WritePort(GPIO_TypeDef *GPIOx, uint16_t value);

void MCAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t PinNumber);

uint8_t MCAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t PinNumber);

#endif /* STM32F401XC_GPIO_DRIVER_H_ */
