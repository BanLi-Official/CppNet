#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "ThreadPool.hpp"
#include <mutex>
#include "mutex.hpp"

pthread_mutex_t IOMutex;



void counter(void *arg)
{
    int num=*(int*)arg;

    pthread_mutex_lock(&IOMutex);
    printf("thread %ld count:::counter=%d\n", pthread_getw32threadid_np(pthread_self()),num);
    pthread_mutex_unlock(&IOMutex);

    Sleep(1000);
}

int main()
{   
    pthread_mutex_init(&IOMutex,NULL);
    ThreadPool pool(3,10);

    for(int i=0;i<100;i++)
    {
        int* num=(int *)malloc(sizeof(int));
        *num=i;
        pool.addTask(Task(counter,num));
    }

    Sleep(30000);

    return 1;
}