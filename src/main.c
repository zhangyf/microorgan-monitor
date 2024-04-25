#include "DHTXX.h"
#include "StepMotor.h"

#define DHT11_PIN 25

#define STEP_MOTOR_PIN_A 26
#define STEP_MOTOR_PIN_B 27
#define STEP_MOTOR_PIN_C 28
#define STEP_MOTOR_PIN_D 29

int main()
{
    int ret = 0;

    DHT11 mDHT;
    if (dhtxxInitialize(&mDHT, DHT11_PIN) > 0)
    {
        return -1;
    }

    StepMotor mStepMotor;
    if (stepMotorInitialize(&mStepMotor, STEP_MOTOR_PIN_A, STEP_MOTOR_PIN_B, STEP_MOTOR_PIN_C, STEP_MOTOR_PIN_D) > 0)
    {
        return -1;
    }

    pinMode(mDHT.pinNum, OUTPUT); // set mode to output
    digitalWrite(mDHT.pinNum, HIGH); // output a high level 
    printf("Starting to read....\n");

    while(1)
    {
        pinMode(mDHT.pinNum, OUTPUT); // set mode to output
        digitalWrite(mDHT.pinNum, HIGH); // output a high level 
        delay(3000);
        if (dhtxxRead(&mDHT))
        {
            printf("DHT11 Sensor data read ok!\t{ \"timestamp\":\t%ld\t\"humidity\":\t%.1f%\t\"temperature\":\t%.1f C }\n", 
                        dhtxxGetTimestamp(&mDHT), dhtxxGetHumidity(&mDHT) , dhtxxGetTemperature(&mDHT));
            
            // printf("RH:%d.%d\n", (mDHT.databuf >> 24) & 0xff, (mDHT.databuf >> 16) & 0xff); 
            // printf("TMP:%d.%d\n", (mDHT.databuf >> 8) & 0xff, mDHT.databuf & 0xff);
            dhtxxReset(&mDHT);
        }
        else
        {
            printf("Sensor dosen't ans!\n");
            dhtxxReset(&mDHT);
        }
    }
    return 0;
}