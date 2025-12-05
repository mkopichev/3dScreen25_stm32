#include "../Inc/encoder.h"

volatile uint16_t dataArray[64];

void encoderTimInit(void) {

	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA1_Channel5->CCR &= ~DMA_CCR_EN;
	DMA1_Channel5->CNDTR = 64;
	DMA1_Channel5->CPAR = (uint32_t) & TIM2->CCR1;
	DMA1_Channel5->CMAR = (uint32_t) dataArray;
	NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	DMA1_Channel5->CCR = (1 << DMA_CCR_PL_Pos) | (1 << DMA_CCR_MSIZE_Pos)
			| DMA_CCR_HTIE | DMA_CCR_TCIE | (1 << DMA_CCR_PSIZE_Pos)
			| DMA_CCR_MINC | DMA_CCR_CIRC | DMA_CCR_EN;

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 720 - 1; // 200 kHz
	TIM2->ARR = 50000; // 2 Hz, 250 ms period of timer 2
	TIM2->CCMR1 = (1 << TIM_CCMR1_CC1S_Pos);
	TIM2->CCER = TIM_CCER_CC1E;
	TIM2->DIER = TIM_DIER_CC1DE | TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM2->CR1 = TIM_CR1_CEN;
}

uint16_t encoderPeriod = 0;
uint8_t flag = 0;

void DMA1_Channel5_IRQHandler(void) {

	uint32_t averageTmpValue = 0;
	if (DMA1->ISR & DMA_ISR_HTIF5) {

		DMA1->IFCR = DMA_IFCR_CHTIF5;

		averageTmpValue +=
				((int32_t) dataArray[0] - (int32_t) dataArray[63]) < 0 ?
						50000 + dataArray[0] - dataArray[63] :
						dataArray[0] - dataArray[63];

		for (uint8_t i = 1; i < 32; i++) {

			averageTmpValue +=
					((int32_t) dataArray[i] - (int32_t) dataArray[i - 1]) < 0 ?
							50000 + dataArray[i] - dataArray[i - 1] :
							dataArray[i] - dataArray[i - 1];
		}
	}

	if (DMA1->ISR & DMA_ISR_TCIF5) {

		DMA1->IFCR = DMA_IFCR_CTCIF5;

		for (uint8_t i = 32; i < 64; i++) {

			averageTmpValue +=
					((int32_t) dataArray[i] - (int32_t) dataArray[i - 1]) < 0 ?
							50000 + dataArray[i] - dataArray[i - 1] :
							dataArray[i] - dataArray[i - 1];
		}
	}
	flag = 0;
	encoderPeriod = averageTmpValue >> 5;
}

float encoderGetRPM(void) {
	if(encoderPeriod == 0) return 0.0f;
	return 60.0f / (encoderPeriod * 0.00001f * 11 * 4.4f);
}

void TIM2_IRQHandler(void) {
	TIM2->SR = ~TIM_SR_UIF;
	if(flag) {
		encoderPeriod = 0;
	}
	flag = 1;

}
