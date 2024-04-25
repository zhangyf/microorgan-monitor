#ifndef INCLUDE_DHTXX_H_
#define INCLUDE_DHTXX_H_

#include <time.h>
#include "common.h"

#define HIGH_TIME 32

typedef struct {
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
long dhtXXGetTimestamp(DHT11 *self);
void dhtxxReset(DHT11 *self);

#endif // INCLUDE_DHTXX_H_