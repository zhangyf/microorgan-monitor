#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

//#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef unsigned long uint32;

#define NUM_THREADS 5 // 定义线程数量
#define DELAY_SECONDS 1 // 定义每个线程输出的时间间隔（秒）

// 定义一个结构体，将线程ID和要输出的数字存储在一起
typedef struct {
    int thread_id;
    int number;
} ThreadData;

#endif // INCLUDE_COMMON_H_