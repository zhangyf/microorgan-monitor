#include "Relay.h"

int fan_pin = 32;
int water_pump_pin = 29;

void* relay_thread(void* arg) {
    // 初始化wiringPi库
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "Failed to initialize wiringPi.\n");
        exit(1);
    }

    // 设置GPIO引脚模式为输出
    pinMode(fan_pin, OUTPUT);
    pinMode(water_pump_pin, OUTPUT);

    while (1) {
        pthread_mutex_lock(&mtx);
        while (!signal_received) {
            pthread_cond_wait(&cv, &mtx);
        }

        if (relay_state == FAN_ON) {
            // 收到信号A，开启继电器（高电平）
            digitalWrite(fan_pin, HIGH);
            printf("Relay ON\n");
        } else if (relay_state == FAN_OFF) {
            // 收到信号B，关闭继电器（低电平）
            digitalWrite(fan_pin, LOW);
            printf("Relay OFF\n");
        } else if (relay_state == WATER_PUMP_ON) {
            digitalWrite(water_pump_pin, LOW);
            printf("Relay OFF\n");
        } else if (relay_state == WATER_PUMP_OFF) {
            digitalWrite(water_pump_pin, LOW);
            printf("Relay OFF\n");
        } else {
            printf("Unknown relay state %d\n", relay_state);
        }

        // 重置信号标志
        signal_received = 0;
        pthread_mutex_unlock(&mtx);
    }

    return NULL;
}