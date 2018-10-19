//
//  ttcpserv.c
//  TCPIPJUAN2
//
//  Created by admindyn on 2018/10/17.
//  Copyright © 2018年 admindyn. All rights reserved.
//

#include "err_debug.h"
#include "cliserv.h"
#include "ttcpserv.h"

int testttcpserver()
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
     当客户程序仅调用一次 sendto 函数发出请求时，通常应该设置 TCP_NOPUSH 插口选项。这样，当请求长度超过报文段最大长度 MSS时 ， 协议就会尽可能发出满长度的报文段。这样可以减少报文段的数量，减少的程度取决于每次发送的数据量
     */
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
        //
        if (send(sockfd, reply, REPLY, MSG_EOF)!=REPLY) {
            err_sys("write error");
        }
        
        close(sockfd);
    }
    
    /*T/TCP的应用场景模式
     
     使用T/TCP的最大好处 或许 在于避免了三次握手过程 ， 对于那些交换的数据量比较小的应用程序， T/TCP减少的时延将给它们带来好处。
     
     
     
     */
    
}
