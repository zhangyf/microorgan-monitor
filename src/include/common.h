#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <wiringPi.h>
#include <stddef.h> 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>

typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef unsigned long uint32;

#define SECONDS_PER_DAY (60 * 60 * 24)
#define DIFFERENCE_MORE_THAN_3_DAYS(timestamp1, timestamp2) ((difftime(timestamp1, timestamp2) / SECONDS_PER_DAY) > 3)

#endif // INCLUDE_COMMON_H_