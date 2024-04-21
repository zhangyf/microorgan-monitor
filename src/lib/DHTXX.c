#include "DHTXX.h"
#include <stdio.h>
#include <stdlib.h>

int initialize(DHT11 *self, int pin)
{
    self->pinNum = pin;
    self->databuf = 0;

    printf("PIN:\t%d\n", self->pinNum);
    if (-1 == wiringPiSetup()) {
        printf("Setup wiringPi failed!");
        return 1;
    }
    else {
        printf("Successfully setup wiringPi!\n");
    }

    return 0;
}

uint8 read(DHT11 *self)
{
    uint8 crc; 
    uint8 i;
  
    pinMode(self->pinNum, OUTPUT); // set mode to output
    digitalWrite(self->pinNum, LOW); // output a low level 
    delay(25);
    digitalWrite(self->pinNum, HIGH); // output a high level 
    pinMode(self->pinNum, INPUT); // set mode to input
    pullUpDnControl(self->pinNum, PUD_UP);
 
    delayMicroseconds(27);
    if (digitalRead(self->pinNum) == 0) //SENSOR ANS
    {
        while (!digitalRead(self->pinNum))
            ; //wait to high
 
        for (i = 0; i < 32; i++)
        {
            while (digitalRead(self->pinNum))
                ; //data clock start
            while (!digitalRead(self->pinNum))
                ; //data start
            delayMicroseconds(HIGH_TIME);
            self->databuf *= 2;
            if (digitalRead(self->pinNum) == 1) //1
            {
                self->databuf++;
            }
        }
 
        for (i = 0; i < 8; i++)
        {
            while (digitalRead(self->pinNum))
                ; //data clock start
            while (!digitalRead(self->pinNum))
                ; //data start
            delayMicroseconds(HIGH_TIME);
            crc *= 2;  
            if (digitalRead(self->pinNum) == 1) //1
            {
                crc++;
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }
}