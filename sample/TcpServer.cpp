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

//���룺F:\study_app\MinGW\x86_64-8.1.0-release-win32-seh-rt_v6-rev0\mingw64\bin\g++.exe -fdiagnostics-color=always -g F:\WORKS\c++\c++������\sample\TcpServer.cpp -o F:\WORKS\c++\c++������\sample\TcpServer.exe -lws2_32
//g++ -fdiagnostics-color=always -g TcpServer.cpp -o TcpServer.exe -lws2_32

using namespace std;

int main()
{
    WSAData wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    // 1. �����������׽���
    int lfd = socket(AF_INET, SOCK_STREAM, 0);//AF_INET->ipv4    SOCK_STREAM->��ʽ����Э��
    if(lfd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. ��socket()����ֵ�ͱ��ص�IP�˿ڰ󶨵�һ��
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);   // ��˶˿�
    // INADDR_ANY������������IP, ����������������������IP��ַ
    // �������Դ�������һ��IP��ַ
    // �����һ�����ڱ��صİ󶨲���
    addr.sin_addr.s_addr = INADDR_ANY;  // ������ֵΪ0 == 0.0.0.0
//    inet_pton(AF_INET, "192.168.237.131", &addr.sin_addr.s_addr);
    int ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("bind");
        exit(0);
    }

    // 3. ���ü���
    ret = listen(lfd, 128);
    if(ret == -1)
    {
        perror("listen");
        exit(0);
    }

    // 4. �����ȴ������ܿͻ�������
    struct sockaddr_in cliaddr;
    int clilen = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &clilen);  //�ɹ�ʱ��accept��������һ���µ��ļ�������cfd������Ψһ��ʶ��ͻ��˵�ͨ���׽��֣�ʧ��ʱ����-1��
    if(cfd == -1)
    {
        perror("accept");
        exit(0);
    }
    else
    {
        cout<<"connect sucess\n";
    }
    // ��ӡ�ͻ��˵ĵ�ַ��Ϣ
    char ip[24] = {0};

    //printf("�ͻ��˵�IP��ַ: %s, �˿�: %d\n",inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)),ntohs(cliaddr.sin_port));
    //printf("�ͻ��˵�IP��ַ: %s, �˿�: %d\n",WSAAddressToStringA((LPSOCKADDR)&cliaddr.sin_addr.s_addr,sizeof(cliaddr.sin_addr.s_addr), NULL, (LPSTR)ip, &(DWORD )sizeof(ip)),ntohs(cliaddr.sin_port));
    //printf("�ͻ��˵�IP��ַ: %s, �˿�: %d\n",ntohl(cliaddr.sin_addr.S_un.S_addr),ntohs(cliaddr.sin_port));
    //printf("�ͻ��˵�IP��ַ: %s, �˿�: %d\n",cliaddr.sin_addr.s_addr,ntohs(cliaddr.sin_port));
    // ��ӡ�ͻ��˵ĵ�ַ��Ϣ


    strcpy(ip, inet_ntoa(cliaddr.sin_addr));
    printf("�ͻ��˵�IP��ַ��%s, �˿ڣ�%d\n", ip, ntohs(cliaddr.sin_port));


    // 5. �Ϳͻ���ͨ��
    int number=0;
    while(1)
    {
        cout<<"number="<<number++<<"\n";
        // ��������
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        //cout<<"memset over\n";
        int len = recv(cfd, buf, sizeof(buf),0);
        //cout<<"recv over\n";
        if(len > 0)
        {
            printf("�ͻ���say: %s\n", buf);
            send(cfd, buf, len,0);
        }
        else if(len  == 0)
        {
            printf("�ͻ��˶Ͽ�������...\n");
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


