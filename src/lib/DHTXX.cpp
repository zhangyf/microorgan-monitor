#include "DHTXX.h"
#include "Relay.h"

microorgan_monitor::DHTXX::DHTXX() : pin_num_(0), timestamp_(0), temperature_(0.0), humidity_(0.0), exit_condition_(false)
{
    UpdateCurrentTimestamp();
    std::shared_ptr<unsigned long> data_buf_ = std::make_shared<unsigned long>(0);
}

// Getter functions
unsigned int microorgan_monitor::DHTXX::GetPinNum() const
{
    return pin_num_;
}

unsigned long long microorgan_monitor::DHTXX::GetTimestamp() const
{
    return timestamp_;
}

float microorgan_monitor::DHTXX::GetTemperature() const
{
    return temperature_;
}

float microorgan_monitor::DHTXX::GetHumidity() const
{
    return humidity_;
}

unsigned long microorgan_monitor::DHTXX::GetDataBuf()
{
    return *data_buf_;
}

// Setter functions
void microorgan_monitor::DHTXX::SetPinNum(const unsigned int pin_num)
{
    pin_num_ = pin_num;
}

void microorgan_monitor::DHTXX::UpdateCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now().time_since_epoch();
    SetTimestamp(std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}

void microorgan_monitor::DHTXX::SetTimestamp(unsigned long long timestamp)
{
    timestamp_ = timestamp;
}

void microorgan_monitor::DHTXX::SetHumidity(const float humidity) 
{
    humidity_ = humidity;
}

void microorgan_monitor::DHTXX::SetTemperature(const float temperature)
{
    temperature_ = temperature;
}

void microorgan_monitor::DHTXX::Reset()
{
    SetHumidity(0.0);
    SetTemperature(0.0);
    SetTimestamp(0);
    data_buf_ = std::make_shared<unsigned long>(0);
    exit_condition_ = false;
}

void microorgan_monitor::DHTXX::IncDataBuf()
{
    ++(*data_buf_);
}

bool microorgan_monitor::DHTXX::StopLoop()
{
    return exit_condition_;
}

void microorgan_monitor::DHTXX::Loop()
{
    std::cout << "DHTXX thread Loop()" << std::endl;
    while (!StopLoop())
    {
        pinMode(GetPinNum(), OUTPUT);    // set mode to output
        digitalWrite(GetPinNum(), HIGH); // output a high level
        delay(3000);

        if (ReadData())
        {
            printf("DHT11 Sensor data read ok!\t{\"timestamp\":\t%ld\t\"humidity\":\t%.1f%(%d)\t\"temperature\":\t%.1f C(%d)\t exist_condition: %d} %d %d %d\n",
                   GetTimestamp(),
                   GetHumidity(), HIGH_HUMIDITY,
                   GetTemperature(), HIGH_TEMPERATURE,
                   StopLoop(),
                   (GetTemperature() > HIGH_TEMPERATURE),
                   (GetHumidity() >= HIGH_HUMIDITY),
                   DIFFERENCE_MORE_THAN_3_DAYS((time_t)GetTimestamp(), last_fandui_time));

            // 高温（温度不低于50°）高湿（湿度不低于50%）环境，且距离上次翻堆超过3天，则需要翻堆
            if (GetTemperature() > HIGH_TEMPERATURE && GetHumidity() >= HIGH_HUMIDITY && DIFFERENCE_MORE_THAN_3_DAYS((time_t)GetTimestamp(), last_fandui_time))
            {
                if (start_motor_cnt++ > START_MOTOR_THRESHOLD)
                {
                    StartMotor(); // 翻堆
                    FanOn(); //打开风扇
                    last_fandui_time = time(NULL);
                    printf("start_motor and FanOn and %d\n", last_fandui_time);
                    start_motor_cnt = 0;
                }
            }
            else if (GetTemperature() <= LOW_TEMPERATURE)
            {
                if (stop_motor_cnt++ > STOP_MOTOR_THRESHOLD)
                {
                    StopMotor(); // 停止翻堆
                    FanOff(); // 关闭风扇
                    stop_motor_cnt = 0;
                }
            }
            else
            {
                start_motor_cnt = 0;
                stop_motor_cnt = 0;
            }

            // 湿度低于阈值，开始加湿，并翻堆
            if (GetHumidity() <= LOW_HUMIDITY)
            {
                if (start_watering_cnt++ > START_WATERING_THRESHOLD)
                {
                    StartMotor(); // 翻堆
                    WaterPumpOn(); // 加湿
                    // last_fandui_time = time(NULL);
                    start_watering_cnt = 0;
                }
            }
            else if (GetHumidity() >= HIGH_HUMIDITY)
            {
                if (stop_watering_cnt++ > STOP_WATERING_THRESHOLD)
                {
                    StopMotor(); //停止翻堆
                    WaterPumpOff(); // 停止加湿
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
        Reset();

        UpdateCurrentTimestamp();
        std::cout << "DHTXX thread UpdateCurrentTimestamp() " << GetTimestamp() << std::endl;
        break;
    }
}

bool microorgan_monitor::DHTXX::ReadData()
{
    unsigned char crc;

    pinMode(GetPinNum(), OUTPUT);   // set mode to output
    digitalWrite(GetPinNum(), LOW); // output a low level
    delay(25);
    digitalWrite(GetPinNum(), HIGH); // output a high level
    pinMode(GetPinNum(), INPUT);     // set mode to input
    pullUpDnControl(GetPinNum(), PUD_UP);

    delayMicroseconds(27);

    if (digitalRead(GetPinNum()) == 0) // SENSOR ANS
    {
        while (!digitalRead(GetPinNum()))
            ; // wait to high

        for (int i = 0; i < 32; i++)
        {
            while (digitalRead(GetPinNum()))
                ; // data clock start
            while (!digitalRead(GetPinNum()))
                ; // data start
            delayMicroseconds(HIGH_TIME);
            (*data_buf_) *= 2;
            if (digitalRead(GetPinNum()) == 1) // 1
            {
                IncDataBuf();
            }
        }

        for (int i = 0; i < 8; i++)
        {
            while (digitalRead(GetPinNum()))
                ; // data clock start
            while (!digitalRead(GetPinNum()))
                ; // data start
            delayMicroseconds(HIGH_TIME);
            crc *= 2;
            if (digitalRead(GetPinNum()) == 1) // 1
            {
                crc++;
            }
        }

        SetHumidity(((GetDataBuf() >> 24) & 0xff) + ((GetDataBuf() >> 16) & 0xff) / 1000.0);
        SetTemperature(((GetDataBuf() >> 8) & 0x7f) + (GetDataBuf() & 0xff) / 10.0 - 3.0);
        UpdateCurrentTimestamp();

        return true;
    }
    else
    {
        Reset();
        return false;
    }
   crc++;
   return true;
}
