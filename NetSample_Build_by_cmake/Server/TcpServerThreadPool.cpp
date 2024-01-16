// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <pthread.h>
#include <strings.h>
#include "ThreadPool.hpp"

//#define _WIN32_WINNT 0x0600


pthread_mutex_t IOMutex;


#pragma comment(lib, "ws2_32.lib")

//编译：F:\study_app\MinGW\x86_64-8.1.0-release-win32-seh-rt_v6-rev0\mingw64\bin\g++.exe -fdiagnostics-color=always -g F:\WORKS\c++\c++网络编程\sample\TcpServer.cpp -o F:\WORKS\c++\c++网络编程\sample\TcpServer.exe -lws2_32
//g++ -fdiagnostics-color=always -g TcpServer.cpp -o TcpServer.exe -lws2_32

using namespace std;


struct SockInfo
{
    int cfd;  //通信socket标识
    //pthread_t tid;//线程的id
    struct sockaddr_in addr;  //地址信息

};

typedef struct AcceptConnInfo
{
    ThreadPool pool;//线程池实例
    int lfd;//监听套接字标识
}AcceptConnInfo;

struct SockInfo infos[128];  //最大可连接128个客户端




void working(void* arg)
{
    struct SockInfo* myinfo=(struct SockInfo *)arg;
    // 打印客户端的地址信息
    char ip[24] = {0};

    strcpy(ip, inet_ntoa(myinfo->addr.sin_addr));
    printf("客户端的IP地址：%s, 端口：%d\n", ip, ntohs((myinfo->addr).sin_port));


    // 5. 和客户端通信
    int number=0;
    while(1)
    {
        cout<<"number="<<number++<<"\n";
        // 接收数据
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        cout<<"memset over\n";
        int len = recv(myinfo->cfd, buf, sizeof(buf),0);
        cout<<"recv over\n";
        if(len > 0)
        {
            printf("Client say: %s\n", buf);
            send(myinfo->cfd, buf, len,0);
        }
        else if(len  == 0)
        {
            printf("客户端断开了连接...\n");
            break;
        }
        else
        {
            perror("read");
            break;
        }
    }

    return ;
}



void AcceptConn(void *arg)
{
    AcceptConnInfo *ACInfo=(AcceptConnInfo *)arg;
    // 4. 阻塞等待并接受客户端连接
    cout<<"aaaaaaaaaaaaaaa";
    while (1)
    {
        //创建子进程
        struct SockInfo *info=(struct SockInfo *)malloc(sizeof(struct SockInfo));

        int clilen = sizeof(info->addr);


        info->cfd = accept(ACInfo->lfd, (struct sockaddr*)&info->addr, &clilen);  //成功时，accept函数返回一个新的文件描述符cfd，用于唯一标识与客户端的通信套接字；失败时返回-1。
        if(info->cfd == -1)
        {
            perror("accept");
            exit(0);
        }
        else
        {
            cout<<"connect sucess info->cfd="<<info->cfd<<endl;
            Task task;
            task.function=working;
            task.arg=info;

            ACInfo->pool.addTask(task);
            
        }

    }
    
    return ; 
    
}





int main()
{
    pthread_mutex_init(&IOMutex,NULL);
    WSAData wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    // 1. 创建监听的套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. 将socket()返回值和本地的IP端口绑定到一起
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);   // 大端端口
    // INADDR_ANY代表本机的所有IP, 假设有三个网卡就有三个IP地址
    // 这个宏可以代表任意一个IP地址
    // 这个宏一般用于本地的绑定操作
    addr.sin_addr.s_addr = INADDR_ANY;  // 这个宏的值为0 == 0.0.0.0
//    inet_pton(AF_INET, "192.168.237.131", &addr.sin_addr.s_addr);
    int ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("bind");
        exit(0);
    }

    // 3. 设置监听
    ret = listen(lfd, 128);
    if(ret == -1)
    {
        perror("listen");
        exit(0);
    }

    ThreadPool pool(3,8);
    Task task;
    AcceptConnInfo *ACInfo=(AcceptConnInfo *)malloc(sizeof(AcceptConnInfo));
    ACInfo->pool=pool;
    ACInfo->lfd=lfd;
    task.function=AcceptConn;
    task.arg=ACInfo;
    
    pool.addTask(task);

    while(1){}
    pthread_exit(0);


    //是否需要sleep
    sleep(300);

    return 0;
}
