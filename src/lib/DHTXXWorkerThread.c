#include "DHTXXWorkerThread.h"

void *start(void *arg)
{
    int ret = 0;
    time_t last_fandui_time = 0L;

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
            printf("DHT11 Sensor data read ok!\t{\"timestamp\":\t%ld\t\"humidity\":\t%.1f%(%d)\t\"temperature\":\t%.1f C(%d)\t exist_condition: %d}\n",
                   dhtxxGetTimestamp(&mDHT),
                   dhtxxGetHumidity(&mDHT), NORMAL_HUMIDITY,
                   dhtxxGetTemperature(&mDHT), HIGH_TEMPERATURE,
                   exit_condition,

                   DIFFERENCE_MORE_THAN_3_DAYS((time_t)dhtxxGetTimestamp(&mDHT), last_fandui_time));

            // 高温（温度不低于50°）高湿（湿度不低于50%）环境，且距离上次翻堆超过3天，则需要翻堆
            if (dhtxxGetTemperature(&mDHT) > HIGH_TEMPERATURE 
                    && dhtxxGetHumidity(&mDHT) >= NORMAL_HUMIDITY
                    && DIFFERENCE_MORE_THAN_3_DAYS((time_t)dhtxxGetTimestamp(&mDHT), last_fandui_time))
                    {
                        start_motor(); // 翻堆
                        // fan_on 打开风扇
                        last_fandui_time = time(NULL);
                        printf("start_motor and fan_on and %d\n", last_fandui_time)
                    }
            else if (dhtxxGetTemperature(&mDHT) <= LOW_TEMPERATURE)
            {
                 stop_motor(); // 停止翻堆
                // fan_off 关闭风扇
            }

            // 湿度低于阈值，开始加湿，并翻堆
            if (dhtxxGetHumidity(&mDHT) <= LOW_HUMIDITY)
            {
                // start_motor(); 翻堆
                // water_on 加湿
                // last_fandui_time = time(NULL);
            }
            else if (dhtxxGetHumidity(&mDHT) >= NORMAL_HUMIDITY)
            {
                // stop_motor(); 停止翻堆
                // water off 停止加湿
            }

            // stop_motor();
        }
        else
        {
            printf("Sensor dosen't ans!\n");
        }
        dhtxxReset(&mDHT);
    }
    pthread_exit(NULL);
}
