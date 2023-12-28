#include <queue>
#include <mingw.thread.h>
#include <mutex>
#include "taskqueue.cpp"

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


private:
    mutex m_mutex;
    
    queue<task> taskQueue;
    
};