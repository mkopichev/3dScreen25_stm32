#include "../Inc/controlSystem.h"

extern uint8_t cpuFreqSourceChoosen;
extern Controller_t dcMotCtrl;

float targetRPM = 300.0f;

void controlSystemInit(void) {

	// APB1 prescaler is 2, so TIM4 frequency is x2
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	switch (cpuFreqSourceChoosen) {
	case HSI_SOURCE:
		TIM4->PSC = 80 - 1;
		break;
	case HSE_SOURCE:
		TIM4->PSC = 80 - 1;
		break;
	case PLL_SOURCE:
		TIM4->PSC = 720 - 1; // 100 kHz
		break;
	}
	TIM4->ARR = 1000; // 10 ms period
	TIM4->DIER = TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM4_IRQn);
	TIM4->CR1 = TIM_CR1_CEN;
}

void TIM4_IRQHandler(void) {

	TIM4->SR &= ~TIM_SR_UIF;
	float pidOutput = controllerUpdate(&dcMotCtrl, targetRPM, encoderGetRPM());
	dcMotorRun((uint8_t)pidOutput);
}
