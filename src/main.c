#include "DHTXXWorkerThread.h"
#include "StepperMotor.h"

void signal_handler(int signum) {
    exit_condition = 1; // 设置退出条件，使线程退出循环
    printf("接收到信号: %d\n", signum);
}

int main()
{

    pthread_t dhtxx_thread, stepper_motor_thread;
    int dhtxx_result, stepper_motor_result;

    // 注册信号处理函数
    signal(SIGINT, signal_handler); // 处理Ctrl+C信号
    signal(SIGTERM, signal_handler); // 处理终止信号

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