#include "Relay.h"

namespace microorgan_monitor
{
    std::mutex relay_mtx;
    sem_t relay_sem;
}

unsigned int microorgan_monitor::Relay::GetFanPinNum() const
{
    return fan_pin_num_;
}

unsigned int microorgan_monitor::Relay::GetWaterPumpPinNum() const
{
    return water_pump_pin_num_;
}

unsigned int microorgan_monitor::Relay::GetRelayState() const
{
    return relay_state_;
}

bool microorgan_monitor::Relay::GetSignalReceived() const
{
    return signal_received_;
}

void microorgan_monitor::Relay::SetFanPinNum(const unsigned int fan_pin_num)
{
    fan_pin_num_ = fan_pin_num;
}

void microorgan_monitor::Relay::SetWaterPumpPinNum(const unsigned int water_pump_pin_num)
{
    water_pump_pin_num_ = water_pump_pin_num;
}

void microorgan_monitor::Relay::SetSignalReceived(bool signal_received)
{
    signal_received_ = signal_received;
}

void microorgan_monitor::Relay::SetRelayState(unsigned int relay_state)
{
    relay_state_ = relay_state;
}

void microorgan_monitor::Relay::FanOn()
{
    std::unique_lock<std::mutex> lock(relay_mtx);
    SetSignalReceived(true);
    SetRelayState(FAN_ON);
    sem_post(&relay_sem);
}

void microorgan_monitor::Relay::FanOff()
{
    std::unique_lock<std::mutex> lock(relay_mtx);
    SetSignalReceived(true);
    SetRelayState(FAN_OFF);
    sem_post(&relay_sem);
}

void microorgan_monitor::Relay::WaterPumpOn()
{
    std::unique_lock<std::mutex> lock(relay_mtx);
    SetSignalReceived(true);
    SetRelayState(WATER_PUMP_ON);
    sem_post(&relay_sem);
}

void microorgan_monitor::Relay::WaterPumpOff()
{
    std::unique_lock<std::mutex> lock(relay_mtx);
    SetSignalReceived(true);
    SetRelayState(WATER_PUMP_OFF);
    sem_post(&relay_sem);
}

void microorgan_monitor::Relay::Loop()
{
    std::cout << "Enter Relay Loop()" << std::endl;
    // 初始化wiringPi库
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "Failed to initialize wiringPi.\n");
        exit(1);
    }

    // 设置GPIO引脚模式为输出
    pinMode(GetFanPinNum(), OUTPUT);
    pinMode(GetWaterPumpPinNum(), OUTPUT);

    while (1) {
        std::unique_lock<std::mutex> lock(mtx);
        while (!GetSignalReceived()) {
            sem_wait(&relay_sem);
        }

        if (GetRelayState() == FAN_ON) {
            // 收到信号A，开启继电器（高电平）
            digitalWrite(fan_pin, LOW);
            printf("Fan Relay ON\n");
        } else if (GetRelayState() == FAN_OFF) {
            // 收到信号B，关闭继电器（低电平）
            digitalWrite(fan_pin, HIGH);
            printf("Fan Relay OFF\n");
        } else if (GetRelayState() == WATER_PUMP_ON) {
            digitalWrite(water_pump_pin, LOW);
            printf("Water Pump Relay ON\n");
        } else if (GetRelayState() == WATER_PUMP_OFF) {
            digitalWrite(water_pump_pin, HIGH);
            printf("Water Pump Relay OFF\n");
        } else {
            printf("Unknown relay state %d\n", GetRelayState());
        }

        // 重置信号标志
        SetSignalReceived(false);
        lock.unlock();
    }
}