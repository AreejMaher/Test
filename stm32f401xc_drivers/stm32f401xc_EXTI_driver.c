/*
 * stm32f401xc_EXTI_driver.c
 *
 * Created on: Aug 19, 2025
 * Author: Ahmed Ezzat
 */

//------------------------------
// Includes
//------------------------------
#include "stm32f401xc_EXTI_driver.h"
#include "stm32f401xc_gpio_driver.h"

//============================================================

/*
 * =======================================================
 * 					  Generic Variables
 * =======================================================
 */

void(* G_IRQ_CallBack[16])(void) = {NULL};

//============================================================

/*
 * =======================================================================================
 * 							Generic Macros
 * =======================================================================================
 */
#define AFIO_GPIO_EXTI_Mapping(x)			((x==GPIOA)? 0 : \
											 (x==GPIOB)? 1 : \
											 (x==GPIOC)? 2 : \
											 (x==GPIOD)? 3 : -1)

//============================================================

/*
 * =======================================================
 * 					  Helper Functions
 * =======================================================
 */
void Enable_NVIC(uint16_t IRQ){
	switch(IRQ){
		case 0:	 NVIC_IRQ6_EXTI0_EN()  	 	; break;
		case 1:  NVIC_IRQ7_EXTI1_EN()  	 	; break;
		case 2:	 NVIC_IRQ8_EXTI2_EN() 		; break;
		case 3:	 NVIC_IRQ9_EXTI3_EN()    	; break;
		case 4:	 NVIC_IRQ10_EXTI4_EN() 	    ; break;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:  NVIC_IRQ23_EXTI_5_9_EN()   ; break;
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15: NVIC_IRQ40_EXTI_10_15_EN() ; break;
	}
}

void Disable_NVIC(uint16_t IRQ){
	switch(IRQ){
		case 0:	 NVIC_IRQ6_EXTI0_DIS()  	 ; break;
		case 1:  NVIC_IRQ7_EXTI1_DIS()  	 ; break;
		case 2:	 NVIC_IRQ8_EXTI2_DIS() 		 ; break;
		case 3:	 NVIC_IRQ9_EXTI3_DIS()    	 ; break;
		case 4:	 NVIC_IRQ10_EXTI4_DIS() 	 ; break;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:  NVIC_IRQ23_EXTI_5_9_DIS()   ; break;
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15: NVIC_IRQ40_EXTI_10_15_DIS() ; break;
	}
}

