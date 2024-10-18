#include "StepperMotor.h"

volatile int running = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void setPin(int pin, int value)
{
    digitalWrite(pin, value);
}

int get_motor_state()
{
    return running;
}

void step()
{
    setPin(MOTOR_PIN_1, 1);
    setPin(MOTOR_PIN_2, 0);
    setPin(MOTOR_PIN_3, 0);
    setPin(MOTOR_PIN_4, 0);
    delay(2);
    setPin(MOTOR_PIN_1, 0);
    setPin(MOTOR_PIN_2, 1);
    setPin(MOTOR_PIN_3, 0);
    setPin(MOTOR_PIN_4, 0);
    delay(2);
    setPin(MOTOR_PIN_1, 0);
    setPin(MOTOR_PIN_2, 0);
    setPin(MOTOR_PIN_3, 1);
    setPin(MOTOR_PIN_4, 0);
    delay(2);
    setPin(MOTOR_PIN_1, 0);
    setPin(MOTOR_PIN_2, 0);
    setPin(MOTOR_PIN_3, 0);
    setPin(MOTOR_PIN_4, 1);
    delay(2);
    setPin(MOTOR_PIN_1, 1);
    setPin(MOTOR_PIN_2, 1);
    setPin(MOTOR_PIN_3, 0);
    setPin(MOTOR_PIN_4, 0);
    delay(2);
    setPin(MOTOR_PIN_1, 0);
    setPin(MOTOR_PIN_2, 1);
    setPin(MOTOR_PIN_3, 1);
    setPin(MOTOR_PIN_4, 0);
    delay(2);
    setPin(MOTOR_PIN_1, 0);
    setPin(MOTOR_PIN_2, 0);
    setPin(MOTOR_PIN_3, 1);
    setPin(MOTOR_PIN_4, 1);
    delay(2);
    setPin(MOTOR_PIN_1, 1);
    setPin(MOTOR_PIN_2, 0);
    setPin(MOTOR_PIN_3, 0);
    setPin(MOTOR_PIN_4, 1);
    delay(2);
}

void *motor_thread(void *arg)
{
    fprintf(stdout, "start motor thread %d\n", running);
	
	pinMode(MOTOR_PIN_1, OUTPUT);
    pinMode(MOTOR_PIN_2, OUTPUT);
    pinMode(MOTOR_PIN_3, OUTPUT);
    pinMode(MOTOR_PIN_4, OUTPUT);
    pinMode(MOTOR_LED_PIN, OUTPUT);
    
	while (1)
    {
        pthread_mutex_lock(&mutex);
        if (running)
        {
			//fprintf(stdout, "motor_thread start to involve step()\n");
            step();
        }
        pthread_mutex_unlock(&mutex);
        usleep(1000);
    }
    return NULL;
}

void start_motor()
{
    pthread_mutex_lock(&mutex);
    running = 1;
	fprintf(stdout, "light on %d\n", MOTOR_LED_PIN);
	digitalWrite(MOTOR_LED_PIN, HIGH);
	fprintf(stdout, "involved start_motor()\n");
    pthread_mutex_unlock(&mutex);
}

void stop_motor()
{
    pthread_mutex_lock(&mutex);
    running = 0;
	fprintf(stdout, "light off %d\n", MOTOR_LED_PIN);
	digitalWrite(MOTOR_LED_PIN, LOW);
	fprintf(stdout, "involved stop_motor()\n");
    pthread_mutex_unlock(&mutex);
}

int stepper_motor_init()
{
    if (wiringPiSetup() == -1)
    {
        fprintf(stderr, "WiringPi setup failed\n");
        return 1;
    }

    pthread_t thread_id;
    int ret;

    ret = pthread_create(&thread_id, NULL, motor_thread, NULL);
    if (ret != 0)
    {
        fprintf(stderr, "Failed to create thread");
        return -1;
    }
    fprintf(stdout, "stepper_motor pid=%d\n", ret);
    return 0;
}
