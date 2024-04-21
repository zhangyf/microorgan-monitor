#include "DHTXX.h"
#include <stdio.h>

#define DHT11_PIN 25; 

int main()
{
    int ret = 0;

    DHT11 mDHT;
    if (initialize(&mDHT, DHT11_PIN) > 0)
    {
        return -1;
    }
    pinMode(mDHT.pinNum, OUTPUT); // set mode to output
    digitalWrite(mDHT.pinNum, HIGH); // output a high level 
    printf("Starting to read....\n");

    while(true)
    {
        pinMode(mDHT.pinNum, OUTPUT); // set mode to output
        digitalWrite(mDHT.pinNum, HIGH); // output a high level 
        delay(3000);
        if (read(&mDHT))
        {
            printf("Sensor data read ok!\n");
            printf("RH:%d.%d\n", (mDHT.databuf >> 24) & 0xff, (mDHT.databuf >> 16) & 0xff); 
            printf("TMP:%d.%d\n", (mDHT.databuf >> 8) & 0xff, mDHT.databuf & 0xff);
            mDHT.databuf = 0;
        }
        else
        {
            printf("Sensor dosen't ans!\n");
            mDHT.databuf = 0;
        }
    }
    return 0;
}