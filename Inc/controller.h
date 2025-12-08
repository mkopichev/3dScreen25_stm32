#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "common.h"

typedef struct Controller_t {

	float kp, ki, kd;
	float integral;
	float prevError;
	float outMin;
	float outMax;
	float dt;
	float integralLim;
} Controller_t;

void controllerInit(Controller_t *controller, float kp, float ki, float kd,
		float min, float max, float deltaTime);
float controllerUpdate(Controller_t *controller, float setpoint,
		float measurement);

#endif /* CONTROLLER_H_ */
