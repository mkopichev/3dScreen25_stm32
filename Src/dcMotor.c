#include "../Inc/dcMotor.h"

extern uint8_t cpuFreqSourceChoosen;

// PWM pin PA6 TIM3_CH1, enable pin PA7

void dcMotorPwmInit(void) {

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
	GPIOA->CRL |= GPIO_CRL_MODE6_0 | GPIO_CRL_CNF6_1;
	GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
	GPIOA->CRL |= GPIO_CRL_MODE7;
	dcMotorStop();
	switch (cpuFreqSourceChoosen) { // TIM3 is 36 MHz MAX
	case HSI_SOURCE:
		TIM3->PSC = 16 - 1;
		break;
	case HSE_SOURCE:
		TIM3->PSC = 16 - 1;
		break;
	case PLL_SOURCE:
		TIM3->PSC = 72 - 1;
		break;
	}
	TIM3->ARR = 100;
	TIM3->CCER |= TIM_CCER_CC1E;
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	TIM3->CCR1 = 25;
	TIM3->CR1 |= TIM_CR1_CEN;
}

void dcMotorRun(void) {

	GPIOA->BSRR = GPIO_BSRR_BS7;
}

void dcMotorStop(void) {

	GPIOA->BSRR = GPIO_BSRR_BR7;
}
