#include "common.h"

extern uint8_t cpuFreqSourceChoosen;
Controller_t dcMotCtrl;
float rpm;

int main(void) {

	if (!cpuFreqInit(PLL_SOURCE)) {

		NVIC_SystemReset();
		cpuFreqSourceChoosen = HSI_SOURCE;
	}
	userLedInit();
	dcMotorInit();
	encoderInit();
	controllerInit(&dcMotCtrl, 0.05f, 0.3f, 0.0f, 0.0f, 100.0f,
			10.0f / 1000.0f);
	controlSystemInit();
	onButtonInit();
	for (;;) {

		rpm = encoderGetRPM();
		userLedToggle(LED_ON);
		delayMs((uint16_t) rpm / 2);
		userLedToggle(LED_OFF);
		delayMs((uint16_t) rpm / 2);
	}
}
