#ifndef INCLUDE_DHTXX_H_
#define INCLUDE_DHTXX_H_

#include "common.h"

#define HIGH_TIME                   32
#define DHT11_PIN                   27
#define HIGH_TEMPERATURE            32
#define LOW_TEMPERATURE             25
#define HIGH_HUMIDITY               70
#define LOW_HUMIDITY                30
#define START_MOTOR_THRESHOLD       3
#define STOP_MOTOR_THRESHOLD        3
#define START_WATERING_THRESHOLD    3
#define STOP_WATERING_THRESHOLD     3

class DHTXX {
public:
    DHTXX();
    ~DHTXX() {};

    unsigned int GetPinNum() const;
    unsigned long long GetTimestamp() const;
    float GetTemperature() const;
    float GetHumidity() const;

    void SetTimestamp(unsigned long timestamp);
    void SetPinNum(const unsigned int pin_num);

    void Start();
private:
    void UpdateCurrentTimestamp();

    unsigned int pin_num_;
    unsigned long data_buf_;
    unsigned long long timestamp_;
    float temperature_;
    float humidity_;
};

// uint8 exit_condition;

// typedef struct
// {
//     int pinNum;
//     uint32 databuf;
//     float temperature;
//     float humidity;
//     long timestamp;
// } DHT11;

// int dhtxxInitialize(DHT11 *self, int pin);
// uint8 dhtxxRead(DHT11 *self);
// float dhtxxGetTemperature(DHT11 *self);
// float dhtxxGetHumidity(DHT11 *self);
// long dhtxxGetTimestamp(DHT11 *self);
// void dhtxxReset(DHT11 *self);

#endif // INCLUDE_DHTXX_H_
