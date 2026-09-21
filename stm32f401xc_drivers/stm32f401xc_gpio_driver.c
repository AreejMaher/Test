/*
 * stm32f103x6_gpio_driver.c
 *
 *  Created on: Aug 10, 2025
 *  Author: Ahmed Ezzat
 */

//------------------------------
// Includes
//------------------------------
#include "stm32f401xc_gpio_driver.h"

//============================================================

/*
 * ========================================================================
 * 				 APIs Supported by "MCAL GPIO DRIVER"
 * ========================================================================
 */

/**================================================================
  * @Fn				- MCAL_GPIO_Init
  * @brief 			- Initializes GPIOx PINy
  * @param [in]  	- GPIOx: x can be (A....E depends on device) to select GPIO Peripherals
  * @param [in] 	- pinConfig: pointer to GPIO_PinConfig_t structure that contains the config info for pin
  * @retval 		- none
  * Note 			- stm32f103xx have GPIO A,B,C,D,E but this package has only A and B fully and some of C and D
  */
void MCAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_PinConfig_t *PinConfig){
    uint32_t pin = PinConfig->GPIO_PinNumber;

    // Clear old data from registers
    GPIOx->MODER   &= ~(0x3 << (2 * pin));
    GPIOx->OTYPER  &= ~(1   << pin);
    GPIOx->OSPEEDR &= ~(0x3 << (2 * pin));
    GPIOx->PUPDR   &= ~(0x3 << (2 * pin));

    // Set new config
    switch (PinConfig->GPIO_MODE)
    {
    case GPIO_MODE_OP:   // General purpose output
        GPIOx->MODER   |= (0x1 << (2 * pin));   // 01
        GPIOx->OTYPER  |= (PinConfig->GPIO_TYPE << pin);
        GPIOx->OSPEEDR |= (PinConfig->GPIO_Output_Speed << (2 * pin));
        GPIOx->PUPDR   |= (PinConfig->GPIO_PU_PD << (2 * pin));
        break;

    case GPIO_MODE_AF:   // Alternate function
        GPIOx->MODER   |= (0x2 << (2 * pin));   // 10
        GPIOx->OTYPER  |= (PinConfig->GPIO_TYPE << pin);
        GPIOx->OSPEEDR |= (PinConfig->GPIO_Output_Speed << (2 * pin));
        GPIOx->PUPDR   |= (PinConfig->GPIO_PU_PD << (2 * pin));

        // Configure AF (AFRL [0..7], AFRH [8..15])
        if (pin < 8) {
            GPIOx->AFR[0] &= ~(0xF << (4 * pin));
            GPIOx->AFR[0] |= (PinConfig->GPIO_AFx << (4 * pin));
        }
        else if((pin >= 8) && (pin < 16)){
            GPIOx->AFR[1] &= ~(0xF << (4 * (pin - 8)));
            GPIOx->AFR[1] |= (PinConfig->GPIO_AFx << (4 * (pin - 8)));
        }
        break;

    case GPIO_MODE_Analog:   // Analog
        GPIOx->MODER   |= (0x3 << (2 * pin));   // 11
        GPIOx->PUPDR   |= (PinConfig->GPIO_PU_PD << (2 * pin));
        break;

    case GPIO_MODE_IP:   // Input
    default:
        // Input mode (00)
        GPIOx->PUPDR   |= (PinConfig->GPIO_PU_PD << (2 * pin));
        break;
    }
}

/**================================================================
  * @Fn				- MCAL_GPIO_Reset
  * @brief 			- Reset all GPIOx regesters
  * @param [in]  	- GPIOx: x can be (A....E depends on device) to select GPIO Peripherals
  * @retval 		- None
  * Note 			- None
  */
void MCAL_GPIO_Denit(GPIO_TypeDef *GPIOx){
	if(GPIOx == GPIOA){
		RCC_GPIOA_CLK_RST_SET();
	}
	else if(GPIOx == GPIOB){
		RCC_GPIOB_CLK_RST_SET();
	}
	else if(GPIOx == GPIOC){;
		RCC_GPIOC_CLK_RST_SET();
	}
	else if(GPIOx == GPIOD){
		RCC_GPIOD_CLK_RST_SET();
	}
	else if(GPIOx == GPIOE){
		RCC_GPIOE_CLK_RST_SET();
	}
}

/**================================================================
  * @brief 			- Read specific pin
  * @param [in]  	- GPIOx: x can be (A....E depends on device) to select GPIO Peripherals
  * @param [in]  	- pinNo: can be (0...15 depends on GPIOx) to select pin number
  * @retval 		- the input value (two values based on @ref GPIO_PIN_state)
  * Note 			- None
  */
uint8_t MCAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t PinNumber){
	return ( (GPIOx->IDR & (1 << PinNumber)) ? GPIO_PIN_SET : GPIO_PIN_RST );
}

/**================================================================
  * @Fn				- MCAL_GPIO_READPORT
  * @brief 			- Read all pins in the port
  * @param [in]  	- GPIOx: x can be (A....E depends on device) to select GPIO Peripherals
  * @retval 		- the input value for all the port (two values for each pin of the port based on @ref GPIO_PIN_state )
  * Note 			- None
  */
uint16_t MCAL_GPIO_ReadPort(GPIO_TypeDef *GPIOx){
	uint16_t PortStatus;
	PortStatus = GPIOx->IDR;
	return PortStatus;
}

