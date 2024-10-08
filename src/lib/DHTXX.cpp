#include "DHTXX.h"

DHTXX::DHTXX() : pin_num_(0), data_buf_(0), timestamp_(0), temperature_(0.0), humidity_(0.0), exit_condition_(false)
{
    UpdateCurrentTimestamp();
}

unsigned int DHTXX::GetPinNum() const
{
    return pin_num_;
}

unsigned long long DHTXX::GetTimestamp() const
{
    return timestamp_;
}

float DHTXX::GetTemperature() const
{
    return temperature_;
}

float DHTXX::GetHumidity() const
{
    return humidity_;
}

void DHTXX::SetPinNum(const unsigned int pin_num)
{
    pin_num_ = pin_num;
}

void DHTXX::UpdateCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now().time_since_epoch();
    SetTimestamp(std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}

void DHTXX::SetTimestamp(unsigned long long timestamp)
{
    timestamp_ = timestamp;
}

bool DHTXX::StopLoop()
{
    return exit_condition_;
}

void DHTXX::Loop()
{
    std::cout << "DHTXX thread Loop()" << std::endl;
    while (!StopLoop())
    {
        /*
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

            // 高温（温度不低于50°）高湿（湿度不低于50%）环境，且距离上次翻堆超过3天，则需要翻堆
            if (dhtxxGetTemperature(&mDHT) > HIGH_TEMPERATURE && dhtxxGetHumidity(&mDHT) >= HIGH_HUMIDITY && DIFFERENCE_MORE_THAN_3_DAYS((time_t)dhtxxGetTimestamp(&mDHT), last_fandui_time))
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
            else if (dhtxxGetTemperature(&mDHT) <= LOW_TEMPERATURE)
            {
                if (stop_motor_cnt++ > STOP_MOTOR_THRESHOLD)
                {
                    stop_motor(); // 停止翻堆
                    fan_off(); // 关闭风扇
                    stop_motor_cnt = 0;
                }
            }
            else
            {
                start_motor_cnt = 0;
                stop_motor_cnt = 0;
            }

            // 湿度低于阈值，开始加湿，并翻堆
            if (dhtxxGetHumidity(&mDHT) <= LOW_HUMIDITY)
            {
                if (start_watering_cnt++ > START_WATERING_THRESHOLD)
                {
                    start_motor(); // 翻堆
                    water_pump_on(); // 加湿
                    // last_fandui_time = time(NULL);
                    start_watering_cnt = 0;
                }
            }
            else if (dhtxxGetHumidity(&mDHT) >= HIGH_HUMIDITY)
            {
                if (stop_watering_cnt++ > STOP_WATERING_THRESHOLD)
                {
                    stop_motor(); //停止翻堆
                    water_pump_off(); // 停止加湿
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
        */

        UpdateCurrentTimestamp();
        std::cout << "DHTXX thread UpdateCurrentTimestamp() " << GetTimestamp() << std::endl;
    }
}

// int dhtxxInitialize(DHT11 *self, int pin)
// {
//     self->pinNum = pin;
//     self->databuf = 0;

//     printf("PIN:\t%d\n", self->pinNum);
//     if (-1 == wiringPiSetup())
//     {
//         printf("DHTXX Setup wiringPi failed!");
//         return 1;
//     }
//     else
//     {
//         printf("DHTXX Successfully setup wiringPi!\n");
//     }

//     time_t currentTime = time(NULL);
//     if (currentTime == -1)
//     {
//         self->timestamp = -1;
//     }
//     else
//     {
//         self->timestamp = (long)currentTime;
//     }

//     return 0;
// }

// float dhtxxGetTemperature(DHT11 *self)
// {
//     return self->temperature;
// }

// float dhtxxGetHumidity(DHT11 *self)
// {
//     return self->humidity;
// }

// long dhtxxGetTimestamp(DHT11 *self)
// {
//     return self->timestamp;
// }

// void dhtxxReset(DHT11 *self)
// {
//     self->humidity = 0.0;
//     self->temperature = 0.0;
//     self->databuf = 0;

//     time_t currentTime = time(NULL);
//     if (currentTime == -1)
//     {
//         self->timestamp = -1;
//     }
//     else
//     {
//         self->timestamp = (long)currentTime;
//     }
// }

uint8 dhtxxRead(DHT11 *self)
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
        self->temperature = ((self->databuf >> 8) & 0x7f) + (self->databuf & 0xff) / 10.0 - 3.0;

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
        dhtxxReset(self);
        return 0;
    }
}
