#ifndef INCLUDE_STEPPER_MOTER_H_
#define INCLUDE_STEPPER_MOTER_H_

#include "common.h"

namespace microorgan_monitor
{
#define MOTOR_PIN_1 22
#define MOTOR_PIN_2 23
#define MOTOR_PIN_3 24
#define MOTOR_PIN_4 25

    extern std::mutex stepper_motor_mtx;

    class StepperMonitor
    {
    public:
        StepperMonitor() : running_(false) {};
        ~StepperMonitor() {};

        void Loop();
        void StartMotor();
        void StopMotor();
        std::vector<int> GetPins() const;

    private:
        void SetPin(unsigned int pin, int value);
        void Step();
        bool IsRunning();
        void SetRunning(bool running);
        unsigned int GetPin(unsigned int idx) const;

        bool running_;
        const std::vector<std::vector<int>> motor_value_ = {
            {1, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 1},
            {0, 0, 0, 1},
            {1, 0, 0, 1}};
        const std::vector<int> pin_num_ = {MOTOR_PIN_1, MOTOR_PIN_2, MOTOR_PIN_3, MOTOR_PIN_4};
    };
}

#endif // INCLUDE_STEPPER_MOTER_H_
