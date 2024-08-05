#include "thread_func2.h"

void* thread_func2(void* arg) {
    for (char c = 'a'; c <= 'j'; ++c) {
        printf("Thread 2: %c\n", c);
    }
    return NULL;
}