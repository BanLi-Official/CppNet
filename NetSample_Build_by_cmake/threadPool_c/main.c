#include <stdlib.h>
#include <stdio.h>
#include "ThreadPool.h"
#include <pthread.h>
#include <unistd.h>


void counter(void *arg)
{
    int num=*(int*)arg;
    printf("thread %ld count:::counter=%d\n",pthread_self(),num);
    sleep(2);
}

int main()
{
    printf("hello world");
    ThreadPool *pool=threadPoolCreate(3,10,100);
    for(int i=0;i<100;i++)
    {
        int* num=(int *)malloc(sizeof(int));
        *num=i;
        threadPoolAdd(pool,counter,num);
    }

    sleep(30);

    threadPoolDestroy(pool);

    return 1;
}