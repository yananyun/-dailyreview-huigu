//
//  udpcli.c
//  TCPIPJUAN2
//
//  Created by admindyn on 2018/10/17.
//  Copyright © 2018年 admindyn. All rights reserved.
//

#include "err_debug.h"
#include "cliserv.h"
#include "udpcli.h"

int testudpcli(int argc,char* argv[])
{
    /*simple UDP client*/
    struct sockaddr_in serv;
    
    char request[REQUEST],reply[REPLY];
    
    int sockfd,n;
    
    if (argc!=2) {
        err_quit("usage:udpcli <IP address of server>");
    }
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0))<0) {
        err_sys("socket error");
    }
    
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(argv[1]);
    serv.sin_port = htons(UDP_SERV_PORT)
    ;
    
    /*form request[] ...*/
    
    if (sendto(sockfd, request, REQUEST, 0, (SA)NULL, (int*)NULL)!=REQUEST) {
        err_sys("sendto error");
    }
    if ((n= recvfrom(sockfd, reply, REPLY, 0, (SA)NULL, (int*)NULL))<0) {
        err_sys("recvfrom error");
    }
    
    /*process n bytes of reply[] ...*/
    
    return 1;
}
