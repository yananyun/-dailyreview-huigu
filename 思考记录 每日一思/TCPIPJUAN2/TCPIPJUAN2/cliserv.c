//
//  cliserv.c
//  TCPIPJUAN2
//
//  Created by admindyn on 2018/10/17.
//  Copyright © 2018年 admindyn. All rights reserved.
//

#include "cliserv.h"


int testudpserv()
{
    /*simple UDP server*/
    struct sockaddr_in serv, cli;
    char request[REQUEST], reply[REPLY];
    
    int sockfd,n,clien;
    /*
     创建一个用于UDP通讯的文件描述符
     */
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0))<0) {
        err_sys("socket error");
    }
    /*创建一个网卡插口地址结构体 并填入服务器的本机地址 这里设置为通配符INADDR_ANY
     
     这意味着服务器可以从任何一个本机接口接收数据报(假设服务器是多宿主的 即可以有多个网络接口 )
     
     端口号设为服务器的知名端口(UDP_SERV_PORT)
     
     该常量在头文件中定义
     
     最终将本机的IP地址与端口号绑定到UDP的文件描述符上
     
     */
    memset(&serv, 0, sizeof(serv));
    
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    
    serv.sin_port = htons(UDP_SERV_PORT);
    
    if (bind(sockfd, (SA)&serv, sizeof(serv))<0) {
        err_sys("bind error");
    }
    /*
     接下来 服务器程序 进入一个无限循环
     等待客户程序的请求到达(recvfrom)
     处理该请求 然后发出应答(sendto)
     */
    for (; ; ) {
        clien = sizeof(cli);
        /*由于UDP是不保证可靠的协议 数据报可能会丢失 失序 或 重复，因此实用的应用程序 需要在recvfrom这里设置一个超时定时器
         
         用以检测数据报的丢失 并重传请求
         
         开发一个可靠的UDP程序 需要 额外的超时机制 RTT值测量 请求标识
         可靠的udp需要 序列号 重传计数器 往返时间估计器
         */
        if ((n= recvfrom(sockfd, request, REQUEST, 0, (SA)&cli, &clien))<0) {
            
            err_sys("recvfrom error");
        }
        
        /*process n bytes of request[] and create reply[]*/
        
        
        if (sendto(sockfd, reply, REPLY, 0, (SA)&cli, sizeof(cli))!=REPLY) {
            
            err_sys("sendto error");
        }
        
        
    }
    
    
}





