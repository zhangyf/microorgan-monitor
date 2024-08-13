#include "StepperMotor.h"

volatile int running = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_start = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_stop = PTHREAD_COND_INITIALIZER;

void setPin(int pin, int value) {
    digitalWrite(pin, value);
}

void step(int steps, int delay) {
    int sequence[8][4] = {
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 1},
        {1, 0, 0, 1}
    };

    for (int i = 0; i < steps; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 4; k++) {
                setPin(PIN_1 + k, sequence[j][k]);
            }
            usleep(delay);
        }
    }
}

void* motor_thread(void* arg) {
    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    while (1) {
        pthread_mutex_lock(&mutex);
        while (!running) {
            pthread_cond_wait(&cond_start, &mutex);
        }
        pthread_mutex_unlock(&mutex);

        step(2048, 100); // 转动一圈

        pthread_mutex_lock(&mutex);
        running = 0;
        pthread_cond_signal(&cond_stop);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void start_motor() {
    pthread_mutex_lock(&mutex);
    running = 1;
    pthread_cond_signal(&cond_start);
    pthread_mutex_unlock(&mutex);
}

void stop_motor() {
    pthread_mutex_lock(&mutex);
    while (running) {
        pthread_cond_wait(&cond_stop, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

int stepper_motor_init() {
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "WiringPi setup failed\n");
        return 1;
    }

    pinMode(PIN_1, OUTPUT);
    pinMode(PIN_2, OUTPUT);
    pinMode(PIN_3, OUTPUT);
    pinMode(PIN_4, OUTPUT);

    pthread_t stepper_motor_thread;
    pthread_create(&stepper_motor_thread, NULL, motor_thread, NULL);

    return 0;
}
