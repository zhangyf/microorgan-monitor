#include "MarqueueThread.h"

pthread_mutex_t marqueue_mutex;
pthread_cond_t marqueue_cond;
int marqueue_signal_received = 0;

char line1_message[1024];
uint16 line1_message_len;

char line2_message[1024];
uint16 line2_message_len;

void *marqueue_thread(void *arg)
{
    // 初始化wiringPi库
    if (wiringPiSetup() == -1)
    {
        fprintf(stderr, "Failed to initialize wiringPi.\n");
        exit(1);
    }

    lcd_init(LCD_ADDR);
    // uint16 position = 0;
    // const uint16 show_delay = 500000;

    clear_lcd();

    while (1)
    {
        pthread_mutex_lock(&marqueue_mutex);
        while (!marqueue_signal_received)
        {
            pthread_cond_wait(&marqueue_cond, &marqueue_mutex);
        }
        // 清除当前显示的内容
        clear_lcd();
        lcd_loc(LINE1);
        print_string(line1_message);
        lcd_loc(LINE2);
        print_string(line2_message);

        marqueue_signal_received = 0;
        pthread_mutex_unlock(&marqueue_mutex);
    }
}

void *update_message(const char *new_message, unsigned int line)
{
    pthread_mutex_lock(&marqueue_mutex);
    if (line == LINE1)
    {
        strncpy(line1_message, new_message, sizeof(line1_message) - 1);
        line1_message_len = strlen(line1_message);
    }
    else if (line == LINE2)
    {
        strncpy(line2_message, new_message, sizeof(line2_message) - 1);
        line2_message_len = strlen(line2_message);
    }
    else
    {
        strncpy(line1_message, new_message, sizeof(line1_message) - 1);
        line1_message_len = strlen(line1_message);
    }
    marqueue_signal_received = 1;
    pthread_cond_signal(&marqueue_cond);
    pthread_mutex_unlock(&marqueue_mutex);
}
