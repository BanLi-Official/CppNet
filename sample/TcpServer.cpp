// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#define _WIN32_WINNT 0x0600


#pragma comment(lib, "ws2_32.lib")

//编译：F:\study_app\MinGW\x86_64-8.1.0-release-win32-seh-rt_v6-rev0\mingw64\bin\g++.exe -fdiagnostics-color=always -g F:\WORKS\c++\c++网络编程\sample\TcpServer.cpp -o F:\WORKS\c++\c++网络编程\sample\TcpServer.exe -lws2_32
//g++ -fdiagnostics-color=always -g TcpServer.cpp -o TcpServer.exe -lws2_32

using namespace std;

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
    struct sockaddr_in cliaddr;
    int clilen = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &clilen);  //成功时，accept函数返回一个新的文件描述符cfd，用于唯一标识与客户端的通信套接字；失败时返回-1。
    if(cfd == -1)
    {
        perror("accept");
        exit(0);
    }
    else
    {
        cout<<"connect sucess\n";
    }
    // 打印客户端的地址信息
    char ip[24] = {0};

    //printf("客户端的IP地址: %s, 端口: %d\n",inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)),ntohs(cliaddr.sin_port));
    //printf("客户端的IP地址: %s, 端口: %d\n",WSAAddressToStringA((LPSOCKADDR)&cliaddr.sin_addr.s_addr,sizeof(cliaddr.sin_addr.s_addr), NULL, (LPSTR)ip, &(DWORD )sizeof(ip)),ntohs(cliaddr.sin_port));
    //printf("客户端的IP地址: %s, 端口: %d\n",ntohl(cliaddr.sin_addr.S_un.S_addr),ntohs(cliaddr.sin_port));
    //printf("客户端的IP地址: %s, 端口: %d\n",cliaddr.sin_addr.s_addr,ntohs(cliaddr.sin_port));
    // 打印客户端的地址信息


    strcpy(ip, inet_ntoa(cliaddr.sin_addr));
    printf("客户端的IP地址：%s, 端口：%d\n", ip, ntohs(cliaddr.sin_port));


    // 5. 和客户端通信
    int number=0;
    while(1)
    {
        cout<<"number="<<number++<<"\n";
        // 接收数据
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        cout<<"memset over\n";
        int len = recv(cfd, buf, sizeof(buf),0);
        cout<<"recv over\n";
        if(len > 0)
        {
            printf("客户端say: %s\n", buf);
            send(cfd, buf, len,0);
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

    close(cfd);
    close(lfd);
    WSACleanup ();
    return 0;
}


