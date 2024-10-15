#include "ButtonState.h"

// 中断服务例程
void buttonInterrupt(int pin)
{
    int state = digitalRead(pin);
    if (pin == FAN_BTN_PIN)
    {
        fprintf(stdout, "FAN_BTN_PIN: %s\n", state == LOW ? "Open" : "Closed");
        if (state == LOW)
        {
            fan_on();
        }
        else if (state == HIGH)
        {
            fan_off();
        }
        else
        {
            fprintf(stderr, "FAN_BTN_PIN UNKNOWN state %d\n", state);
        }
    }
    else if (pin == WATER_PUMP_BTN_PIN)
    {
        fprintf(stdout, "WATER_PUMP_BTN_PIN: %s\n", state == LOW ? "Open" : "Closed");
        if (state == LOW)
        {
            water_pump_on();
        }
        else if (state == HIGH)
        {
            water_pump_off();
        }
        else
        {
            fprintf(stderr, "WATER_PUMP_BTN_PIN UNKNOWN state %d\n", state);
        }
    }
    else if (pin == STEPPER_MOTOR_BTN_PIN)
    {
        fprintf(stdout, "STEPPER_MOTOR_BTN_PIN: %s\n", state == LOW ? "Open" : "Closed");
        if (state == LOW)
        {
            start_motor();
        }
        else if (state == HIGH)
        {
            stop_motor();
        }
        else
        {
            fprintf(stderr, "STEPPER_MOTOR_BTN_PIN UNKNOWN state %d\n", state);
        }
    }

    char msg[17];
    snprintf(msg, 17, "F:%d W:%d M:%d\n", get_fan_state(), get_water_pump_state(), get_motor_state());
    update_message(msg, LINE2);
    memset(msg, '\0', sizeof(msg));
}

void initBtns()
{
    // 设置GPIO引脚为输入模式，并启用上拉电阻
    pinMode(FAN_BTN_PIN, INPUT);
    pullUpDnControl(FAN_BTN_PIN, PUD_UP);

    pinMode(WATER_PUMP_BTN_PIN, INPUT);
    pullUpDnControl(WATER_PUMP_BTN_PIN, PUD_UP);

    pinMode(STEPPER_MOTOR_BTN_PIN, INPUT);
    pullUpDnControl(STEPPER_MOTOR_BTN_PIN, PUD_UP);

    // 设置中断服务例程
    if (wiringPiISR(FAN_BTN_PIN, INT_EDGE_BOTH, &buttonInterrupt) < 0)
    {
        fprintf(stderr, "Unable to setup ISR for FAN_BTN_PIN\n");
        exit(1);
    }

    if (wiringPiISR(WATER_PUMP_BTN_PIN, INT_EDGE_BOTH, &buttonInterrupt) < 0)
    {
        fprintf(stderr, "Unable to setup ISR for WATER_PUMP_BTN_PIN\n");
        exit(1);
    }

    if (wiringPiISR(STEPPER_MOTOR_BTN_PIN, INT_EDGE_BOTH, &buttonInterrupt) < 0)
    {
        fprintf(stderr, "Unable to setup ISR for STEPPER_MOTOR_BTN_PIN\n");
        exit(1);
    }
}