#ifndef INCLUDE_BUTTON_STATE_H_
#define INCLUDE_BUTTON_STATE_H_

#include "common.h"
#include "StepperMotor.h"
#include "Relay.h"
#include "MarqueueThread.h"

#define FAN_BTN_PIN
#define WATER_PUMP_BTN_PIN
#define STEPPER_MOTOR_BTN_PIN

void initBtns();
void buttonInterrupt(int pin);

#endif