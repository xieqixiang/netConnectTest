#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "wrap.h"

#define SERV_IP "127.0.0.1"
#define SERV_PORT 9527

int main(void)
{
    int sfd;
    ssize_t len;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZ];
    
    /*创建一个socket 指定IPv4 TCP*/
    sfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    /*初始化一个地址结构:*/
    bzero(&serv_addr, sizeof(serv_addr));                       //清零
    serv_addr.sin_family = AF_INET;                             //IPv4协议族
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);    //指定IP 字符串类型转换为网络字节序 参3:传出参数
    serv_addr.sin_port = htons(SERV_PORT);                      //指定端口 本地转网络字节序
    
    /*根据地址结构链接指定服务器进程*/
    Connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    while (1) {
        /*从标准输入获取数据*/
        fgets(buf, sizeof(buf), stdin);
        /*将数据写给服务器*/
        ssize_t r = Write(sfd, buf, strlen(buf));        //向服务器发数据
        printf("Write r ======== %d\n",(int)r);
        /*从服务器读回转换后数据*/
        len = Read(sfd, buf, sizeof(buf));
        printf("Read len ========= %d\n",(int)len);
        /*写至标准输出*/
        Write(STDOUT_FILENO, buf, len);
    }
    
    /*关闭链接*/
    Close(sfd);
    
    return 0;
}

