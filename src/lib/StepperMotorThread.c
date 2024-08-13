#include "StepperMotorThread.h"

void* start(void* arg) {
    int singleEightBeat[8] = {1, 0, 0, 0, 0, 0, 0, 0};
    int doubleEightBeat[8] = {1, 0, 0, 0, 1, 0, 0, 0};

    StepperMotor mStepperMotor;

    while (1) {
        sem_wait(&sem_motor_on); // 等待启动信号
        for (int i = 0; i < 8; i++) {
            mStepperMotor->setMotorPins(0, singleEightBeat[i]);
            mStepperMotor->setMotorPins(1, singleEightBeat[(i + 1) % 8]);
            mStepperMotor->setMotorPins(2, singleEightBeat[(i + 2) % 8]);
            mStepperMotor->setMotorPins(3, singleEightBeat[(i + 3) % 8]);
            usleep(1000);
        }
        for (int i = 0; i < 8; i++) {
            mStepperMotor->setMotorPins(0, doubleEightBeat[i]);
            mStepperMotor->setMotorPins(1, doubleEightBeat[(i + 1) % 8]);
            mStepperMotor->setMotorPins(2, doubleEightBeat[(i + 2) % 8]);
            mStepperMotor->setMotorPins(3, doubleEightBeat[(i + 3) % 8]);
            usleep(1000);
        }
        sem_post(&sem_motor_off); // 发送停止信号
    }
    pthread_exit(NULL);
}