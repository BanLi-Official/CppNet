// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

struct PDU
{
    int PDULen;
    bool usefull;
    char buf[1024];
} Message;

int loadPDU(char *buf, PDU pdu)
{
    if (pdu.PDULen - sizeof(int) - sizeof(bool) == 0 || pdu.usefull == false)
    {
        cout << "装载错误，pdu无效" << endl;
        return -1;
    }
    int len = htonl(pdu.PDULen); // 先将长度转为网络字节序
    memcpy(buf, &len, sizeof(int));
    memcpy(buf + sizeof(int), &pdu.usefull, sizeof(bool));
    memcpy(buf + sizeof(bool) + sizeof(int), pdu.buf, pdu.PDULen - sizeof(int) - sizeof(bool));

    cout << "PDU内容装载完毕" << endl;
    return 1;
}

int main()
{
    // 1. 创建通信的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. 连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999); // 大端端口
    inet_pton(AF_INET, "192.168.8.161", &addr.sin_addr.s_addr);

    int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("connect");
        exit(0);
    }

    // 3. 和服务器端通信
    int number = 0;
    while (1)
    {
        // 发送数据
        char buf[2048];
        // 读取文本中的内容
        int getFile = open("../word.txt", O_RDONLY);
        int len;

        do
        {
            // 从文本文件中读取文本内容
            char message[1024];
            len = read(getFile, message, (rand() % 1000) + 1);
            string str(message);
            cout << "message=    " << str << endl;

            // 传入PDU中，并将其统一拷贝到缓冲区当中，然后写一个传输PDU的功能函数，使接收端得到一个PDU;
            struct PDU pdu;
            pdu.PDULen = len + 4 + 1;
            pdu.usefull = true;
            memcpy(pdu.buf, message, len);
            int res = loadPDU(buf, pdu); // 将这个pdu装载进入buf缓冲区

            if (res != -1)
            {
                write(fd, buf, pdu.PDULen); // 将缓冲区内容发送出去
                cout << "发送完毕！" << endl<< endl;
            }

            // 清空缓冲区
            memset(buf, 0, sizeof(buf));
            memset(message, 0, sizeof(message));
            sleep(1);
        } while (len != 0);

        cout << endl
             << endl
             << endl;

        sleep(10); // 每隔1s发送一条数据
    }

    close(fd);

    return 0;
}
