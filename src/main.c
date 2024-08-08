#include "DHTXX.h"
//#include "TinyStepper.h"


#include "DHTXXWorkerThread.h"
#include "thread_func2.h"

uint8 exit_condition = 0;
sem_t sem_on;

void signal_handler(int signum) {
    exit_condition = 1; // 设置退出条件，使线程退出循环
    printf("接收到信号: %d\n", signum);
}

int main()
{

    // TinyStepper mTinyStepper;
    // if (tinyStepperInitialize(&mTinyStepper, STEP_MOTOR_PIN_A, STEP_MOTOR_PIN_B, STEP_MOTOR_PIN_C, STEP_MOTOR_PIN_D) > 0)
    // {
    //     return -1;
    // }

    pthread_t dhtxx_thread, thread2; // 创建两个线程ID
    int dhtxx_result, result2;

    // 初始化信号量
    if (sem_init(&sem_on, 0, 1) != 0) {
        perror("信号量sem_on初始化失败");
        exit(1);
    }

    // 注册信号处理函数
    signal(SIGINT, signal_handler); // 处理Ctrl+C信号
    signal(SIGTERM, signal_handler); // 处理终止信号

    // 创建DHTXX 工作线程，执行start函数
    dhtxx_result = pthread_create(&dhtxx_thread, NULL, start, NULL);
    if (dhtxx_result != 0) {
        perror("Failed to create dhtxx worker thread");
        exit(EXIT_FAILURE);
    }

    // 创建线程2，执行thread_func2函数
    result2 = pthread_create(&thread2, NULL, thread_func2, NULL);
    if (result2 != 0) {
        perror("Failed to create thread 2");
        exit(EXIT_FAILURE);
    }

    // 等待线程结束
    pthread_join(dhtxx_thread, NULL);
    pthread_join(thread2, NULL);

    // 销毁信号量
    sem_destroy(&sem_on);

    printf("All threads finished.\n");

    return 0;
}