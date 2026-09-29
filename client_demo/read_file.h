#ifndef READ_FILE_H_INCLUDED
#define READ_FILE_H_INCLUDED

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

#define READ_N  64*1024

typedef enum {
    READ_STATE_IDLE = 0,      // 初始状态
    READ_STATE_SENDING=2,       // 正在写入文件
    READ_STATE_COMPLETED=3,     // 文件已收完
    READ_STATE_ERROR =4         // 写失败
} read_state_t;

typedef struct{

    size_t        len;                 //每次接收数据长度
    size_t       total_len;           // 文件的总字节数（N）
    size_t       read_len;
    char  *  read_buff;
    read_state_t   state;

}read_context_t;

int read_file(FILE *file,read_context_t *rdct);

#endif // READ_FILE_H_INCLUDED
