#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "ThreadPool.hpp"
#include <mutex>
#include "mutex.hpp"
#include <iostream>

pthread_mutex_t IOMutex;
int global_num=0;



void counter(void *arg)
{
    int num=*(int*)arg;

    pthread_mutex_lock(&IOMutex);
    printf("thread %ld count:::counter=%d\n", pthread_self(),num);
    global_num++;
    pthread_mutex_unlock(&IOMutex);

    sleep(1);
}

int main()
{   
    pthread_mutex_init(&IOMutex,NULL);
    ThreadPool pool(3,10);

    for(int i=0;i<50;i++)
    {
        int* num=(int *)malloc(sizeof(int));
        *num=i;
        pool.addTask(Task(counter,num));
    }

    while (global_num!=50)//判断所有任务都已完成
    {
        continue;
    }

    delete &pool; //销毁线程池实例
    pthread_mutex_lock(&IOMutex);
    std::cout<<"The thread pool instance is being destroyed.............................................."<<endl;
    pthread_mutex_unlock(&IOMutex);
    
    sleep(10);//等待几秒钟给线程池时间销毁

    return 1;
}