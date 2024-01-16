#include <pthread.h>
#include "ThreadPool.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMBER 2

int num=0;

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

int threadPoolDestroy(ThreadPool *pool)
{
    if(pool ==NULL)
    {
        return -1;
    }

    //关闭线程池
    pool->shutdown=1;
    //阻塞回收管理者线程
    pthread_join(pool->managerID,NULL);
    //唤醒阻塞的消费者线程
    for(int i=0;i<pool->liveNum;i++)
    {
        pthread_cond_signal(&pool->notEmpty);
    }
    //释放内存
    if(pool->taskQ)
    {
        free(pool->taskQ);
    }
    if(pool->threadIDs)
    {
        free(pool->threadIDs);
    }
    //销毁同步资源
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_mutex_destroy(&pool->mutexPool);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);

    free(pool);
    pool=NULL;
    return 0;
}

void threadPoolAdd(ThreadPool *pool, void (*func)(void *), void *arg)
{
    Task task;
    task.function=func;
    task.arg=arg;

    pthread_mutex_lock(&pool->mutexPool);
    while(pool->queueSize == pool->queueCapacity && !pool->shutdown)
    {
        pthread_cond_wait(&pool->notFull,&pool->mutexPool);
    }
    if (pool->shutdown)
    {
        pthread_mutex_unlock(&pool->mutexPool);
        return;
    }

    //添加任务
    pool->taskQ[pool->queueRear]=task;
    pool->queueRear=(pool->queueRear+1)%pool->queueCapacity;
    pool->queueSize++;

    pthread_cond_signal(&pool->notEmpty);
    
    pthread_mutex_unlock(&pool->mutexPool);
}

int threadPoolBusyNum(ThreadPool * pool)
{
    pthread_mutex_lock(&pool->mutexPool);
    int busyNum=pool->busyNum;
    pthread_mutex_unlock(&pool->mutexPool);
    
    return busyNum;
}

int threadPoolAliveNum(ThreadPool *pool)
{
    pthread_mutex_lock(&pool->mutexPool);
    int aliveNum=pool->liveNum;
    pthread_mutex_unlock(&pool->mutexPool);

    return aliveNum;
}

void* manager(void* arg)
{
    //变量初始化
    ThreadPool *pool=(ThreadPool *)arg;
    
    //循环每3秒检测一次（在线程池没有被shutdown的情况下）
    while (!pool->shutdown)
    {
        _sleep(3);
        //取基础数据
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize=pool->queueSize;
        int LiveNum=pool->liveNum;
        int BusyNum=pool->busyNum;
        pthread_mutex_unlock(&pool->mutexPool);
            
        //创建线程
        //任务的个数>存活的线程个数 &&存活的线程<最大的线程数
        if(queueSize>LiveNum && LiveNum<pool->maxNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            int count=0;//当次已经创建的线程的数量
            for(int i=0;i<pool->maxNum && count < NUMBER 
                && pool->liveNum<pool->maxNum;i++)
            {
                if(pool->threadIDs[i]==0)
                {

                    pthread_create(&pool->threadIDs[i],NULL,worker,pool);
                    printf("thread %ld be creadted@@@@@@@@@\n",num++);
                    pool->liveNum++;
                    count++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }

        //杀死线程
        //忙的线程*2<存活的线程 && 存活的线程>最小的线程数
        if(BusyNum*2 < LiveNum && LiveNum > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum=NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            //唤醒等待notEmpty资源的线程，并诱导其自杀
            for(int i=0;i<NUMBER;i++)
            {
                pthread_cond_signal(&pool->notEmpty);
            }
        }
        
    }

    return NULL;
}

void threadExit(ThreadPool *pool)
{
    pthread_t pid=pthread_self();
    for(int i=0;i<pool->maxNum;i++)
    {
        if(pool->threadIDs[i]==pid)
        {
            pool->threadIDs[i]=0;
            printf("thread %ld Exits\n",pid);
            break;
        }
    }
    pthread_exit(NULL);
}

void *worker(void *arg)
{
    ThreadPool *pool=(ThreadPool *)arg;
    while (1)  //不断从任务队列中取任务并进行运行
    {
        //先给线程池上锁
        pthread_mutex_lock(&pool->mutexPool);
        //判断当前任务队列是否为空
        while(pool->queueSize==0&& !pool->shutdown)
        {//任务队列为空的时候阻塞线程
            //当线程调用pthread_cond_wait时，它首先会释放mutexPool互斥锁，
            //   在等待条件变量的过程中，线程会阻塞，直到其他线程调用pthread_cond_signal或pthread_cond_broadcast来
            //   通知条件变量的状态发生了改变。一旦收到信号，线程会重新获取mutexPool互斥锁，并继续执行后面的代码。
            pthread_cond_wait(&pool->notEmpty,&pool->mutexPool);

            //判断是不是要销毁线程,这个exitNum来源于管理者的设定，告诉线程们，有多少线程要自杀
            if(pool->exitNum>0) 
            {
                pool->exitNum--;
                if(pool->liveNum>pool->minNum)
                {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    threadExit(pool);//线程自杀
                }
            }
        }

        //判断线程池是否被关闭了
        if(pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            threadExit(pool);
        }

        //一切正常后，从循环任务队列中拉取任务进行运行
        Task task;
        task=pool->taskQ[pool->queueFront];

        //移动头结点
        pool->queueFront=(pool->queueFront+1)%pool->queueCapacity;
        pool->queueSize--;

        //发出notFull的信号并给pool解锁
        pthread_cond_signal(&pool->notFull);
        pthread_mutex_unlock(&pool->mutexPool);

        printf("thread %ld start working.....\n",pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);
        task.function(task.arg);
        free(task.arg);
        task.arg=NULL;
        printf("thread %ld end working.......\n",pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);

    }

    return NULL;
    
}



