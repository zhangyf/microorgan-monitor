#include "DHTXXWorkerThread.h"

void *start(void *arg)
{
    int ret = 0;
    int start_motor_cnt = 0;
    int stop_motor_cnt = 0;
    int start_watering_cnt = 0;
    int stop_watering_cnt = 0;
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
            printf("DHT11 Sensor data read ok!\t{\"timestamp\":\t%ld\t\"humidity\":\t%.1f%(%d)\t\"temperature\":\t%.1f C(%d)\t exist_condition: %d} %d %d %d\n",
                   dhtxxGetTimestamp(&mDHT),
                   dhtxxGetHumidity(&mDHT), HIGH_HUMIDITY,
                   dhtxxGetTemperature(&mDHT), HIGH_TEMPERATURE,
                   exit_condition,
                   (dhtxxGetTemperature(&mDHT) > HIGH_TEMPERATURE),
                   (dhtxxGetHumidity(&mDHT) >= HIGH_HUMIDITY),
                   DIFFERENCE_MORE_THAN_3_DAYS((time_t)dhtxxGetTimestamp(&mDHT), last_fandui_time));

			int relay_state = get_relay_state();
            // 高温（温度不低于50°）高湿（湿度不低于50%）环境，且距离上次翻堆超过3天，则需要翻堆
            if (dhtxxGetTemperature(&mDHT) > HIGH_TEMPERATURE 
						&& dhtxxGetHumidity(&mDHT) >= HIGH_HUMIDITY 
						&& DIFFERENCE_MORE_THAN_3_DAYS((time_t)dhtxxGetTimestamp(&mDHT), last_fandui_time)
						&& relay_state == FAN_OFF)
            {
                if (start_motor_cnt++ > START_MOTOR_THRESHOLD)
                {
                    start_motor(); // 翻堆
                    fan_on(); //打开风扇
                    last_fandui_time = time(NULL);
                    printf("start_motor and fan_on and %d\n", last_fandui_time);
                    start_motor_cnt = 0;
                }
            }
            else if (dhtxxGetTemperature(&mDHT) <= LOW_TEMPERATURE && relay_state == FAN_ON)
            {
                if (stop_motor_cnt++ > STOP_MOTOR_THRESHOLD)
                {
                    stop_motor(); // 停止翻堆
                    fan_off(); // 关闭风扇
                    printf("stop_motor and fan_off\n");
                    stop_motor_cnt = 0;
                }
            }
            else
            {
                start_motor_cnt = 0;
                stop_motor_cnt = 0;
            }

            // 湿度低于阈值，开始加湿，并翻堆
            if (dhtxxGetHumidity(&mDHT) <= LOW_HUMIDITY && relay_state == WATER_PUMP_OFF)
            {
                if (start_watering_cnt++ > START_WATERING_THRESHOLD)
                {
                    start_motor(); // 翻堆
                    water_pump_on(); // 加湿
                    last_fandui_time = time(NULL);
                    printf("start_motor and water_pump_on and %d\n", last_fandui_time);
                    start_watering_cnt = 0;
                }
            }
            else if (dhtxxGetHumidity(&mDHT) >= HIGH_HUMIDITY && relay_state == WATER_PUMP_ON)
            {
                if (stop_watering_cnt++ > STOP_WATERING_THRESHOLD)
                {
                    stop_motor(); //停止翻堆
                    water_pump_off(); // 停止加湿
                    printf("stop_motor and water_pump_off\n");
                    stop_watering_cnt = 0;
                }
            }
            else 
            {
                start_watering_cnt = 0;
                stop_watering_cnt = 0;
            }
        }
        else
        {
            printf("Sensor dosen't ans!\n");
        }
        dhtxxReset(&mDHT);
    }
    pthread_exit(NULL);
}
