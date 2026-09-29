#ifndef  WRITE_DATA_H
#define WRITE_DATA_H

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
#include <netinet/in.h>
#include <sys/time.h>

#include  "recv_data.h"

#define  WRITE_PATH "/home/wz/c/data/"

typedef  enum{
        WRITE_STATE_IDLE=0,
        WRITE_STATE_ING=2,
        WRITE_STATE_OVER=3,
        WRITE_STATE_ERR=4

}write_state_t;

typedef struct{
         size_t   len;
         size_t    total_len;
         size_t    write_len;

         write_state_t   state;

}write_context_t;

int    write_n(FILE *f,recv_context_t *rct,write_context_t *wct);

#endif
