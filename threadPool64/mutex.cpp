#include "mutex.hpp"
#include <iostream>

using namespace std;

MyMutex mutexs=new MyMutex(); 

MyMutex::MyMutex()
{
    if(pthread_mutex_init(&this->IOMutex,NULL)!=0)
    {
        cout<<"mutex����ʼ��ʧ�ܣ�"<<endl;
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
