#include <queue>

#include <pthread.h>
#include <mutex>


using callback =void(*)(void*);
using namespace std;
struct task
{
    task()
    {
        function=nullptr;
        arg=nullptr;
    }

    task(callback f,void *arg)
    {
        function=f;
        this->arg=arg;
    }

    callback function;
    void * arg;
};


class taskQ //任务队列
{
public:
    //构造函数
    taskQ();

    //析构函数
    ~taskQ();

    //放入任务
    void inputTask(task a);
    void inputTask(callback f,void *arg);

    //取出任务
    task getTask();

    //获取任务数量
    inline int getTaskNum()
    {
        return taskQueue.size();
    }

private:
    pthread_mutex_t m_mutex;
    
    queue<task> taskQueue;
    
};