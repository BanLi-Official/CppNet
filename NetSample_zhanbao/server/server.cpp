// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

struct PDU
{
    int PDULen;
    bool usefull;
    char buf[1024];
} Message;

int ParsePDU(struct PDU *pdu,char *buf)  //将缓冲区内容解析为PDU形式，再将pdu通过传出参数传出
{
    int PDUlenN;
    memcpy(&PDUlenN, buf, sizeof(int)); //提取pdu长度
    pdu->PDULen = ntohl(PDUlenN); // 将长度的网络字节序转为主机字节序
    int messageSize = pdu->PDULen - sizeof(bool) - sizeof(int);
    cout << "PDULen=" << pdu->PDULen << endl;
    memcpy(&pdu->usefull, buf + sizeof(int), sizeof(bool));  //提取pdu是否有效的标志
    cout << "pdu.usefull= " << pdu->usefull << endl;
    memcpy(pdu->buf, buf + sizeof(bool) + sizeof(int), messageSize);   //提取pdu信息内容
    buf[messageSize] = '\0';   //在最后加个结束符
    string str(pdu->buf);
    cout << "pdu.buf= " << str << endl;
    memset(pdu->buf, 0, sizeof(pdu->buf));   //清空
    cout<<"pdu解析成功！"<<endl<<endl<<endl;
    return 1;

}

int main()
{
    // 1. 创建监听的套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. 将socket()返回值和本地的IP端口绑定到一起
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999); // 大端端口
    // INADDR_ANY代表本机的所有IP, 假设有三个网卡就有三个IP地址
    // 这个宏可以代表任意一个IP地址
    // 这个宏一般用于本地的绑定操作
    addr.sin_addr.s_addr = INADDR_ANY; // 这个宏的值为0 == 0.0.0.0
                                       //    inet_pton(AF_INET, "192.168.8.161", &addr.sin_addr.s_addr);
    int ret = bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("bind");
        exit(0);
    }

    // 3. 设置监听
    ret = listen(lfd, 128);
    if (ret == -1)
    {
        perror("listen");
        exit(0);
    }

    // 4. 阻塞等待并接受客户端连接
    struct sockaddr_in cliaddr;
    int clilen = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr *)&cliaddr, (socklen_t *)&clilen);
    if (cfd == -1)
    {
        perror("accept");
        exit(0);
    }
    // 打印客户端的地址信息
    char ip[24] = {0};
    printf("客户端的IP地址: %s, 端口: %d\n",
           inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)),
           ntohs(cliaddr.sin_port));

    // 5. 和客户端通信
    while (1)
    {
        // 接收数据
        char buf[2048];
        memset(buf, 0, sizeof(buf));
        int len = read(cfd, buf, sizeof(buf));
        if (len > 0)
        {
            struct PDU pdu;
            ParsePDU(&pdu,buf);
        }
        else if (len == 0)
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

    return 0;
}
