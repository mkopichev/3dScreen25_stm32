#include "../Inc/onButon.h"

// Button is connected to PB11 pin and commutates GND.

extern uint16_t softStartSetpoint;
extern float pidOutput;
extern Controller_t dcMotCtrl;

void onButtonInit(void) {

	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
	GPIOB->CRH &= ~GPIO_CRH_MODE11;
	GPIOB->CRH &= ~GPIO_CRH_CNF11;
	GPIOB->CRH |= GPIO_CRH_CNF11_1;
	GPIOB->ODR |= GPIO_ODR_ODR11;
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI11_PB;
	EXTI->IMR |= EXTI_IMR_IM11;
	EXTI->FTSR |= EXTI_FTSR_FT11;
	EXTI->RTSR |= EXTI_RTSR_RT11;
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	EXTI->SWIER |= EXTI_SWIER_SWIER11;
}

void EXTI15_10_IRQHandler(void) {

	static bool onButtonInitialState = true;

	if (EXTI->PR & EXTI_PR_PIF11) {

		EXTI->PR |= EXTI_PR_PIF11;
		if ((GPIOB->IDR & GPIO_IDR_IDR11) && onButtonInitialState) {

			dcMotorStop();
			return;
		} else {

			onButtonInitialState = false;
			if (GPIOB->IDR & GPIO_IDR_IDR11) {

				NVIC_EnableIRQ(TIM4_IRQn);
			} else {

				softStartSetpoint = 0;
				dcMotorStop();
				controllerInit(&dcMotCtrl, 0.05f, 0.3f, 0.0f, 0.0f, 100.0f,
						10.0f / 1000.0f);
				NVIC_DisableIRQ(TIM4_IRQn);
			}
		}
	}
}