/**================================================================
  * @Fn				- MCAL_GPIO_WRITEPIN
  * @brief 			- Write on specific pin
  * @param [in]  	- GPIOx: x can be (A....E depends on device) to select GPIO Peripherals
  * @param [in]  	- pinNo: can be (0...15 depends on GPIOx) to select pin number
  * @param [in]  	- Value: the sate of the pin (can be two values based on @ref GPIO_PIN_state)
  * @retval 		- None
  * Note 			- None
  */
void MCAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t PinNumber, uint16_t value){
	if(value == GPIO_PIN_SET){
		// BSy: Port x Set bit y (y= 0 .. 15)
		// These bits are write-only and can be accessed in Word mode only.
		// 0: No action on the corresponding ODRx bit
		// 1: Set the corresponding ODRx bit
        GPIOx->ODR |= (1 << PinNumber);
	}
	else{
		// BRy: Port x Reset bit y (y= 0 .. 15)
		// These bits are write-only and can be accessed in Word mode only.
		// 0: No action on the corresponding ODRx bit
		// 1: Reset the corresponding ODRx bit
        GPIOx->ODR &= ~(1 << PinNumber);
	}
}

/**================================================================
  * @Fn				- MCAL_GPIO_WritePinAtomic
  * @brief 			- Write on specific pin without reading or change whole port
  * @param [in]  	- GPIOx: x can be (A....E depends on device) to select GPIO Peripherals
  * @param [in]  	- pinNo: can be (0...15 depends on GPIOx) to select pin number
  * @param [in]  	- Value: the sate of the pin (can be two values based on @ref GPIO_PIN_state)
  * @retval 		- None
  * Note 			- None
  */
void MCAL_GPIO_WritePinAtomic(GPIO_TypeDef *GPIOx, uint16_t PinNumber, uint16_t value){
	if(HIGH == value){
		if(GPIOx == GPIOA){
			GPIOA->BSRR = 1 << PinNumber;
		}
		else if(GPIOx == GPIOB){
			GPIOB->BSRR = 1 << PinNumber;
		}
		else if(GPIOx == GPIOC){
			GPIOC->BSRR = 1 << PinNumber;
		}

	}
	else if(LOW == value){
		if(GPIOx == GPIOA){
			GPIOA->BSRR = 1 << (PinNumber + 16);
		}
		else if(GPIOx == GPIOB){
			GPIOB->BSRR = 1 << (PinNumber + 16);
		}
		else if(GPIOx == GPIOC){
			GPIOC->BSRR = 1 << (PinNumber + 16);
		}
	}
}

/**================================================================
  * @Fn				- MCAL_GPIO_WRITEPORT
  * @brief 			- Write on all pins in the port
  * @param [in]  	- GPIOx: x can be (A....E depends on device) to select GPIO Peripherals
  * @param [in]  	- Value: the sate of all pins of specific port (can be two values for each pin of the port based on @ref GPIO_PIN_state)
  * @retval 		- None
  * Note 			- None
  */
void MCAL_GPIO_WritePort(GPIO_TypeDef *GPIOx, uint16_t value){
	GPIOx->ODR = (uint32_t) value;
}

/**================================================================
  * @Fn				- MCAL_GPIO_TOGGLEPIN
  * @brief 			- Toggle on specific pin
  * @param [in]  	- GPIOx: x can be (A....E depends on device) to select GPIO Peripherals
  * @param [in]  	- pinNo: can be (0...15 depends on GPIOx) to select pin number
  * @retval 		- None
  * Note 			- None
  */
void MCAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t PinNumber){
	GPIOx->ODR ^= (uint32_t)(1 << PinNumber);
}

/**================================================================
  * @Fn				- MCAL_GPIO_LOCKPIN
  * @brief 			- Lock specific pin
  * @param [in]  	- GPIOx: x can be (A....E depends on device) to select GPIO Peripherals
  * @param [in]  	- pinNo: can be (0...15 depends on GPIOx) to select pin number
  * @retval 		- Enable if pin config is locked or Error if pin is not locked (Ok and Error are based on @ref GPIO_PIN_LOCK_state)
  * Note 			- None
  */
uint8_t MCAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t PinNumber){
	//	Bit 16 LCKK[16]: Lock key
	//	This bit can be read anytime. It can only be modified using the Lock Key Writing Sequence.
	//	0: Port configuration lock key not active
	//	1: Port configuration lock key active. GPIOx_LCKR register is locked until the next reset.
	//	LOCK key writing sequence:
	//	Write 1
	//	Write 0
	//	Write 1
	//	Read 0
	//	Read 1 (this read is optional but confirms that the lock is active)
	//	Note: During the LOCK Key Writing sequence, the value of LCK[15:0] must not change.
	//	Any error in the lock sequence will abort the lock.
	//	Bits 15:0 LCKy: Port x Lock bit y (y= 0 .. 15)
	//	These bits are read write but can only be written when the LCKK bit is 0.
	//	0: Port configuration not locked
	//	1: Port configuration locked.

	// Set the LCKK[16]
	volatile uint32_t tmp = (1 << 16);

	// Set the LCKy
	tmp |= PinNumber;

	// Write 1
	GPIOx->LCKR = tmp;

	// Write 0
	GPIOx->LCKR = PinNumber;

	// Write 1
	GPIOx->LCKR = tmp;

	// Read 0
	tmp = GPIOx->LCKR;

	if((uint32_t)((GPIOx->LCKR) & (1 << 16))){
		return GPIO_RET_LOCK_EN;
	}
	else{
		return GPIO_RET_LOCK_DIS;
	}
}
