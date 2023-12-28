#include "taskQueue.hpp"
#include <iostream>

using namespace std;



taskQ::taskQ()
{
    pthread_mutex_init(&this->m_mutex,NULL);

}

taskQ::~taskQ()
{
    pthread_mutex_destroy(&this->m_mutex);
}

void taskQ::inputTask(Task a)
{
    pthread_mutex_lock(&m_mutex);
    this->taskQueue.push(a);
    pthread_mutex_unlock(&m_mutex);
}

void taskQ::inputTask(callback f, void *arg)
{
    pthread_mutex_lock(&m_mutex);
    this->taskQueue.push(Task(f,arg));
    pthread_mutex_unlock(&m_mutex);
}

Task taskQ::getTask()
{
    pthread_mutex_lock(&m_mutex);
    Task res=this->taskQueue.front();//取最前面第一个任务
    this->taskQueue.pop();//弹出第一个任务
    pthread_mutex_unlock(&m_mutex);
    return res;
}
