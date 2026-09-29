#ifndef SEND_DATA_H
#define SEND_DATA_H
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

#include <stdbool.h>
#include <sys/types.h>   // ssize_t
#include <errno.h>

// 发送状态枚举
typedef enum {
    SEND_STATE_IDLE = 0,      // 初始状态
    SEND_STATE_SENDING,       // 正在发送中（有部分已发完）
    SEND_STATE_COMPLETED,     // 全部发送完毕
    SEND_STATE_ERROR          // 发送失败
} send_state_t;

// 理想的 send_n 上下文
typedef struct {
    // -------- 1. 核心“三要素”（必须保留）--------
    const char  *buf;           // 指向待发送数据的起始地址
    size_t       total_len;     // 总共需要发送的字节数（N）
    size_t       sent_len;      // 已经成功发送的字节数（关键！）

    // -------- 2. 运行状态与错误快照 --------
    send_state_t state;         // 当前发送进度状态
    int          last_errno;    // 保存 errno（防止被后续调用覆盖）
    int   fd;           // 绑定的套接字句柄

    // -------- 3. 异步回调扩展（用于事件驱动）--------
    void (*on_complete)(void *user_data);  // 发送完成时的回调函数
    void *user_data;                       // 用户自定义数据（如指向你的 connect_sock）
} send_context_t;


int send_n(int acp, char *buff, size_t len);
int  dir_data(char * out);

#endif
