#include "Relay.h"

int fan_pin = 2;
int water_pump_pin = 0;
int fan_state = 0;
int water_pump_state = 0;

int get_relay_state()
{
    return relay_state;
}

int get_fan_state()
{
    return fan_state;
}

int get_water_pump_state()
{
    return water_pump_state;
}

void *fan_on()
{
    pthread_mutex_lock(&mtx);
    signal_received = 1;
    relay_state = FAN_ON;
    fan_state = 1;
    fprintf(stdout, "send FAN_ON signal\n");
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&mtx);
}

void *fan_off()
{
    pthread_mutex_lock(&mtx);
    signal_received = 1;
    relay_state = FAN_OFF;
    fan_state = 0;
    fprintf(stdout, "send FAN_OFF signal\n");
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&mtx);
}

void *water_pump_on()
{
    pthread_mutex_lock(&mtx);
    signal_received = 1;
    relay_state = WATER_PUMP_ON;
    water_pump_state = 1;
    fprintf(stdout, "send WATER_PUMP_ON signal\n");
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&mtx);
}

void *water_pump_off()
{
    pthread_mutex_lock(&mtx);
    signal_received = 1;
    relay_state = WATER_PUMP_OFF;
    water_pump_state = 0;
    fprintf(stdout, "send WATER_PUMP_OFF signal\n");
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&mtx);
}

void *relay_thread(void *arg)
{
    // 初始化wiringPi库
    if (wiringPiSetup() == -1)
    {
        fprintf(stderr, "Failed to initialize wiringPi.\n");
        exit(1);
    }

    // 设置GPIO引脚模式为输出
    pinMode(fan_pin, OUTPUT);
    pinMode(water_pump_pin, OUTPUT);

    while (1)
    {
        pthread_mutex_lock(&mtx);
        while (!signal_received)
        {
            pthread_cond_wait(&cv, &mtx);
        }

        if (relay_state == FAN_ON)
        {
            // 收到信号A，开启继电器（高电平）
            digitalWrite(fan_pin, LOW);
            fprintf(stdout, "Fan Relay ON\n");
        }
        else if (relay_state == FAN_OFF)
        {
            // 收到信号B，关闭继电器（低电平）
            digitalWrite(fan_pin, HIGH);
            fprintf(stdout, "Fan Relay OFF\n");
        }
        else if (relay_state == WATER_PUMP_ON)
        {
            digitalWrite(water_pump_pin, LOW);
            fprintf(stdout, "Water Pump Relay ON\n");
        }
        else if (relay_state == WATER_PUMP_OFF)
        {
            digitalWrite(water_pump_pin, HIGH);
            fprintf(stdout, "Water Pump Relay OFF\n");
        }
        else
        {
            fprintf(stderr, "Unknown relay state %d\n", relay_state);
        }

        // 重置信号标志
        signal_received = 0;
        pthread_mutex_unlock(&mtx);
    }

    return NULL;
}
