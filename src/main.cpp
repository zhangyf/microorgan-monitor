// #include "DHTXXWorkerThread.h"
// #include "StepperMotor.h"
// #include "MarqueueThread.h"

#include "DHTXX.h"

int main(int argc, char* argv[]) {

    std::cout << "hello world" << std::endl;
    DHTXX dhtxx;

    dhtxx.SetPinNum(DHT11_PIN);

    std::cout << dhtxx.GetPinNum() << std::endl;
    std::cout << dhtxx.GetTimestamp() << std::endl;

    std::thread dhtxx_thread(&DHTXX::Start, &dhtxx);

    dhtxx_thread.join();

    // pthread_t dhtxx_tid, motor_tid, relay_tid, marqueue_tid;
    // int dhtxx_result, motor_result, relay_result, marqueue_result;

    // marqueue_init();

    // pthread_mutex_init(&mtx, NULL);
    // pthread_cond_init(&cv, NULL);
    // signal_received = 0;
    // relay_state = RELAY_STATE_OFF;

    // if (wiringPiSetup() == -1) {
    //     fprintf(stderr, "WiringPi setup failed\n");
    //     return 1;
    // }

    // // 创建DHTXX 工作线程，执行start函数
    // dhtxx_result = pthread_create(&dhtxx_tid, NULL, start, NULL);
    // if (dhtxx_result != 0) {
    //     perror("Failed to create dhtxx worker thread");
    //     exit(EXIT_FAILURE);
    // }

    // // 初始化步进电机驱动
	// motor_result = pthread_create(&motor_tid, NULL, motor_thread, NULL);
    // if (motor_result != 0) {
    //     perror("Failed to create thread");
    //     return -1;
    // }

    // relay_result = pthread_create(&relay_tid, NULL, relay_thread, NULL);
    // if (relay_result != 0) {
    //     perror("Failed to create thread");
    //     return -1;        
    // }

    // marqueue_result = pthread_create(&marqueue_tid, NULL, marqueue_thread, NULL);
    // if (marqueue_result != 0) {
    //     perror("Failed to create thread");
    //     return -1;        
    // }

    // update_message("Hello LCD!");

    // // 等待线程结束
    // pthread_join(dhtxx_tid, NULL);
    // pthread_join(motor_tid, NULL);
    // pthread_join(relay_tid, NULL);
    // pthread_join(marqueue_tid, NULL);
    // printf("All threads finished.\n");

    return 0;
}
