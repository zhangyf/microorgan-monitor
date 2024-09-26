#ifndef INCLUDE_RELAY_H
#define INCLUDE_RELAY_H

#include "common.h"
#include <mutex>
#include <condition_variable>
#include <atomic>

// 定义继电器连接的GPIO引脚
extern int fan_pin; // 根据实际情况修改
extern int water_pump_pin;

// 定义全局变量
pthread_mutex_t mtx;
pthread_cond_t cv;
int signal_received;
int relay_state;

#define FAN_ON          0
#define FAN_OFF         1
#define WATER_PUMP_ON   2 
#define WATER_PUMP_OFF  3 

void* relay_thread(void* arg);

#endif // INCLUDE_RELAY_H