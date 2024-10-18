#ifndef INCLUDE_RELAY_H
#define INCLUDE_RELAY_H

#include "common.h"

// 定义继电器连接的GPIO引脚
extern int fan_pin; // 根据实际情况修改
extern int water_pump_pin;
extern int fan_led_pin; // 根据实际情况修改
extern int water_pump_led_pin;
extern int fan_state;
extern int water_pump_state;

// 定义全局变量
pthread_mutex_t mtx;
pthread_cond_t cv;
int signal_received;
int relay_state;
int fan_state;

#define RELAY_STATE_OFF	0
#define FAN_ON          1
#define FAN_OFF         2
#define WATER_PUMP_ON   3 
#define WATER_PUMP_OFF  4 

void* relay_thread(void* arg);
void* fan_on();
void* fan_off();
void* water_pump_on();
void* water_pump_off();
int get_relay_state();
int get_fan_state();
int get_water_pump_state();

#endif // INCLUDE_RELAY_H
