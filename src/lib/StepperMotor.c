#include "StepperMotor.h"

volatile int running = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void setPin(int pin, int value) {
    digitalWrite(pin, value);
}

void step() {
    setPin(MOTOR_PIN_1, 1); setPin(MOTOR_PIN_2, 0); setPin(MOTOR_PIN_3, 0); setPin(MOTOR_PIN_4, 0); delay(2);
    setPin(MOTOR_PIN_1, 0); setPin(MOTOR_PIN_2, 1); setPin(MOTOR_PIN_3, 0); setPin(MOTOR_PIN_4, 0); delay(2);
    setPin(MOTOR_PIN_1, 0); setPin(MOTOR_PIN_2, 0); setPin(MOTOR_PIN_3, 1); setPin(MOTOR_PIN_4, 0); delay(2);
    setPin(MOTOR_PIN_1, 0); setPin(MOTOR_PIN_2, 0); setPin(MOTOR_PIN_3, 0); setPin(MOTOR_PIN_4, 1); delay(2);
    setPin(MOTOR_PIN_1, 1); setPin(MOTOR_PIN_2, 1); setPin(MOTOR_PIN_3, 0); setPin(MOTOR_PIN_4, 0); delay(2);
    setPin(MOTOR_PIN_1, 0); setPin(MOTOR_PIN_2, 1); setPin(MOTOR_PIN_3, 1); setPin(MOTOR_PIN_4, 0); delay(2);
    setPin(MOTOR_PIN_1, 0); setPin(MOTOR_PIN_2, 0); setPin(MOTOR_PIN_3, 1); setPin(MOTOR_PIN_4, 1); delay(2);
    setPin(MOTOR_PIN_1, 1); setPin(MOTOR_PIN_2, 0); setPin(MOTOR_PIN_3, 0); setPin(MOTOR_PIN_4, 1); delay(2);
}

void* motor_thread(void* arg) {
    printf("start motor thread %d\n", running);
    while (1) {
        pthread_mutex_lock(&mutex);
        if (running) {
            step();
        }
        pthread_mutex_unlock(&mutex);
        usleep(1000);
    }
    return NULL;
}

void start_motor() {
    pthread_mutex_lock(&mutex);
    running = 1;
    pthread_mutex_unlock(&mutex);
}

void stop_motor() {
    pthread_mutex_lock(&mutex);
    running = 0;
    pthread_mutex_unlock(&mutex);
}

int stepper_motor_init() {
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "WiringPi setup failed\n");
        return 1;
    }

    pthread_t thread_id;
    int ret;

    ret = pthread_create(&thread_id, NULL, motor_thread, NULL);
    if (ret != 0) {
        perror("Failed to create thread");
        return -1;
    }
	printf("stepper_motor pid=%d\n", ret);
    return 0;
}
