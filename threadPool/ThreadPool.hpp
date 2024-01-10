#include "taskQueue.hpp"
#include "pthread.h"


class ThreadPool
{
public:
    //构造函数
    ThreadPool(int min,int max);

    //析构函数
    ~ThreadPool();

    //添加任务
    void addTask(Task a);

    //获取忙线程的个数
    inline int getBusyNumber()
    {
        return this->busy_Num;
    }

    //获取活着的线程个数
    inline int getLiveNumber()
    {
        return this->live_Num;
    }

private:
    //工作任务函数
    static void* worker(void *arg);

    //管理者线程函数
    static void* manager(void *arg);

    //线程退出函数
    void threadExit();

private:
    taskQ *task;
    int m_minNum;
    int m_maxNum;
    int live_Num;
    int busy_Num;
    int exit_Num;
    pthread_mutex_t mutexPool;
    pthread_cond_t notEmpty;
    pthread_t ManagerId;
    pthread_t* threadIds;
    bool shutdown=false;
    static const int NUMBER=2;
};