#include "common.h"

Controller_t dcMotCtrl;

float rpm;

int main(void) {

	if (!cpuFreqInit(PLL_SOURCE)) {

		NVIC_SystemReset();
	}
	userLedInit();
	dcMotorInit();
	encoderInit();
	controllerInit(&dcMotCtrl, 0.05f, 0.3f, 0.0f, 0.0f, 100.0f,
			10.0f / 1000.0f);

	controlSystemInit();

	for (;;) {

		rpm = encoderGetRPM();
		userLedToggle(LED_ON);
		delayMs(1000);
		userLedToggle(LED_OFF);
		delayMs(1000);
	}
}
