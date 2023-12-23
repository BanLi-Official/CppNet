// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <winsock2.h>
#include <iostream>

using namespace std;

//编译
//F:\study_app\MinGW\x86_64-8.1.0-release-win32-seh-rt_v6-rev0\mingw64\bin\g++.exe -fdiagnostics-color=always -g F:\WORKS\c++\c++网络编程\sample\TcpClient.cpp -o F:\WORKS\c++\c++网络编程\sample\TcpClient.exe -lws2_32
//g++ -fdiagnostics-color=always -g TcpClient.cpp -o TcpClient.exe -lws2_32

int main()
{
    WSAData wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    // 1. 创建通信的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. 连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);   // 大端端口
    //inet_pton(AF_INET, "192.168.237.131", &addr.sin_addr.s_addr);
    addr.sin_addr.s_addr=inet_addr ("127.0.0.1");

    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("connect");
        exit(0);
    }
    else
    {
        cout<<"connect Sucess"<<"\n";
    }

    // 3. 和服务器端通信
    int number = 0;
    while(1)
    {
        cout<<"number="<<number<<"      ";
        // 发送数据
        char buf[1024];
        sprintf(buf, "hello , server...%d\n", number++);
        send(fd, buf, strlen(buf)+1,0);
        cout<<"write over\n";
        // 接收数据
        memset(buf, 0, sizeof(buf));
        cout<<"get over\n";
        int len = recv(fd, buf, sizeof(buf),0);
        cout<<"recv over\n";
        if(len > 0)
        {
            printf("server say: %s\n", buf);
        }
        else if(len  == 0)
        {
            printf("server connect out...\n");
            break;
        }
        else
        {
            perror("read");
            break;
        }
        sleep(1);   // 每隔1s发送一条数据
    }

    close(fd);
    WSACleanup ();
    return 0;
}
