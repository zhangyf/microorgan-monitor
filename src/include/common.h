#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

//#include <wiringPi.h>
// #include <stddef.h> 
// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <unistd.h>
// 
// #include <signal.h>
// #include <time.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>
#include <memory>
#include <mutex>
#include <semaphore.h>
#include <vector>
#include <string>

#define SECONDS_PER_DAY (60 * 60 * 24)
#define DIFFERENCE_MORE_THAN_3_DAYS(timestamp1, timestamp2) ((difftime(timestamp1, timestamp2) / SECONDS_PER_DAY) > 3)

#endif // INCLUDE_COMMON_H_