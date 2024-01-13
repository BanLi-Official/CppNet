#include <pthread.h>



class MyMutex
{
public:
    pthread_mutex_t IOMutex;
    MyMutex();
    ~MyMutex();

    void lock(pthread_mutex_t &a);
    void unlock(pthread_mutex_t &a);



};



