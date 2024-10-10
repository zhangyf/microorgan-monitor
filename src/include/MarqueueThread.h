#ifndef INCLUDE_MARQUEUE_THREAD_H
#define INCLUDE_MARQUEUE_THREAD_H

#include "common.h"
#include "I2C16X2.h"
//#include "lcd.h"

// #define LCD_RS  25
// #define LCD_E   24
// #define LCD_D4  23
// #define LCD_D5  18
// #define LCD_D6  17
// #define LCD_D7  11

#define LCD_ADDR 0x27

extern pthread_mutex_t marqueue_mutex;
extern pthread_cond_t marqueue_cond;
extern int marqueue_signal_received;

I2C16x2 lcd;
char message[1024];
uint16 message_len;
// uint16 position = 0;
//const uint16 show_delay = 500000;

void* marqueue_init();
void* marqueue_thread(void* arg);
void* update_message(const char* new_message);

#endif // INCLUDE_MARQUEUE_THREAD_H
