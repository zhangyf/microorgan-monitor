#include "MarqueueThread.h"

char message[1024];
pthread_mutex_t marqueue_mutex;
pthread_cond_t marqueue_cond;
uint16 message_len;

// void* marqueue_init() {

//     if (wiringPiSetup() == -1) {
//         fprintf(stderr, "Failed to initialize wiringPi.\n");
//         exit(1);
//     }

//     pthread_mutex_init(&marqueue_mutex, NULL);
//     pthread_cond_init(&marqueue_cond, NULL);

//     lcd = lcdInit(2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

//     if (lcd == -1) {
//         printf("Failed to initialize LCD\n");
//         exit(1);
//     }

//     // 清除LCD屏幕
//     lcdClear(lcd);
// }

void* marqueue_thread(void* arg) {
    // 初始化wiringPi库
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "Failed to initialize wiringPi.\n");
        exit(1);
    }

    lcd_init(LCD_ADDR);
	uint16 position = 0;
	const uint16 show_delay = 500000;

    clear_lcd();

    while (1) {
        pthread_mutex_lock(&marqueue_mutex);
        // 清除当前显示的内容
        // lcdPosition(lcd, position, 0);
        // lcdPuts(lcd, "                "); // 用空格覆盖当前内容
        clear_lcd();
        lcd_loc(LINE1);
        print_string(message);

        // // 更新显示位置
        // position = (position + 1) % 16;

        // // 在新位置显示信息
        // lcdPosition(lcd, position, 0);
        // lcdPuts(lcd, message);

        pthread_mutex_unlock(&marqueue_mutex);

        // 延迟一段时间
        usleep(show_delay); // 延迟500毫秒
    }
}

void* update_message(const char* new_message) {
    pthread_mutex_lock(&marqueue_mutex);
    strncpy(message, new_message, sizeof(message) - 1);
    message_len = strlen(message);
    pthread_cond_signal(&marqueue_cond);
    pthread_mutex_unlock(&marqueue_mutex);
}
