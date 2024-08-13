#include "DHTXX.h"
//#include "TinyStepper.h"


#include "DHTXXWorkerThread.h"
#include "thread_func2.h"

uint8 exit_condition = 0;
sem_t sem_motor_on;
sem_t sem_motor_off;

void signal_handler(int signum) {
    exit_condition = 1; // 设置退出条件，使线程退出循环
    printf("接收到信号: %d\n", signum);
}

int main()
{

    pthread_t dhtxx_thread, stepper_motor_thread;
    int dhtxx_result, stepper_motor_result;

    // 初始化信号量
    if (sem_init(&sem_motor_on, 0, 1) != 0) {
        perror("信号量sem_motor_on初始化失败");
        exit(1);
    }

    if (sem_init(&sem_motor_off, 0, 1) != 0) {
        perror("信号量sem_motor_off初始化失败");
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
    stepper_motor_result = pthread_create(&stepper_motor_thread, NULL, start, NULL);
    if (stepper_motor_result != 0) {
        perror("Failed to create stepper_motor_thread");
        exit(EXIT_FAILURE);
    }

    // 等待线程结束
    pthread_join(dhtxx_thread, NULL);
    pthread_join(stepper_motor_thread, NULL);

    // 销毁信号量
    sem_destroy(&sem_motor_on);
    sem_destroy(&sem_motor_off);

    printf("All threads finished.\n");

    return 0;
}