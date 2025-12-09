#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "common.h"

void dcMotorInit(void);
void dcMotorRun(uint8_t rpm);
void dcMotorStop(void);

#endif /* DCMOTOR_H_ */
