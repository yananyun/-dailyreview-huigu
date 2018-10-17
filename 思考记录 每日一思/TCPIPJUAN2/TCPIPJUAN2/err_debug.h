//
//  err_debug.h
//  TCPIPJUAN2
//
//  Created by admindyn on 2018/10/17.
//  Copyright © 2018年 admindyn. All rights reserved.
//

#ifndef err_debug_h
#define err_debug_h

#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#define MAXLINE 1024
void err_sys(const char *fmt, ...);
void err_quit(const char*,...);
int read_stream(int,char*, int);
#endif /* err_debug_h */
