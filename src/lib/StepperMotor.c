#include "StepperMotor.h"

void setMotorPins(StepperMoter *self, int motorIdx, int value)
{
    if (motorIndex < 0 || motorIndex > 3)
    {
        return;
    }
    gpio_set_value(self->motorPin[motorIndex], value);
}
