#include "mutex.hpp"
#include <iostream>

using namespace std;

MyMutex mutexs=new MyMutex(); 

MyMutex::MyMutex()
{
    if(pthread_mutex_init(&this->IOMutex,NULL)!=0)
    {
        cout<<"mutexËø³õÊ¼»¯Ê§°Ü£¡"<<endl;
    }
}

void MyMutex::lock(&pthread_mutex_t a)
{
    pthread_mutex_lock(&a);
}

void MyMutex::unlock(&pthread_mutex_t a)
{
    pthread_mutex_unlock(&a);
}
