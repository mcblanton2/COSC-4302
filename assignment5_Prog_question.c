#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int counter = 0;
sem_t mutex;
void *t1_func(void *arg)
{
    while (1)
    {
        sem_wait(&mutex);
        counter++;
        printf("t1 incremented counter: %d\n", counter);
        sem_post(&mutex);
        sleep(1);
    }
}

void *t2_func(void *arg)
{
    while (1)
    {
        sem_wait(&mutex);
        counter--;
        printf("t2 decremented counter: %d\n", counter);
        sem_post(&mutex);
        sleep(1);
    }
}

int main()
{
    pthread_t t1, t2;

    sem_init(&mutex, 0, 1);

    pthread_create(&t1, NULL, t1_func, NULL);
    pthread_create(&t2, NULL, t2_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&mutex);
    return 0;
}
