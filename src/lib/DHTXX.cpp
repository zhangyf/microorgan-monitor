#include "DHTXX.h"

DHTXX::DHTXX() : pin_num_(0), data_buf_(0), timestamp_(0), temperature_(0.0), humidity_(0.0) {
    UpdateCurrentTimestamp();
}

unsigned int DHTXX::GetPinNum() const {
    return pin_num_;
}

unsigned long long DHTXX::GetTimestamp() const {
    return timestamp_;
}

float DHTXX::GetTemperature() const {
    return temperature_;
}

float DHTXX::GetHumidity() const {
    return humidity_;
}

void DHTXX::SetPinNum(const unsigned int pin_num) {
    pin_num_ = pin_num;
}

void DHTXX::UpdateCurrentTimestamp() {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    timestamp_ = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}

void DHTXX::Start() {
    std::cout << "DHTXX thread Start()" << std::endl;
    UpdateCurrentTimestamp();
    std::cout << "DHTXX thread UpdateCurrentTimestamp() " << GetTimestamp() << std::endl;
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

// uint8 dhtxxRead(DHT11 *self)
// {
//     uint8 crc;
//     uint8 i;

//     pinMode(self->pinNum, OUTPUT);   // set mode to output
//     digitalWrite(self->pinNum, LOW); // output a low level
//     delay(25);
//     digitalWrite(self->pinNum, HIGH); // output a high level
//     pinMode(self->pinNum, INPUT);     // set mode to input
//     pullUpDnControl(self->pinNum, PUD_UP);

//     delayMicroseconds(27);
//     if (digitalRead(self->pinNum) == 0) // SENSOR ANS
//     {
//         while (!digitalRead(self->pinNum))
//             ; // wait to high

//         for (i = 0; i < 32; i++)
//         {
//             while (digitalRead(self->pinNum))
//                 ; // data clock start
//             while (!digitalRead(self->pinNum))
//                 ; // data start
//             delayMicroseconds(HIGH_TIME);
//             self->databuf *= 2;
//             if (digitalRead(self->pinNum) == 1) // 1
//             {
//                 self->databuf++;
//             }
//         }

//         for (i = 0; i < 8; i++)
//         {
//             while (digitalRead(self->pinNum))
//                 ; // data clock start
//             while (!digitalRead(self->pinNum))
//                 ; // data start
//             delayMicroseconds(HIGH_TIME);
//             crc *= 2;
//             if (digitalRead(self->pinNum) == 1) // 1
//             {
//                 crc++;
//             }
//         }

//         self->humidity = ((self->databuf >> 24) & 0xff) + ((self->databuf >> 16) & 0xff) / 1000.0;
//         self->temperature = ((self->databuf >> 8) & 0x7f) + (self->databuf & 0xff) / 10.0 - 3.0;

//         time_t currentTime = time(NULL);
//         if (currentTime == -1)
//         {
//             self->timestamp = -1;
//         }
//         else
//         {
//             self->timestamp = (long)currentTime;
//         }
//         return 1;
//     }
//     else
//     {
//         dhtxxReset(self);
//         return 0;
//     }
// }
