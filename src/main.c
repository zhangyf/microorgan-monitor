#include "DHTXXWorkerThread.h"
#include "StepperMotor.h"
#include "MarqueueThread.h"

int main()
{
    pthread_t dhtxx_tid, motor_tid, relay_tid, marqueue_tid;
    int dhtxx_result, motor_result, relay_result, marqueue_result;

    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&cv, NULL);
    signal_received = 0;
    relay_state = RELAY_STATE_OFF;

    if (wiringPiSetup() == -1) {
        fprintf(stderr, "WiringPi setup failed\n");
        return 1;
    }

//    marqueue_init();

    // 创建DHTXX 工作线程，执行start函数
    dhtxx_result = pthread_create(&dhtxx_tid, NULL, start, NULL);
    if (dhtxx_result != 0) {
        perror("Failed to create dhtxx worker thread");
        exit(EXIT_FAILURE);
    }

    // 初始化步进电机驱动
	motor_result = pthread_create(&motor_tid, NULL, motor_thread, NULL);
    if (motor_result != 0) {
        perror("Failed to create thread");
        return -1;
    }

    relay_result = pthread_create(&relay_tid, NULL, relay_thread, NULL);
    if (relay_result != 0) {
        perror("Failed to create thread");
        return -1;        
    }

//    marqueue_result = pthread_create(&marqueue_tid, NULL, marqueue_thread, NULL);
//    if (marqueue_result != 0) {
//        perror("Failed to create thread");
//        return -1;        
//    }
//
//    update_message("Hello LCD!");

//	printf("delay 10s to wait .....\n");
//	delay(10000);
//
//	pthread_mutex_lock(&mtx);
//    signal_received = 1;
//    relay_state = WATER_PUMP_ON;
//	printf("send WATER_PUMP_ON signal\n");
//	pthread_cond_signal(&cv);
//	pthread_mutex_unlock(&mtx);
//
//	delay(10000);
//
//	pthread_mutex_lock(&mtx);
//    signal_received = 1;
//    relay_state = WATER_PUMP_OFF;
//	printf("send WATER_PUMP_OFF signal\n");
//	pthread_cond_signal(&cv);
//	pthread_mutex_unlock(&mtx);


    // 等待线程结束
    pthread_join(dhtxx_tid, NULL);
    pthread_join(motor_tid, NULL);
    pthread_join(relay_tid, NULL);
//    pthread_join(marqueue_tid, NULL);
    printf("All threads finished.\n");

    return 0;
}
