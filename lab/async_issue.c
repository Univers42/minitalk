#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int global_counter = 0;

void *increment_thread(void *arg)
{
    (void)arg;
    char buf[100];
    while (1)
    {
        global_counter++; // No lock, possible race
        int len = snprintf(buf, sizeof(buf), "[INC] Counter: %d\n", global_counter);
        write(STDOUT_FILENO, buf, len);
        usleep(10000);
    }
    return NULL;
}

void *decrement_thread(void *arg)
{
    (void)arg;
    char buf[100];
    while (1)
    {
        global_counter--; // No lock, possible race
        int len = snprintf(buf, sizeof(buf), "[DEC] Counter: %d\n", global_counter);
        write(STDOUT_FILENO, buf, len);
        usleep(10000);
    }
    return NULL;
}

int main(void)
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment_thread, NULL);
    pthread_create(&t2, NULL, decrement_thread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
