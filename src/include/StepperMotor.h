#ifndef INCLUDE_STEPPER_MOTER_H_
#define INCLUDE_STEPPER_MOTER_H_

#include "common.h"

#define MOTOR_PIN_1 17
#define MOTOR_PIN_2 18
#define MOTOR_PIN_3 27
#define MOTOR_PIN_4 22

typedef struct
{
    int motorPinp[4];
} StepperMotor;

extern sem_t sem_motor_on;
extern sem_t sem_motor_off;

void setMotorPins(StepperMoter *self, int motorIdx, int value);

#endif // INCLUDE_STEPPER_MOTER_H_