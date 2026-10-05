#ifndef SOCK_DATA_H_INCLUDED
#define SOCK_DATA_H_INCLUDED

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>  // 通常需要，定义 off_t 等类型
#include <unistd.h>     // 系统调用函数声明
#include <stdbool.h>

#include "write_data.h"
#include "tui.h"

#define MAX_PACKET_SIZE (1024*1024)
#define  RECV_N  64*1024

int recv_n(int  sock,recv_context_t *rct);
int  kfsocket(int port,char *ip);
int echo_server_dirent(int sock,node *nd4,node *nd3);

#endif // SOCK_DATA_H_INCLUDED
