#include "StepperMotor.h"

namespace microorgan_monitor
{
    std::mutex stepper_motor_mtx;
}

void microorgan_monitor::StepperMonitor::SetPin(unsigned int pin, int value)
{
    digitalWrite(pin, value);
}

unsigned int microorgan_monitor::StepperMonitor::GetPin(unsigned int idx) const
{
    return pin_num_[idx];
}

void microorgan_monitor::StepperMonitor::Step() {
    for (auto vec = motor_value_.begin(); vec != motor_value_.end(); vec++) 
    {
        for (int i = 0; i < (*vec).size(); i++) {
            SetPin(GetPin(i), (*vec)[i]);
        }
        delay(2);
    }
}

void microorgan_monitor::StepperMonitor::Loop() {
    while (true) {
        std::unique_lock<std::mutex> lock(stepper_motor_mtx);
        if (IsRunning()) {
            Step();
        }
        lock.unlock();
        usleep(1000);
    }
}

bool microorgan_monitor::StepperMonitor::IsRunning()
{
    return running_;
}

std::vector<int> microorgan_monitor::StepperMonitor::GetPins() const 
{
    return pin_num_;
}

void microorgan_monitor::StepperMonitor::SetRunning(bool running)
{
    std::unique_lock<std::mutex> lock(stepper_motor_mtx);
    running_ = running;
}

void microorgan_monitor::StepperMonitor::StartMotor() 
{
    SetRunning(true);
}

void microorgan_monitor::StepperMonitor::StopMotor() 
{
    SetRunning(false);
}