void Set_EXTI(EXTI_PinConfig_t *EXTI_PinConfig){
	// Configure EXTI Pin to be AF IP Floating
	GPIO_PinConfig_t EXTI_Pin;
	EXTI_Pin.GPIO_PinNumber = EXTI_PinConfig->EXTI_PIN.GPIO_PinNumber;
	EXTI_Pin.GPIO_MODE = GPIO_MODE_IP;
	EXTI_Pin.GPIO_PU_PD = GPIO_PU_PD_NONE;
	MCAL_GPIO_Init((EXTI_PinConfig->EXTI_PIN.GPIOx), &EXTI_Pin);

	// Update SYSCFG to route EXTI between port A B C
	uint8_t SYSCFG_EXTI_Index = (EXTI_PinConfig->EXTI_PIN.EXTI_LineNumber) / 4;
	uint8_t SYSCFG_EXTI_Position = ((EXTI_PinConfig->EXTI_PIN.EXTI_LineNumber) % 4) * 4;

	// Clear the 4-bits before set
	SYSCFG->EXTICR[SYSCFG_EXTI_Index] &= ~(0xF << SYSCFG_EXTI_Position);
	SYSCFG->EXTICR[SYSCFG_EXTI_Index] |= ((AFIO_GPIO_EXTI_Mapping(EXTI_PinConfig->EXTI_PIN.GPIOx) & 0xF) << SYSCFG_EXTI_Position);

	// Update Trigger selection
	// Clear them before set
	EXTI->RTSR &= ~(1 << EXTI_PinConfig->EXTI_PIN.EXTI_LineNumber);
	EXTI->FTSR &= ~(1 << EXTI_PinConfig->EXTI_PIN.EXTI_LineNumber);

	if(EXTI_TRIGGER_RISING == (EXTI_PinConfig->EXTI_EdgeSelect)){
		EXTI->RTSR |= (1 << EXTI_PinConfig->EXTI_PIN.EXTI_LineNumber);
	}
	else if(EXTI_TRIGGER_FALLING == (EXTI_PinConfig->EXTI_EdgeSelect)){
		EXTI->FTSR |= (1 << EXTI_PinConfig->EXTI_PIN.EXTI_LineNumber);
	}
	else if(EXTI_TRIGGER_RISING_FALLING == (EXTI_PinConfig->EXTI_EdgeSelect)){
		EXTI->RTSR |= (1 << EXTI_PinConfig->EXTI_PIN.EXTI_LineNumber);
		EXTI->FTSR |= (1 << EXTI_PinConfig->EXTI_PIN.EXTI_LineNumber);
	}

	// Update IRQ CallBack function
	G_IRQ_CallBack[EXTI_PinConfig->EXTI_PIN.EXTI_LineNumber] = EXTI_PinConfig->P_IRQ_CallBack;

	// Enable/Disable IRQ EXTI & NVIC
	if(EXTI_IRQ_EN == (EXTI_PinConfig->EXTI_Enable)){
		EXTI->IMR |= (1 << EXTI_PinConfig->EXTI_PIN.EXTI_LineNumber);
		Enable_NVIC(EXTI_PinConfig->EXTI_PIN.EXTI_LineNumber);
	}
	else{
		EXTI->IMR &= ~(1 << EXTI_PinConfig->EXTI_PIN.EXTI_LineNumber);
		Disable_NVIC(EXTI_PinConfig->EXTI_PIN.EXTI_LineNumber);
	}
}

//============================================================

/*
 * ========================================================================
 * 				 APIs Supported by "MCAL EXTI GPIO DRIVER"
 * ========================================================================
 */

/**================================================================
 * @Fn				-MCAL_EXTI_GPIO_Init
 * @brief 			-this is used to inialize EXTI from SpecifiC GPIO PIN and specify the Mask/Trigger Condition and IRQ CallBack
 * @param [in]   	-EXTI_Config set by @ref EXTI_define, EXTI_Trigger_define and EXTI_IRQ_define
 * @retval 			-none
 * Note				-Stm32F103C6 MCU has GPIO A,B,C,D,E Modules
 * 				 	But LQFP48 Package has only GPIO A,B,PART of C/D exported as external PINS from the MCU
 * 				 	- Also Mandatory to Enable RCC Clock for AFIO and the corresponding GPIO
 */
void MCAL_EXTI_GPIO_Init(EXTI_PinConfig_t *EXTI_PinConfig){
	Set_EXTI(EXTI_PinConfig);
}

/**================================================================
 * @Fn					-MCAL_EXTI_GPIO_DeInit
 * @brief 				-reset  EXTI Registers and NVIC corresponding IRQ Mask
 * @retval 				-none
 * Note					-none
 */
void MCAL_EXTI_GPIO_DeInit(GPIO_TypeDef *GPIOx){
	// Clear IMR Register
	EXTI->IMR = 0x00000000;

	// Clear EMR Register
	EXTI->EMR = 0x00000000;

	// Clear RTSR Register
	EXTI->RTSR = 0x00000000;

	// Clear FTSR Register
	EXTI->FTSR = 0x00000000;

	// Clear SWIER Register
	EXTI->SWIER = 0x00000000;

	// Clear PR Register
	EXTI->PR = 0xFFFFFFFF;

	// Disable IRQ From NVIC
	NVIC_IRQ6_EXTI0_DIS();
	NVIC_IRQ7_EXTI1_DIS();
	NVIC_IRQ8_EXTI2_DIS();
	NVIC_IRQ9_EXTI3_DIS();
	NVIC_IRQ10_EXTI4_DIS();
	NVIC_IRQ23_EXTI_5_9_DIS();
	NVIC_IRQ40_EXTI_10_15_DIS();
}

