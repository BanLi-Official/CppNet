#include "taskQueue.hpp"
#include <iostream>

using namespace std;

int main()
{
    cout<<"hello world!";
    return 0;
}

taskQ::taskQ()
{
    pthread_mutex_init(&this->m_mutex,NULL);

}

taskQ::~taskQ()
{
    pthread_mutex_destroy(&this->m_mutex);
}

void taskQ::inputTask(task a)
{
    this->taskQueue.push(a);
}

void taskQ::inputTask(callback f, void *arg)
{
    this->taskQueue.push(task(f,arg));
}

task taskQ::getTask()
{
    task res=this->taskQueue.front();//取最前面第一个任务
    this->taskQueue.pop();//弹出第一个任务
    return task();
}
