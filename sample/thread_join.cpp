#include <iostream>
#include <mingw.thread.h>
using namespace std;
void thread_1(int x)
{
    while(x--)
    {
        cout<<"子线程1111"<<endl;
    }
}
void thread_2(int x)
{
    while(x--)
    {
        cout<<"子线程2222"<<endl;
    }
}
int main()
{
    thread first ( thread_1,20);     // 开启线程，调用：thread_1()
    thread second (thread_2,100);  // 开启线程，调用：thread_2(100)

    first.join();                // pauses until first finishes 这个操作完了之后才能destroyed
    second.join();               // pauses until second finishes//join完了之后，才能往下执行。
    //while(1)
    {
        std::cout << "主线程\n";
    }
    return 0;
}



