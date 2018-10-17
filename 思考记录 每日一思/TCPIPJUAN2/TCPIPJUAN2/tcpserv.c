//
//  tcpserv.c
//  TCPIPJUAN2
//
//  Created by admindyn on 2018/10/17.
//  Copyright © 2018年 admindyn. All rights reserved.
//

#include "err_debug.h"
#include "cliserv.h"
#include "tcpserv.h"

int testtcpserver()
{
    /*simple TCP server*/
    
    struct sockaddr_in serv,cli;
    char request[REQUEST],reply[REPLY];
    
    int listenfd,sockfd,n,clilen;
    /*
     创建TCP 套接字的 文件描述符
     */
    if ((listenfd = socket(PF_INET, SOCK_STREAM, 0))<0) {
        err_sys("socket error");
    }
    /*
    用服务器IP地址与端口号 填充网卡地址接口结构体
     */
    memset(&serv, 0, sizeof(serv));
    
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(TCP_SERV_PORT);
    /*和UDP一样 填充使用通配符做IP地址 并且绑定端口号 都需要bind函数*/
    if (bind(listenfd, (SA)&serv, sizeof(serv))<0) {
        err_sys("bind error");
    }
    /*TCP 比UDP的不同 在于这里调用listen函数 将刚创建的TCP套接字文件描述符 等待客户端方发起连接 第二个参数为允许的最大挂起连接数 内核为该这些连接做排队处理*/
    if (listen(listenfd, SOMAXCONN)) {
        err_sys("listen error");
    }
    
    for (; ; ) {
        clilen = sizeof(cli);
        /*服务器调用accept函数后就进入阻塞状态 知道有客户进程调用connect函数而建立起一个连接 函数accept返回一个新的文件描述符 这是真正的TCP客户与服务器的通信连接*/
        if ((sockfd = accept(listenfd, (SA)&cli, &clilen))<0) {
            err_sys("accept error");
        }
        /*
         服务器调用 read_stream读取客户请求 再调用write函数 向客户发送应答
         */
        
        if ((n=read_stream(sockfd, request, REQUEST))<0) {
            err_sys("read error");
        }
        /*process n bytes of request[] and create reply[] ..*/
        
        if (write(sockfd, reply, REPLY)!=REPLY) {
            err_sys("write error");
        }
        
        close(sockfd);
    }
    /*
     注意  以上是一个反复循环的服务器 ：它把当前客户请求处理完毕后才又调用accept去接收另一个客户的连接。
     
     那么如何并发呢
     
     并发服务器可以并行地处理多个客户请求
     
     在Unix主机上实现并发服务器的常用技术：
     在accept函数返回后 调用Unix的fork函数创建一个紫禁城，由子进程处理客户的请求，父进程则紧接着又调用accept去接收别的客户连接。
     */
    /*
     还有一种方式 是提前准备
     即 服务器启动时连续调用fork函数数次 并让每个子进程都在同一个监听插口描述符上调用accept函数
     
     */
    
    /*
     但是以上 开销还是大 开辟子进程 我们选择开辟子线程
     
     在第一种方式中 为每一个新建立的连接创建一个线程。
     
     */
    
    /*
     注意 TCP 我们这个场景 是 客户端 发送完之后 首先发出FIN关闭连接 。根据TCP要求 首先发出FIN的一端 在双方完全关闭连接后 首先发出的一方要保持TIME_WAIT状态
     
     当连接处于TIME_WAIT状态时，同一连接（即客户IP地址和端口号 以及服务器IP地址和端口号这4个值相同  ）不能重复打开
     
     
     如果是 服务器TIME_WAIT 会占用大量内存
     */
}


