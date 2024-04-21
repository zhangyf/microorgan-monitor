#ifndef INCLUDE_DHTXX_H
#define INCLUDE_DHTXX_H

#include <wiringPi.h>

#define HIGH_TIME 32

typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef unsigned long uint32;

typedef struct {
    int pinNum;
    uint32 databuf;
    float temperature;
    float humidity;
} DHT11;

int initialize(DHT11 *self, int pin);
uint8 read(DHT11 *self);
float getTemperature(DHT11 *self);
float getHumidity(DHT11 *self);
void reset(DHT11 *self);

#endif // INCLUDE_DHTXX_H