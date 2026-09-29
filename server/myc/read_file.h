#ifndef READ_FILE_H_INCLUDED
#define READ_FILE_H_INCLUDED

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

#define  READ_N    64*1024+10

typedef enum {
    READ_STATE_IDLE = 0,      // 初始状态
    READ_STATE_SENDING=2,       // 正在读取中（有部分已发完）
    READ_STATE_COMPLETED=3,     // 文件已读完
    READ_STATE_ERROR =4         // 读取失败
} read_state_t;

// 理想的 send_n 上下文
typedef struct {
    // -------- 1. 核心“三要素”（必须保留）--------
     char   buff[READ_N];           // 读取数据
    size_t        len;                 //每次读取数据长度
    size_t       total_len;     // 文件的总字节数（N）
    size_t       read_len;      // 已经成功读取的字节数（关键！）

    // -------- 2. 运行状态与错误快照 --------
    read_state_t  state;         // 当前读取状态
    int          last_errno;    // 保存 errno（防止被后续调用覆盖）
    int   fd;           // 绑定的套接字句柄

    // -------- 3. 异步回调扩展（用于事件驱动）--------
    void (*on_complete)(void *user_data);  // 发送完成时的回调函数
    void *user_data;                       // 用户自定义数据（如指向你的 connect_sock）
} read_context_t;

int  read_file(FILE *f,read_context_t  *rdct);

#endif // READ_FILE_H_INCLUDED
