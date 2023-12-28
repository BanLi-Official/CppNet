#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "ThreadPool.hpp"



void counter(void *arg)
{
    int num=*(int*)arg;
    printf("thread %ld count:::counter=%d\n",pthread_self(),num);
    sleep(2);
}

int main()
{   
    ThreadPool pool(3,10);
    
    for(int i=0;i<100;i++)
    {
        int* num=(int *)malloc(sizeof(int));
        *num=i;
        pool.addTask(Task(counter,num));
    }

    sleep(30);

    return 1;
}