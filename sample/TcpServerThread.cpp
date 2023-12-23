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

#define _WIN32_WINNT 0x0600


#pragma comment(lib, "ws2_32.lib")

//编译：F:\study_app\MinGW\x86_64-8.1.0-release-win32-seh-rt_v6-rev0\mingw64\bin\g++.exe -fdiagnostics-color=always -g F:\WORKS\c++\c++网络编程\sample\TcpServer.cpp -o F:\WORKS\c++\c++网络编程\sample\TcpServer.exe -lws2_32
//g++ -fdiagnostics-color=always -g TcpServer.cpp -o TcpServer.exe -lws2_32

using namespace std;


struct SockInfo
{
    int cfd;  //通信socket标识
    pthread_t tid;//线程的id
    struct sockaddr_in addr;  //地址信息

};

struct SockInfo infos[128];  //最大可连接128个客户端




void* working(void* arg)
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
            printf("客户端say: %s\n", buf);
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

    return NULL;
}






int main()
{
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

    // 4. 阻塞等待并接受客户端连接

    int max_client=sizeof(infos)/sizeof(infos[0]);
    for(int i=0;i<max_client;i++)
    {
        //bzero(&infos[i],sizeof(infos[i]));
        //char myBuffer[100];
        memset(&infos[i], 0, sizeof(infos[i]));// 将myBuffer中的内容全部置为0
        infos[i].cfd=-1;
        infos[i].tid=-1;
    }

    while (1)
    {
        //创建子进程
        struct SockInfo *info;
        for(int i=0;i<max_client;i++)
        {
            if(infos[i].tid==-1)
            {
                info=&infos[i];
                break;
            }
            if (i==max_client-1)
            {
                sleep(1);
                i--;
            }
            
        }

        int clilen = sizeof(info->addr);
        info->cfd = accept(lfd, (struct sockaddr*)&info->addr, &clilen);  //成功时，accept函数返回一个新的文件描述符cfd，用于唯一标识与客户端的通信套接字；失败时返回-1。
        if(info->cfd == -1)
        {
            perror("accept");
            exit(0);
        }
        else
        {
            cout<<"connect sucess\n";
            
            pthread_create(&info->tid,NULL,working,info);
            pthread_detach(info->tid);
        }

    }
    


    
    close(lfd);
    return 0;
    
}

