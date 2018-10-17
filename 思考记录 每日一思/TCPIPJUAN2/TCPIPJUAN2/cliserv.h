//
//  cliserv.h
//  TCPIPJUAN2
//
//  Created by admindyn on 2018/10/17.
//  Copyright © 2018年 admindyn. All rights reserved.
//

#ifndef cliserv_h
#define cliserv_h

#include <stdio.h>
/*
 Common includes and defines for UDP TCP
 and T/TCP
 
 
 clients and servers
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define REQUEST 400
#define REPLY 400

#define UDP_SERV_PORT 7777
#define TCP_SERV_PORT 8888
#define TTCP_SERV_PORT 9999

/*Following shortens all the type casts of pointer arguments*/
#define SA struct sockaddr*



#endif /* cliserv_h */
