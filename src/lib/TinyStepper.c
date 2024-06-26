#include "TinyStepper.h"

int tinyStepperInitialize(TinyStepper *self, int pin1, int pin2, int pin3, int pin4)
{
    self->pin[0] = pin1;
    self->pin[1] = pin2;
    self->pin[2] = pin3;
    self->pin[3] = pin4;

    if (-1 == wiringPiSetup()) {
        printf("StepMotor Setup wiringPi failed!");
        return 1;
    }
    else {
        printf("StepMotor Successfully setup wiringPi!\n");
    }

    for (int i=0; i<4; i++)
    {
        pinMode(self->pin[i], OUTPUT);
    }

    return 0;
}

int tinyStepperRotate(TinyStepper *self, float angle, int direction)
{
    printf("start to rotate\n");
    return 0;
}

