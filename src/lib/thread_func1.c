#include "thread_func1.h"

void* thread_func1(void* arg) {
    for (int i = 1; i <= 10; ++i) {
        printf("Thread 1: %d\n", i);
    }
    return NULL;
}