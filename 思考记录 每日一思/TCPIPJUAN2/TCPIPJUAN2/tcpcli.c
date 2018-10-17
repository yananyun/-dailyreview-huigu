//
//  cliserver.c
//  TCPIPJUAN2
//
//  Created by admindyn on 2018/10/17.
//  Copyright © 2018年 admindyn. All rights reserved.
//
#include "err_debug.h"
#include "cliserv.h"
#include "tcpcli.h"


int testtcp(int argc,char* argv[])
{
    /*simple TCP client*/
    struct sockaddr_in serv;
    
    char request[REQUEST],reply[REPLY];
    
    int sockfd,n;
    
    if (argc!=2) {
        err_quit("usage:tcpcli<IP address of server>");
    }
    /*
     创建TCP 文件描述符
     
     */
    if ((sockfd=socket(PF_INET, SOCK_STREAM, 0))<0) {
        err_sys("socket error");
    }
    
    /*创建一个网卡插口地址结构体 并填入服务器的IP地址 和 端口号
     
    
     
     */
    
    memset(&serv, 0, sizeof(serv));
    
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(argv[1]);
    
    serv.sin_port = htons(TCP_SERV_PORT);
    /*
     对connect函数的调用启动TCP的三次握手过程
     在客户和服务器之间建立连接
     */
    if (connect(sockfd, (SA)&serv, sizeof(serv))<0) {
        err_sys("connect error");
    }
    /*from request[] ...*/
    /*
     客户的请求用write函数发给服务器 之后使用shutdown函数关闭连接的一半。也就是数据流从客户向服务器的方向。这就告知服务器客户的数据已经发完：从客户端向服务器传递了一个文件结束的通知。这时有一个设置了FIN标志的TCP报文段发给服务器。
     */
    if (write(sockfd, request, REQUEST)!=REQUEST) {
        err_sys("write error");
    }
    /*
     shutdown函数关闭连接的一半。也就是数据流从客户向服务器的方向。这就告知服务器客户的数据已经发完：从客户端向服务器传递了一个文件结束的通知。这时有一个设置了FIN标志的TCP报文段发给服务器。
     */
    if (shutdown(sockfd, 1)<0) {
        err_sys("shutdown error");
    }
    /*
     客户此时仍然可以从连接中读取数据 因为现在是TCP的半关闭
     */
    if ((n=read_stream(sockfd, reply, REPLY))<0) {
        err_sys("read error");
    }
    /*这里使用shutdown 半关闭 也是因为在TCP这个面相字节流的协议中 没有任何形式的记录定界符，因而从服务器端TCP传回的应答可能会包含在多个TCP报文段中。因此，就会存在需要多次调用read函数才能传递给客户进程。*/
    /*
     而服务器端会发送完应答后就会关闭连接 使得TCP向客户端发送一个带FIN的报文段，在read函数中返回一个文件结束标志（返回值为0）.
     为了处理这些细节问题 在read_stream函数中不断调用read函数知道接收缓存满或者read函数返回一个文件结束标志
     */
    /*
     在类似 TCP 这样的流协议中用来给记录定界。许多
     Internet应用程序 ( FTP 、SMTP 、HTTP 和 NNTP ) 使用回车 和 换行符 来标记记录的结束 。 其他一些应用程序 ( DNS， RPC ) 则在每个记录的前面加上一个定长的记录长度字段。 在我们的测试应用场景中，利用了 TCP 的文件结束标志 ( FIN ) ， 因为在每次事务中客户只 向服务器发送一个请求，而服务器也只发回一个应答。
     */
    
    return 1;
    
    
}
