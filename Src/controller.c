#include "../Inc/controller.h"

void controllerInit(Controller_t *controller, float kp, float ki, float kd,
		float min, float max, float deltaTime) {

	controller->kp = kp;
	controller->ki = ki;
	controller->kd = kd;
	controller->integral = 0.0f;
	controller->prevError = 0.0f;
	controller->outMin = min;
	controller->outMax = max;
	controller->dt = deltaTime;
	controller->integralLim = 2.0f * max;
}

float controllerUpdate(Controller_t *controller, float setpoint,
		float measurement) {

	float error = setpoint - measurement;
	// proportional part
	float P = controller->kp * error;
	// integral part
	controller->integral += error * controller->dt;
	if ((controller->integral > controller->integralLim)
			|| (controller->integral < -controller->integralLim)) {

		(controller->integral > controller->integralLim) ?
				(controller->integral = controller->integralLim) :
				(controller->integral = -controller->integralLim);
	}
	float I = controller->ki * controller->integral;
	// differential part
	float D = controller->kd * (error - controller->prevError) / controller->dt;
	controller->prevError = error;

	float output = P + I + D;

	if (output > controller->outMax) {

		output = controller->outMax;
		// Clamping anti-windup
		if (error * output > 0) {

			controller->integral -= error * controller->dt;
		}
	}
	if (output < controller->outMin) {

		output = controller->outMin;
		// Clamping anti-windup
		if (error * output > 0) {

			controller->integral -= error * controller->dt;
		}
	}

	return output;
}
