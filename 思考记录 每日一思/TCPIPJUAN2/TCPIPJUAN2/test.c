//
//  test.c
//  TCPIPJUAN2
//
//  Created by admindyn on 2018/10/16.
//  Copyright © 2018年 admindyn. All rights reserved.
//

/*
 重 点 在 目 录 netinet ， 它 包 含 所 有 TCP/IP 源 代 码 。 在 目 录 kern 和 net 中我们也可 找到一些文件。前者是协议无关的插口代码，而后者是一些通用联网函数，用于 TCP/IP 例程， 如路由代码。
 */

#include "err_debug.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"



#define BUFFSIZE 150


int test()
{
    struct sockaddr_in serv;
    
    char buff[BUFFSIZE];
    int sockfd,n;
    
    /*创建一个UDP 文件描述符 参数 第一 Internet协议族 数据udp接口协议 */
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0))<0) {
        err_sys("socket error");
    }
    
    bzero((char*)&serv, sizeof(serv));
    
    serv.sin_family = AF_INET;
    /*在结构体sockaddr_in中存放（日期/时间服务器的）IP地址*/
    serv.sin_addr.s_addr = inet_addr("140.252.13.34");
    /*在结构体sockaddr_in中存放（日期/时间服务器的）端口号 大多数TCP/IP实现都提供标准的 日期/时间服务器，它的端口号为 13*/
    serv.sin_port = htons(13);
    /*为了避免拥塞网络 这里buf的长度要小于1472字节*/
    /*sendto 函数调用中 第5个参数指向一个结构体serv服务器地址结构体描述  第6个参数指示第5个参数结构体的占用空间的长度 这个长度为16字节
     
     sendto调用的第一件事就是验证这些参数是有效的 并且将第5个参数的结构体复制到一个mbuf中*/
    if (sendto(sockfd, buff, BUFFSIZE, 0, (struct sockaddr*)&serv, sizeof(serv))!=BUFFSIZE) {
        err_sys("send to error");
    }
    /*调用recvfrom读取从服务器发回的数据包*/
    if ((n=recvfrom(sockfd, buff, BUFFSIZE, 0, (struct sockaddr*)NULL, (socklen_t*)NULL))<2) {
        err_sys("recvfrom error");
    }
    /*null terminate*/
    buff[n-2]=0;
    printf("%s\n",buff);
    
    
    return 1;
    
}
