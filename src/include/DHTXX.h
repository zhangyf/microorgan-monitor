#ifndef INCLUDE_DHTXX_H_
#define INCLUDE_DHTXX_H_

#include "common.h"

#define HIGH_TIME                   32
#define DHT11_PIN                   29

#define HIGH_TEMPERATURE            40
#define LOW_TEMPERATURE             20
#define HIGH_HUMIDITY               90
#define LOW_HUMIDITY                40
#define START_MOTOR_THRESHOLD       3
#define STOP_MOTOR_THRESHOLD        3
#define START_WATERING_THRESHOLD    3
#define STOP_WATERING_THRESHOLD     3

uint8 exit_condition;

typedef struct
{
    int pinNum;
    uint32 databuf;
    float temperature;
    float humidity;
    long timestamp;
} DHT11;

int dhtxxInitialize(DHT11 *self, int pin);
uint8 dhtxxRead(DHT11 *self);
float dhtxxGetTemperature(DHT11 *self);
float dhtxxGetHumidity(DHT11 *self);
long dhtxxGetTimestamp(DHT11 *self);
void dhtxxReset(DHT11 *self);

#endif // INCLUDE_DHTXX_H_
