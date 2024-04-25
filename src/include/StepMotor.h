#ifndef INCLUDE_STEP_MOTOR_H_
#define INCLUDE_STEP_MOTOR_H_

#include "common.h"

#define CLOCKWISE 1
#define COUNTERCLOCKWISE -1

typedef struct 
{
    int pin[4];
    int direction;
    int speed;
} StepMotor;

int initialize(StepMotor *self, int pin1, int pin2, int pin3, int pin4);
int rotate(StepMotor *self, float angle, int direction);

#endif // INCLUDE_STEP_MOTOR_H_