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

uint8 read(void)
{
    uint8 crc; 
    uint8 i;
  
    pinMode(pinNumber, OUTPUT); // set mode to output
    digitalWrite(pinNumber, LOW); // output a low level 
    delay(25);
    digitalWrite(pinNumber, HIGH); // output a high level 
    pinMode(pinNumber, INPUT); // set mode to input
    pullUpDnControl(pinNumber, PUD_UP);
 
    delayMicroseconds(27);
    if (digitalRead(pinNumber) == 0) //SENSOR ANS
    {
        while (!digitalRead(pinNumber))
            ; //wait to high
 
        for (i = 0; i < 32; i++)
        {
            while (digitalRead(pinNumber))
                ; //data clock start
            while (!digitalRead(pinNumber))
                ; //data start
            delayMicroseconds(HIGH_TIME);
            databuf *= 2;
            if (digitalRead(pinNumber) == 1) //1
            {
                databuf++;
            }
        }
 
        for (i = 0; i < 8; i++)
        {
            while (digitalRead(pinNumber))
                ; //data clock start
            while (!digitalRead(pinNumber))
                ; //data start
            delayMicroseconds(HIGH_TIME);
            crc *= 2;  
            if (digitalRead(pinNumber) == 1) //1
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