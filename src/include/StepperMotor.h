#ifndef INCLUDE_STEPPER_MOTER_H_
#define INCLUDE_STEPPER_MOTER_H_

#include "common.h"

#define MOTOR_PIN_1 7
#define MOTOR_PIN_2 0
#define MOTOR_PIN_3 2
#define MOTOR_PIN_4 3

extern volatile int running;
extern pthread_mutex_t mutex;

void    setPin(int pin, int value);
void    step();
void*   motor_thread(void* arg);
void    start_motor();
void    stop_motor();
int     stepper_motor_init();

#endif // INCLUDE_STEPPER_MOTER_H_
