#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "common.h"

#define MAX_RPM 1360.0f

void dcMotorInit(void);
void dcMotorRun(uint8_t rpm);
void dcMotorStop(void);

#endif /* DCMOTOR_H_ */
