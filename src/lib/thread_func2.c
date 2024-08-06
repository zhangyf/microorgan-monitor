#include "thread_func2.h"

void* thread_func2(void* arg) {
    while (!exit_condition) {
        sem_wait(&sem_on);

        for (int i = 1; i <= 10; ++i) {
            printf("Thread 2: %d\n", i);
        }

        sem_post(&sem_on);
    }
    pthread_exit(NULL);
}