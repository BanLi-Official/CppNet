#include "ThreadPool.hpp"
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <pthread.h>


using namespace std;

ThreadPool::ThreadPool(int min, int max)
{


    //ʵ�����������
    task=new taskQ;
    do
    {
        //��ʼ���̳߳�
        this->m_minNum=min;
        this->m_maxNum=max;
        busy_Num=0;
        live_Num=min;

        //���������߳����޸��߳���������ڴ�
        threadIds=new pthread_t[m_maxNum];
        if(threadIds==nullptr)
        {
            cout<<"�̳߳�ʼ��ʧ��"<<endl;
            break;
        }
        //��ʼ��
        memset(threadIds,0,sizeof(pthread_t)*m_maxNum);
        //��ʼ�����������������������ڻ�����ͬ��
        if(pthread_mutex_init(&this->mutexPool,NULL)!=0||
            pthread_cond_init(&this->notEmpty,NULL)!=0)
        {
            cout<<"������������������ʼ��ʧ��"<<endl;
            break;
        }

        //�����߳�
        //������С���̸߳������������߳�
        int count=0;
        for(int i=0;i<m_minNum;i++)
        {
            pthread_create(&threadIds[i],NULL,worker,this);
            cout<<"�������̣߳�id="<<to_string(threadIds[i].x)<<endl;
            
        }

        //���������߽���
        pthread_create(&ManagerId,NULL,manager,this);
    }while (0);
    


}

ThreadPool::~ThreadPool()
{
    shutdown = 1;
    // ���ٹ������߳�
    pthread_join(ManagerId, NULL);
    // ���������������߳�
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
    // ������񣬲���Ҫ�������������������
    task->inputTask(a);
    // ���ѹ������߳�
    pthread_cond_signal(&notEmpty);

}

void *ThreadPool::worker(void *arg)
{
    ThreadPool * pool=static_cast<ThreadPool *>(arg);
    while (1)//���ϴ������б���ȡ��������
    {
        //�ȼ���
        pthread_mutex_lock(&pool->mutexPool);
        while(pool->task->getTaskNum()==0&&!pool->shutdown)
        {
            cout << "thread " << to_string(pthread_self().x) << " waiting..." << endl;
            //�����߳�
            pthread_cond_wait(&pool->notEmpty,&pool->mutexPool);

            //�������֮���ж��Ƿ�Ҫ�����߳�
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
        //�ж��̳߳��ǲ��Ǳ��ر���
        if(pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            pool->threadExit();
        }

        //�����������ȡ��һ������
        Task task= pool->task->getTask();
        pool->busy_Num++;
        pthread_mutex_unlock(&pool->mutexPool);

        cout << "thread " << to_string(pthread_self().x) << " start working..." << endl;

        task.function(task.arg);
        //delete task.arg;
        task.arg=nullptr;
        cout << "thread " << to_string(pthread_self().x) << " end working..." << endl;


        //���������
        pthread_mutex_lock(&pool->mutexPool);
        pool->busy_Num--;
        pthread_mutex_unlock(&pool->mutexPool);


    }
    

    return nullptr;
}

void *ThreadPool::manager(void *arg)
{
    ThreadPool* pool = static_cast<ThreadPool*>(arg);
    // ����̳߳�û�йر�, ��һֱ���
    while (!pool->shutdown)
    {
        // ÿ��5s���һ��
        Sleep(5000);
        // ȡ���̳߳��е����������߳�����
        //  ȡ���������̳߳�����
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->task->getTaskNum();
        int liveNum = pool->live_Num;
        int busyNum = pool->busy_Num;
        pthread_mutex_unlock(&pool->mutexPool);

        // �����߳�
        const int NUMBER = 2;
        // ��ǰ�������>�����߳��� && �����߳���<����̸߳���
        if (queueSize > liveNum && liveNum < pool->m_maxNum)
        {
            // �̳߳ؼ���
            pthread_mutex_lock(&pool->mutexPool);
            int num = 0;
            for (int i = 0; i < pool->m_maxNum && num < NUMBER
                && pool->live_Num < pool->m_maxNum; ++i)
            {
                if (pool->threadIds[i].x == 0)
                {
                    pthread_create(&pool->threadIds[i], NULL, worker, pool);
                    num++;
                    pool->live_Num++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }

        // ���ٶ�����߳�
        // æ�߳�*2 < �����߳���Ŀ && �����߳��� > ��С�߳�����
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
        if (threadIds[i].x == tid.x)
        {
            cout << "threadExit() function: thread " 
                << to_string(pthread_self().x) << " exiting..." << endl;

            threadIds[i].x = 0;
            break;
        }
    }
    pthread_exit(NULL);

}
