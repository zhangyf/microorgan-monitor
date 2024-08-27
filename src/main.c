#include "DHTXXWorkerThread.h"
#include "StepperMotor.h"

int main()
{

    pthread_t dhtxx_tid, motor_tid;
    int dhtxx_result, motor_result;

    if (wiringPiSetup() == -1) {
        fprintf(stderr, "WiringPi setup failed\n");
        return 1;
    }

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

    // 等待线程结束
    pthread_join(dhtxx_tid, NULL);
    pthread_join(motor_tid, NULL);
    printf("All threads finished.\n");

    return 0;
}
