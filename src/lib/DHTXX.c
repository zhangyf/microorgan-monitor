#include "DHTXX.h"
#include <stdio.h>
#include <stdlib.h>

int initialize(DHT11 *self, int pin)
{
    self->pinNum = pin;
    self->databuf = 0;

    printf("PIN:\t%d\n", self->pinNum);
    if (-1 == wiringPiSetup())
    {
        printf("Setup wiringPi failed!");
        return 1;
    }
    else
    {
        printf("Successfully setup wiringPi!\n");
    }

    time_t currentTime = time(NULL);
    if (currentTime == -1)
    {
        self->timestamp = -1;
    }
    else
    {
        self->timestamp = (long)currentTime;
    }

    return 0;
}

float getTemperature(DHT11 *self)
{
    return self->temperature;
}

float getHumidity(DHT11 *self)
{
    return self->humidity;
}

long getTimestamp(DHT11 *self)
{
    return self->timestamp;
}

void reset(DHT11 *self)
{
    self->humidity = 0.0;
    self->temperature = 0.0;
    self->databuf = 0;

    time_t currentTime = time(NULL);
    if (currentTime == -1)
    {
        self->timestamp = -1;
    }
    else
    {
        self->timestamp = (long)currentTime;
    }
}

uint8 read(DHT11 *self)
{
    uint8 crc;
    uint8 i;

    pinMode(self->pinNum, OUTPUT);   // set mode to output
    digitalWrite(self->pinNum, LOW); // output a low level
    delay(25);
    digitalWrite(self->pinNum, HIGH); // output a high level
    pinMode(self->pinNum, INPUT);     // set mode to input
    pullUpDnControl(self->pinNum, PUD_UP);

    delayMicroseconds(27);
    if (digitalRead(self->pinNum) == 0) // SENSOR ANS
    {
        while (!digitalRead(self->pinNum))
            ; // wait to high

        for (i = 0; i < 32; i++)
        {
            while (digitalRead(self->pinNum))
                ; // data clock start
            while (!digitalRead(self->pinNum))
                ; // data start
            delayMicroseconds(HIGH_TIME);
            self->databuf *= 2;
            if (digitalRead(self->pinNum) == 1) // 1
            {
                self->databuf++;
            }
        }

        for (i = 0; i < 8; i++)
        {
            while (digitalRead(self->pinNum))
                ; // data clock start
            while (!digitalRead(self->pinNum))
                ; // data start
            delayMicroseconds(HIGH_TIME);
            crc *= 2;
            if (digitalRead(self->pinNum) == 1) // 1
            {
                crc++;
            }
        }

        self->humidity = ((self->databuf >> 24) & 0xff) + ((self->databuf >> 16) & 0xff) / 1000.0;
        self->temperature = ((self->databuf >> 8) & 0x7f) + (self->databuf & 0xff) / 10.0;

        time_t currentTime = time(NULL);
        if (currentTime == -1)
        {
            self->timestamp = -1;
        }
        else
        {
            self->timestamp = (long)currentTime;
        }
        return 1;
    }
    else
    {
        reset(self);
        return 0;
    }
}
