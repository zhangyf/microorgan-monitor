#ifndef INCLUDE_DHTXX_H_
#define INCLUDE_DHTXX_H_

#include "common.h"

#define HIGH_TIME 32
#define DHT11_PIN 25

#define HIGH_TEMPERATURE    32
#define LOW_TEMPERATURE     25
#define NORMAL_HUMIDITY     70
#define LOW_HUMIDITY        30

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