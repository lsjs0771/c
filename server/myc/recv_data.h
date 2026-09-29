#ifndef  RECV_DATA_H
#define  RECV_DATA_H

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

#define  RECV_N    64*1024+10

#define SERVER_PATH  "/home/wz/c/data/"

typedef  struct{     //数据包类型
          char   type;
         char    ls[9];
}data_type_t;

typedef  enum {
         RECV_STATE_INIT=0,
         RECV_STATE_SEND_TOTAL_LEN=2,
         RECV_STATE_ING=3,
         RECV_STATE_OVER=4

}recv_state_t;


typedef struct {

    data_type_t   dtt;
    recv_state_t    recv_state;

    // ---------- 固定配置（只读） ----------
    size_t     total_len;        // 要接收单个数据包总字节数
    char       *buffer;
    size_t      recv_len_max;  // 实际分配的最大内存（防止越界）

    // ---------- 动态状态（每次更新） ----------
    size_t      recved_len;   // 已接收文件的长度，多个数据包的累加，不含命令数据包长度
    size_t       recv_total_len;      //总文件字节数
    int             recv_retval;      // 保存 recv() 原始返回值（用于判断 -1 错误或 0 断开）

    // ---------- 标志位 ----------
    char *  old_data;
    bool        is_over;          // 是否文件传输完成（true/false）
    bool       is_error;         // 是否发生网络异常

} recv_context_t;


//-------------------------------------------------------------------------------------------------------------------------------------
//int recv_n(int acp, char **out_data, int *out_len);
int  read_file_len(char *  char_in);
int  recv_n(int  acp,recv_context_t *rct);
#endif
