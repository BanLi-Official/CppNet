#include "ThreadPool.hpp"
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>



using namespace std;

ThreadPool::ThreadPool(int min, int max)
{


    //实例化任务队列
    task=new taskQ;
    do
    {
        //初始化线程池
        this->m_minNum=min;
        this->m_maxNum=max;
        busy_Num=0;
        live_Num=min;

        //根据最大的线程上限给线程数组分配内存
        threadIds=new pthread_t[m_maxNum];
        if(threadIds==nullptr)
        {
            cout<<"线程初始化失败"<<endl;
            break;
        }
        //初始化
        memset(threadIds,0,sizeof(pthread_t)*m_maxNum);
        //初始化互斥锁，条件变量。用于互斥与同步
        if(pthread_mutex_init(&this->mutexPool,NULL)!=0||
            pthread_cond_init(&this->notEmpty,NULL)!=0)
        {
            cout<<"互斥锁和条件变量初始化失败"<<endl;
            break;
        }

        //创建线程
        //根据最小的线程个数创建工作线程
        int count=0;
        for(int i=0;i<m_minNum;i++)
        {
            pthread_create(&threadIds[i],NULL,worker,this);
            cout<<"创建子线程，id="<<to_string(threadIds[i])<<endl;
            
        }

        //创建管理者进程
        pthread_create(&ManagerId,NULL,manager,this);
    }while (0);
    


}

ThreadPool::~ThreadPool()
{
    shutdown = 1;
    // 销毁管理者线程
    pthread_join(ManagerId, NULL);
    // 唤醒所有消费者线程
    for (int i = 0; i < live_Num; ++i)
    {
        pthread_cond_signal(&notEmpty);
    }

    if (task) delete task;
    if (threadIds) delete[]threadIds;
    pthread_mutex_destroy(&mutexPool);
    pthread_cond_destroy(&notEmpty);




}

void ThreadPool::addTask(Task a)
{

    if (shutdown)
    {
        return;
    }
    // 添加任务，不需要加锁，任务队列中有锁
    task->inputTask(a);
    // 唤醒工作的线程
    pthread_cond_signal(&notEmpty);

}

void *ThreadPool::worker(void *arg)
{
    ThreadPool * pool=static_cast<ThreadPool *>(arg);
    while (1)//不断从任务列表拉取任务运行
    {
        //先加锁
        pthread_mutex_lock(&pool->mutexPool);
        while(pool->task->getTaskNum()==0&&!pool->shutdown)
        {
            cout << "thread " << to_string(pthread_self()) << " waiting..." << endl;
            //阻塞线程
            pthread_cond_wait(&pool->notEmpty,&pool->mutexPool);

            //解除阻塞之后判断是否要销毁线程
            if(pool->exit_Num>0)
            {
                pool->exit_Num--;
                if (pool->live_Num>pool->m_minNum)
                {
                    pool->live_Num--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    pool->threadExit();
                }
                
            }
            
        }
        //判断线程池是不是被关闭了
        if(pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            pool->threadExit();
        }

        //从任务队列中取出一个任务
        Task task= pool->task->getTask();
        pool->busy_Num++;
        pthread_mutex_unlock(&pool->mutexPool);

        cout << "thread " << to_string(pthread_self()) << " start working..." << endl;
        task.function(task.arg);
        delete task.arg;
        task.arg=nullptr;
        cout << "thread " << to_string(pthread_self()) << " end working..." << endl;


        //任务处理结束
        pthread_mutex_lock(&pool->mutexPool);
        pool->busy_Num--;
        pthread_mutex_unlock(&pool->mutexPool);


    }
    

    return nullptr;
}

void *ThreadPool::manager(void *arg)
{
    ThreadPool* pool = static_cast<ThreadPool*>(arg);
    // 如果线程池没有关闭, 就一直检测
    while (!pool->shutdown)
    {
        // 每隔5s检测一次
        sleep(5);
        // 取出线程池中的任务数和线程数量
        //  取出工作的线程池数量
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->task->getTaskNum();
        int liveNum = pool->live_Num;
        int busyNum = pool->busy_Num;
        pthread_mutex_unlock(&pool->mutexPool);

        // 创建线程
        const int NUMBER = 2;
        // 当前任务个数>存活的线程数 && 存活的线程数<最大线程个数
        if (queueSize > liveNum && liveNum < pool->m_maxNum)
        {
            // 线程池加锁
            pthread_mutex_lock(&pool->mutexPool);
            int num = 0;
            for (int i = 0; i < pool->m_maxNum && num < NUMBER
                && pool->live_Num < pool->m_maxNum; ++i)
            {
                if (pool->threadIds[i] == 0)
                {
                    pthread_create(&pool->threadIds[i], NULL, worker, pool);
                    num++;
                    pool->live_Num++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }

        // 销毁多余的线程
        // 忙线程*2 < 存活的线程数目 && 存活的线程数 > 最小线程数量
        if (busyNum * 2 < liveNum && liveNum > pool->m_minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exit_Num = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            for (int i = 0; i < NUMBER; ++i)
            {
                pthread_cond_signal(&pool->notEmpty);
            }
        }
    }


    return nullptr;
}

void ThreadPool::threadExit()
{
    pthread_t tid = pthread_self();
    for (int i = 0; i < m_maxNum; ++i)
    {
        if (threadIds[i] == tid)
        {
            cout << "threadExit() function: thread " 
                << to_string(pthread_self()) << " exiting..." << endl;
            threadIds[i] = 0;
            break;
        }
    }
    pthread_exit(NULL);

}
