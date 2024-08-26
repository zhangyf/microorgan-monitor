#include "DHTXXWorkerThread.h"
#include "StepperMotor.h"

int main()
{

    pthread_t dhtxx_thread, stepper_motor_thread;
    int dhtxx_result, stepper_motor_result;

    // 创建DHTXX 工作线程，执行start函数
    dhtxx_result = pthread_create(&dhtxx_thread, NULL, start, NULL);
    if (dhtxx_result != 0) {
        perror("Failed to create dhtxx worker thread");
        exit(EXIT_FAILURE);
    }

    // 初始化步进电机驱动
    if (stepper_motor_init() != 0) {
        fprintf(stderr, "Failed to initialize stepper motor driver\n");
        return 1;
    }

    // 等待线程结束
    pthread_join(dhtxx_thread, NULL);
    printf("All threads finished.\n");

    return 0;
}