#include <stdio.h>
#include <pthread.h>

void* worker(void* arg)
{
    (void)arg;

    printf("Worker thread running\n");

    return NULL;
}

int main()
{
    pthread_t thread;

    pthread_create(&thread, NULL, worker, NULL);

    pthread_join(thread, NULL);

    printf("Main thread done\n");

    return 0;
}
