#include "DHTXXWorkerThread.h"

void *start(void *arg)
{
    int ret = 0;

    DHT11 mDHT;
    if (dhtxxInitialize(&mDHT, DHT11_PIN) > 0)
    {
        pthread_exit(NULL);
    }

    pinMode(mDHT.pinNum, OUTPUT);    // set mode to output
    digitalWrite(mDHT.pinNum, HIGH); // output a high level
    printf("DHTXX worker thread starting to read....\n");

    while (!exit_condition)
    {
        pinMode(mDHT.pinNum, OUTPUT);    // set mode to output
        digitalWrite(mDHT.pinNum, HIGH); // output a high level
        delay(3000);
        if (dhtxxRead(&mDHT))
        {
            printf("DHT11 Sensor data read ok!\t{ \"timestamp\":\t%ld\t\"humidity\":\t%.1f%\t\"temperature\":\t%.1f C }\n",
                   dhtxxGetTimestamp(&mDHT), dhtxxGetHumidity(&mDHT), dhtxxGetTemperature(&mDHT));
            dhtxxReset(&mDHT);

            // TODO: 某些情况下需要启动电机

            // start_motor();
            
            // 某些情况下需要停止电机

            // stop_motor();
        }
        else
        {
            printf("Sensor dosen't ans!\n");
            dhtxxReset(&mDHT);
        }
    }
    pthread_exit(NULL);
}