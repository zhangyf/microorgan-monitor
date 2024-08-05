//#include "DHTXX.h"
//#include "TinyStepper.h"

#include <stdlib.h>
#include <pthread.h>
#include "thread_func1.h"
#include "thread_func2.h"

typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef unsigned long uint32;

#define DHT11_PIN 25

#define STEP_MOTOR_PIN_A 26
#define STEP_MOTOR_PIN_B 27
#define STEP_MOTOR_PIN_C 28
#define STEP_MOTOR_PIN_D 29

int main()
{
    // int ret = 0;

    // DHT11 mDHT;
    // if (dhtxxInitialize(&mDHT, DHT11_PIN) > 0)
    // {
    //     return -1;
    // }

    // TinyStepper mTinyStepper;
    // if (tinyStepperInitialize(&mTinyStepper, STEP_MOTOR_PIN_A, STEP_MOTOR_PIN_B, STEP_MOTOR_PIN_C, STEP_MOTOR_PIN_D) > 0)
    // {
    //     return -1;
    // }

    // pinMode(mDHT.pinNum, OUTPUT); // set mode to output
    // digitalWrite(mDHT.pinNum, HIGH); // output a high level 
    // printf("Starting to read....\n");

    // while(1)
    // {
    //     pinMode(mDHT.pinNum, OUTPUT); // set mode to output
    //     digitalWrite(mDHT.pinNum, HIGH); // output a high level 
    //     delay(3000);
    //     if (dhtxxRead(&mDHT))
    //     {
    //         printf("DHT11 Sensor data read ok!\t{ \"timestamp\":\t%ld\t\"humidity\":\t%.1f%\t\"temperature\":\t%.1f C }\n", 
    //                     dhtxxGetTimestamp(&mDHT), dhtxxGetHumidity(&mDHT) , dhtxxGetTemperature(&mDHT));
            
    //         // printf("RH:%d.%d\n", (mDHT.databuf >> 24) & 0xff, (mDHT.databuf >> 16) & 0xff); 
    //         // printf("TMP:%d.%d\n", (mDHT.databuf >> 8) & 0xff, mDHT.databuf & 0xff);
    //         dhtxxReset(&mDHT);
    //     }
    //     else
    //     {
    //         printf("Sensor dosen't ans!\n");
    //         dhtxxReset(&mDHT);
    //     }
    // }
    // return 0;

    pthread_t thread1, thread2; // 创建两个线程ID
    int result1, result2;

    // 创建线程1，执行thread_func1函数
    result1 = pthread_create(&thread1, NULL, thread_func1, NULL);
    if (result1 != 0) {
        perror("Failed to create thread 1");
        exit(EXIT_FAILURE);
    }

    // 创建线程2，执行thread_func2函数
    result2 = pthread_create(&thread2, NULL, thread_func2, NULL);
    if (result2 != 0) {
        perror("Failed to create thread 2");
        exit(EXIT_FAILURE);
    }

    // 等待线程结束
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("All threads finished.\n");

    return 0;
}