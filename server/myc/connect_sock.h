#ifndef  CONNECT_SOCK_H
#define CONNECT_SOCK_H


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

#define SIZE  64*1024


typedef enum {
    SOCK_STATE_UNINIT = 0,   // 未初始化
    SOCK_STATE_CONNECTED,    // 已连接
    SOCK_STATE_CLOSED,       // 已关闭（主动或被动）
    SOCK_STATE_ERROR         // 发生致命错误
} sock_state_t;

typedef struct  {
	   int  sockname;    // 套接字描述符（兼容 Linux/Windows）

	    // ---------- 2. 对端身份（支持 IPv4/IPv6） ----------
 //   struct sockaddr_storage             peer_addr; // 对端 IP 和端口
        socklen_t                 client_addr_len;           // 地址实际长度    ipv4 ipv6
        in_port_t               sin_port;                                //port
                  char               sin_addr[16];                          //ipv4  ip

    // ---------- 3. 运行状态 ----------
    sock_state_t                    state;                // 当前连接状态（比 int isover 语义强）
                    bool                    is_nonblocking;              // 是否设置为非阻塞模式

    // ---------- 4. 超时配置（收发保护） ----------
    struct timeval               send_timeout;      // 发送超时（默认 3s）
    struct timeval               recv_timeout;      // 接收超时（默认 3s）

    // ---------- 5. 错误快照（防止 errno 丢失） ----------
    int                 last_errno;                   // 最近一次系统调用错误码
    char             last_error_msg[64];          // 可选的错误描述（方便打印日志）

    // ---------- 6. 用户私有数据指针（扩展用） ----------
    void       *user_data;                  // 可以挂载你之前写的 recv_type 结构体

}connect_sock_t;

//-----------------------------------------------------------------------------------------------------------------------------
int fwqsocket(int port,char *ip,connect_sock_t *cns);

#endif

