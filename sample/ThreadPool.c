#include <pthread.h>
#include "ThreadPool.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// 任务结构体
typedef struct Task
{
    void (*function)(void* arg);  //函数地址
    void* arg;   //参数
}Task;


// 线程池结构体
typedef struct ThreadPool
{
    // 任务队列
    Task* taskQ;
    int queueCapacity;  // 容量
    int queueSize;      // 当前任务个数
    int queueFront;     // 队头 -> 取数据
    int queueRear;      // 队尾 -> 放数据

    pthread_t managerID;    // 管理者线程ID
    pthread_t *threadIDs;   // 工作的线程ID
    int minNum;             // 最小线程数量
    int maxNum;             // 最大线程数量
    int busyNum;            // 忙的线程的个数
    int liveNum;            // 存活的线程的个数
    int exitNum;            // 要销毁的线程个数
    pthread_mutex_t mutexPool;  // 锁整个的线程池
    pthread_mutex_t mutexBusy;  // 锁busyNum变量
    pthread_cond_t notFull;     // 任务队列是不是满了
    pthread_cond_t notEmpty;    // 任务队列是不是空了

    int shutdown;           // 是不是要销毁线程池, 销毁为1, 不销毁为0
}ThreadPool;


ThreadPool* threadPoolCreate(int min,int max,int queueSize)
{
    ThreadPool* pool=(ThreadPool*)malloc(sizeof(ThreadPool));
    do
    {
        if(pool ==NULL)
        {
            break;
        }
        //malloc出工作的线程的空间
        pool->threadIDs=(pthread_t*)malloc(sizeof(pthread_t)*max);
        if(pool->threadIDs==NULL)
        {
            printf("malloc threadpool faile...\n");
        }
        memset(pool->threadIDs,0,sizeof(pthread_t)*max);

        //基础参数的初始化
        pool->maxNum=max;
        pool->minNum=min;
        pool->busyNum=0;
        pool->liveNum=min;
        pool->exitNum=0;

        //初始化锁和条件参数
        if(pthread_mutex_init(&pool->mutexPool,NULL)!=0||
           pthread_mutex_init(&pool->mutexBusy,NULL)!=0||
           pthread_cond_init(&pool->notEmpty,NULL)!=0||
           pthread_cond_init(&pool->notFull,NULL)!=0)
        {
            printf("mutex or condition init fail...");
            break;
        }

        //任务队列
        pool->taskQ=(Task*)malloc(sizeof(Task)*queueSize);
        pool->queueCapacity=queueSize;
        pool->queueSize=0;//当前任务个数
        pool->queueFront=0;
        pool->queueRear=0;

        pool->shutdown=0;

        pthread_create(&pool->managerID,NULL,manager,pool);
        for (size_t i = 0; i < min; i++)
        {
            pthread_create(&pool->threadIDs[i],NULL,worker,pool);
        }
        
        return pool;
    }while (0);

    //释放资源
    if(pool && pool->threadIDs){free(pool->threadIDs);}
    if(pool && pool->taskQ){free(pool->taskQ);}
    if(pool){free(pool);}
    

    return NULL;
}

void* manager(void* arg)
{

}
void* worker(void* arg)
{
    
}



int main()
{
    printf("hello world!");
    return 0;
}