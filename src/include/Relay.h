#ifndef INCLUDE_RELAY_H
#define INCLUDE_RELAY_H

#include "common.h"

namespace microorgan_monitor 
{
extern std::mutex relay_mtx;
extern sem_t relay_sem;

#define FAN_PIN         28
#define WATER_PUMP_PIN  29
#define RELAY_STATE_OFF	0
#define FAN_ON          1
#define FAN_OFF         2
#define WATER_PUMP_ON   3 
#define WATER_PUMP_OFF  4 

class Relay
{
public:
    Relay()  : fan_pin_num_(0), water_pump_pin_num_(0), relay_state_(RELAY_STATE_OFF), signal_received_(false) {};
    ~Relay() {};

    void FanOn();
    void FanOff();
    void WaterPumpOn();
    void WaterPumpOff();
    void Loop();

    unsigned int GetFanPinNum() const;
    unsigned int GetWaterPumpPinNum() const;
    unsigned int GetRelayState() const;
    bool GetSignalReceived() const;

    void SetFanPinNum(const unsigned int fan_pin_num);
    void SetWaterPumpPinNum(const unsigned int water_pump_pin_num);
    void SetRelayState(unsigned int relay_state);
    void SetSignalReceived(bool signal_received);

private:
    unsigned int fan_pin_num_;
    unsigned int water_pump_pin_num_;
    unsigned int relay_state_;
    bool signal_received_;

};
}

#endif // INCLUDE_RELAY_H
