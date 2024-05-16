#ifndef INCLUDE_TINY_STEPPER_H_
#define INCLUDE_TINY_STEPPER_H_

#include "common.h"

#define CLOCKWISE 1
#define COUNTERCLOCKWISE -1

typedef struct 
{
    int pin[4];
    int direction;
    int speed;
} TinyStepper;

int tinyStepperInitialize(TinyStepper *self, int pin1, int pin2, int pin3, int pin4);
int tinyStepperRotate(TinyStepper *self, float angle, int direction);

#endif // INCLUDE_TINY_STEPPER_H_