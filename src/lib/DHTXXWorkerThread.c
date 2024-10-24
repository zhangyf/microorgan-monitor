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
    fprintf(stdout, "DHTXX worker thread starting to read....\n");

    while (!exit_condition)
    {
        pinMode(mDHT.pinNum, OUTPUT);    // set mode to output
        digitalWrite(mDHT.pinNum, HIGH); // output a high level
        delay(3000);
        if (dhtxxRead(&mDHT))
        {

            fprintf(stdout, "DHT11 Sensor data read ok!\t{\"timestamp\":\t%ld\t\"humidity\":\t%.1f%(%d/%d)\t\"temperature\":\t%.1f C(%d/%d)\t exist_condition: %d}\n",
                   dhtxxGetTimestamp(&mDHT),
                   dhtxxGetHumidity(&mDHT), HIGH_HUMIDITY,LOW_HUMIDITY,
                   dhtxxGetTemperature(&mDHT), HIGH_TEMPERATURE,LOW_TEMPERATURE,
                   exit_condition);
                  // (dhtxxGetTemperature(&mDHT) > HIGH_TEMPERATURE),
                  // (dhtxxGetHumidity(&mDHT) >= HIGH_HUMIDITY),
                  // DIFFERENCE_MORE_THAN_3_DAYS((time_t)dhtxxGetTimestamp(&mDHT), last_fandui_time));
			char msg[17];
			
            snprintf(msg, 17, "H:%.1f% T:%.1f C", dhtxxGetHumidity(&mDHT), dhtxxGetTemperature(&mDHT));
			//fprintf(stdout, "%s\n", msg);
			update_message(msg, LINE1);
			memset(msg, '\0', sizeof(msg));

            snprintf(msg, 17, "Fa:%d Wa:%d Mo:%d", get_fan_state(), get_water_pump_state(), get_motor_state());
            update_message(msg, LINE2);
			//fprintf(stdout, "%s\n", msg);
            memset(msg, '\0', sizeof(msg));

			int fan_state = get_fan_state();
            // 高温（温度不低于50°）高湿（湿度不低于50%）环境，且距离上次翻堆超过3天，则需要翻堆
            if (dhtxxGetTemperature(&mDHT) > HIGH_TEMPERATURE 
						&& dhtxxGetHumidity(&mDHT) >= HIGH_HUMIDITY 
						&& DIFFERENCE_MORE_THAN_3_DAYS((time_t)dhtxxGetTimestamp(&mDHT), last_fandui_time)
						&& fan_state == 0)
            {
                if (start_motor_cnt++ > START_MOTOR_THRESHOLD)
                {
                    start_motor(); // 翻堆
                    fan_on(); //打开风扇
                    last_fandui_time = time(NULL);
                    fprintf(stdout, "start_motor and fan_on and %d\n", last_fandui_time);
                    start_motor_cnt = 0;
                }
            }
            else if (dhtxxGetTemperature(&mDHT) <= LOW_TEMPERATURE && fan_state == 1)
            {
                if (stop_motor_cnt++ > STOP_MOTOR_THRESHOLD)
                {
                    stop_motor(); // 停止翻堆
                    fan_off(); // 关闭风扇
                    fprintf(stdout, "stop_motor and fan_off\n");
                    stop_motor_cnt = 0;
                }
            }
            else
            {
                start_motor_cnt = 0;
                stop_motor_cnt = 0;
            }

			int water_pump_state = get_water_pump_state();
            // 湿度低于阈值，开始加湿，并翻堆
            if (dhtxxGetHumidity(&mDHT) <= LOW_HUMIDITY && water_pump_state == 0)
            {
                if (start_watering_cnt++ > START_WATERING_THRESHOLD)
                {
                    start_motor(); // 翻堆
                    water_pump_on(); // 加湿
                    last_fandui_time = time(NULL);
                    fprintf(stdout, "start_motor and water_pump_on and %d\n", last_fandui_time);
                    start_watering_cnt = 0;
                }
            }
            else if (dhtxxGetHumidity(&mDHT) >= HIGH_HUMIDITY && water_pump_state == 1)
            {
                if (stop_watering_cnt++ > STOP_WATERING_THRESHOLD)
                {
                    stop_motor(); //停止翻堆
                    water_pump_off(); // 停止加湿
                    fprintf(stdout, "stop_motor and water_pump_off\n");
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
            fprintf(stderr, "Sensor dosen't ans!\n");
        }
        dhtxxReset(&mDHT);
    }
    pthread_exit(NULL);
}
