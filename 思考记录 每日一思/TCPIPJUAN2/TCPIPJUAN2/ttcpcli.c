//
//  ttcpcli.c
//  TCPIPJUAN2
//
//  Created by admindyn on 2018/10/17.
//  Copyright © 2018年 admindyn. All rights reserved.
//
/*
 T/TCP 协议中需要使用三个常量:
 1) <sys/socket.h> 中定义的 MSG_EOF。 如果在调用 send 、 sendto 或 sendmsg 函数时设置了该标志，那么利用该连接发送数据就结束了，实际上就是结合了 write和shutdown两个函数的功能 。
 2 ) <sys/protosw.h> 中定义了一个新的协议 请求PRU_SEND_EOF。应该加上该请求。这个请求是由sosend函数发出的。
 3) <sys/protosw.h>中定义了一个新的协议标志PR_IMPLOPCL (意指“隐式打开和关闭” ) 。这个标志有两重含义 ; ( a ) 协议允许在调用 sendto 或 sendmsg 函数时给定对等端的地址 ， 而不必在此之前调用 conncet 函数 ( 隐式打开 ) ; ( b ) 协议能够识别 MSG _EOF 标志 ( 隐式关闭 ) 。 注意 ， 只有在 面向连接的协议 ( 如 TCP ) 中才需要 ( a ) ， 因为 在无连接的协议中总是可以直接调用sendto 和 sendmsg 函数 而不需要事先调用connect 函数。
 
 */
#include "err_debug.h"
#include "cliserv.h"
#include "ttcpcli.h"

/*
 这里注意
 
 ttcp客户端 与正常tcp的不同在于
 
 将原先 客户端需要 connect syn 再发数据的 两个过程的报文段交互 合并成一个
 
 原先 需要 connnect  syn  然后 write 发送 数据
 
 这里 不调用connect  直接 sendto 函数 指明要发送的数据 指明要发送的数据目的地址 指明要发送的数据包的FIN结束标记
 
 
 
 */

int testttcp(int argc,char* argv[])
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
  
    
   /*TCP_NOPUSH 客户发送请求数据超过TCP单次报文段的最大长度时优化 这选项标记 在#include <netinet/tcp.h>*/
   // int n;
    n = 1;
    
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NOPUSH, (char*)&n, sizeof(n))<0) {
        err_sys("TCP_NOPUSH error");
    }
    /*
     SO_REUSEADDR选项
     只修改客户程序，使其重用同一个端口号，这时不但 TIME_W AIT状态的持续时间可以 像前一种情况那样截断到连接中重传超时的 8倍，而且，如果同一连接的另一个替身被
     创建，TIME_WAIT状态就会更快地终止。
     */
    if (setsockopt(sockfd, IPPROTO_TCP, SO_REUSEADDR, (char*)&n, sizeof(n))<0) {
        err_sys("TCP_NOPUSH error");
    }
    /*TIME_WAIT时间 与 报文数据在路由中的TTL值（TTL字段为1字节8比特 最大数据报路由转发次数255） 以及 TCP报文段的最大生存时间MSL（RFC 规定2分钟 即120秒 小雨255）的关系*/
    /*
     互连网上的每一个 IP 数据报都有一个有限的生存期限， 这个期限值是由 IP 首部的TTL ( 生存时间 ) 字段决定的。每一台路由器在转发 IP 数据报时都要将其 TTL 值减 1 ;但如果该 IP 数据报在路由器中等待的时间超过 1 秒，那就要把 TTL 的值减去等待的时间 。 实际上，很少有 IP 数据报在路由器中的等待时间超过 1秒的，因而每个路由器通常都是把 TTL 的值减 1 ( RFC 1812 [ Baker 1995 ] ) 的 5.3.1节 ) 。由于 TTL 字段的长度是8比特 ， 因此 每个IP数据 报所能经历的转发次数至多为255 。

     */
    /*
     从应用程序的角度而言，所谓 TIME_W AIT状态截断就意味着客户程序在与同一个服务器 进行一系列事务时，必须选择是让一系列连接使用同一个本地端口，还是让每次事务使用各 自不同的本地端口。当连接的持续时间小于报文段最大生存时间 MSL 时 ( 在事务中这是典型的情况 ) ， 重用本地端口可以节约 TCP 资源 ( 即减少了对控制 块内存的需求 ）
     如果客户程序在前一次连接的持续时间大于报文段最大生存时间 MSL 的情况下试图重用本 地端口 ， 建立连接时 将返回 EADDRINUSE错误
     */
    /*
     无论应用程序采用哪种端口使用策略 ， 如果两端的主机都支持T/TCP协议， 而且连接的持续时间小于报文段最大生存时间 MSL，那么TIME_WAIT状态的保持时间总是可 以从2倍MSL截断到8倍RTO。这样就节约了资源(即内存和CPU时间)。这对支持T/TCP协议的2台主机之间的任何TCP连接 ( 如FTP 、 SMTP 、 HTTP 以及其他等 ) ， 只要连接持续时间小于 MSL 就都适用。
     */
    /*创建一个网卡插口地址结构体 并填入服务器的IP地址 和 端口号
     
     
     
     */
    
    memset(&serv, 0, sizeof(serv));
    
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(argv[1]);
    
    serv.sin_port = htons(TCP_SERV_PORT);
 
    /*from request[] ...*/
    /*
     客户的请求用write函数发给服务器 之后使用shutdown函数关闭连接的一半。也就是数据流从客户向服务器的方向。这就告知服务器客户的数据已经发完：从客户端向服务器传递了一个文件结束的通知。这时有一个设置了FIN标志的TCP报文段发给服务器。
     */
    if (sendto(sockfd, request, REQUEST, MSG_EOF, (SA)&serv, sizeof(serv))!=REQUEST) {
        err_sys("write error");
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
