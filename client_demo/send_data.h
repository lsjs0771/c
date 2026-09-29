#ifndef SEND_DATA_H_INCLUDED
#define SEND_DATA_H_INCLUDED


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

#define  SEND_N  64*1024

typedef enum {
    SEND_STATE_IDLE = 0,      // 初始状态
    SEND_STATE_SENDING=2,       // 正在写入文件
    SEND_STATE_COMPLETED=3,     // 文件已收完
    SEND_STATE_ERROR =4         // 写失败
} send_state_t;

typedef struct{

    size_t        len;                 //每次接收数据长度
    size_t       total_len;           // 文件的总字节数（N）
    size_t       send_len;
    send_state_t   state;

}send_context_t;



#endif // SEND_DATA_H_INCLUDED
