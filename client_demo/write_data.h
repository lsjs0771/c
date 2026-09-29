#ifndef WRITE_DATA_H_INCLUDED
#define WRITE_DATA_H_INCLUDED

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

typedef struct{
    char  type;     //1:命令   2：数据   占第一个字节，其余为为空
    char ls[9];

}data_type_t;       //规定占10个字节

typedef enum {
    RECV_STATE_IDLE = 0,      // 初始状态
    RECV_STATE_SENDING=2,       // 正在接收中
    RECV_STATE_COMPLETED=3,     // 文件已收完
    RECV_STATE_ERROR =4         // 读取失败
} recv_state_t;

// 理想的 send_n 上下文
typedef struct {
    // -------- 1. 核心“三要素”（必须保留）--------
     char   *buff;           // 接收数据存储
    size_t        len;                 //每次接收数据长度
    size_t       total_len;     // 文件的总字节数（N）
    size_t       recv_len;      // 已经成功接收的字节数（关键！）

    // -------- 2. 运行状态与错误快照 --------
    recv_state_t  state;         // 当前接收状态
    int          last_errno;    // 保存 errno（防止被后续调用覆盖）
    int   fd;           // 绑定的套接字句柄

    // -------- 3. 异步回调扩展（用于事件驱动）--------
    void (*on_complete)(void *user_data);  // 发送完成时的回调函数
    void *user_data;                       // 用户自定义数据（如指向你的 connect_sock）
} recv_context_t;

typedef enum {
    WRITE_STATE_IDLE = 0,      // 初始状态
    WRITE_STATE_SENDING=2,       // 正在写入文件
    WRITE_STATE_COMPLETED=3,     // 文件已收完
    WRITE_STATE_ERROR =4         // 写失败
} write_state_t;

typedef struct{

    size_t        len;                 //每次接收数据长度
    size_t       total_len;           // 文件的总字节数（N）
    size_t       recv_len;

    write_state_t   state;

}write_context_t;

int  write_n(FILE *f,write_context_t *wct,recv_context_t *rct);
#endif // WRITE_DATA_H_INCLUDED