/**================================================================
 * @Fn				-MCAL_EXTI_GPIO_Update
 * @brief 			-this is used to inialize EXTI from SpecifiC GPIO PIN and specify the Mask/Trigger Condition and IRQ CallBack
 * @param [in]   	-EXTI_Config set by @ref EXTI_define, EXTI_Trigger_define and EXTI_IRQ_define
 * @retval 			-none
 * Note				-Stm32F103C6 MCU has GPIO A,B,C,D,E Modules
 * 				 	But LQFP48 Package has only GPIO A,B,PART of C/D exported as external PINS from the MCU
 * 				 	- Also Mandatory to Enable RCC Clock for AFIO and the corresponding GPIO
 */
void MCAL_EXTI_GPIO_Update(EXTI_PinConfig_t *EXTI_PinConfig){
	Set_EXTI(EXTI_PinConfig);
}

//============================================================

/*
 * ===================================
 * 			 ISR  Functions
 * ===================================
 */
void EXTI0_IRQHandler(void){
	// Clear PR Register by writing 1 in it
	EXTI->PR |= (1 << 0);
	G_IRQ_CallBack[0]();
}

void EXTI1_IRQHandler(void){
	// Clear PR Register by writing 1 in it
	EXTI->PR |= (1 << 1);
	G_IRQ_CallBack[1]();
}

void EXTI2_IRQHandler(void){
	// Clear PR Register by writing 1 in it
	EXTI->PR |= (1 << 2);
	G_IRQ_CallBack[2]();
}

void EXTI3_IRQHandler(void){
	// Clear PR Register by writing 1 in it
	EXTI->PR |= (1 << 3);
	G_IRQ_CallBack[3]();
}

void EXTI4_IRQHandler(void){
	// Clear PR Register by writing 1 in it
	EXTI->PR |= (1 << 4);
	G_IRQ_CallBack[4]();
}

void EXTI9_5_IRQHandler(void){
	if (EXTI->PR & 1 << 5 ) {	EXTI->PR |=  (1 << 5)    ; G_IRQ_CallBack[5]() ;   }
	if (EXTI->PR & 1 << 6 ) {	EXTI->PR |=  (1 << 6)    ; G_IRQ_CallBack[6]() ;   }
	if (EXTI->PR & 1 << 7 ) {	EXTI->PR |=  (1 << 7)    ; G_IRQ_CallBack[7]() ;   }
	if (EXTI->PR & 1 << 8 ) {	EXTI->PR |=  (1 << 8)    ; G_IRQ_CallBack[8]() ;   }
	if (EXTI->PR & 1 << 9 ) {	EXTI->PR |=  (1 << 9)    ; G_IRQ_CallBack[9]() ;   }
}

void EXTI15_10_IRQHandler(void){
	if (EXTI->PR & 1 << 10 ) {	EXTI->PR |=  (1 << 10)    ; G_IRQ_CallBack[10]() ;   }
	if (EXTI->PR & 1 << 11 ) {	EXTI->PR |=  (1 << 11)    ; G_IRQ_CallBack[11]() ;   }
	if (EXTI->PR & 1 << 12 ) {	EXTI->PR |=  (1 << 12)    ; G_IRQ_CallBack[12]() ;   }
	if (EXTI->PR & 1 << 13 ) {	EXTI->PR |=  (1 << 13)    ; G_IRQ_CallBack[13]() ;   }
	if (EXTI->PR & 1 << 14 ) {	EXTI->PR |=  (1 << 14)    ; G_IRQ_CallBack[14]() ;   }
	if (EXTI->PR & 1 << 15 ) {	EXTI->PR |=  (1 << 15)    ; G_IRQ_CallBack[15]() ;   }
}
