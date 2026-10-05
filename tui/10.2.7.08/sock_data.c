#include "sock_data.h"

//===========recv_n()====================================================
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <arpa/inet.h>   // 需要 ntohl



/**
 * 从 sock 完整接收一个数据包（4字节长度头 + 数据体）
 * 参数：sock     - 套接字
 *       out_data - 输出：指向 malloc 分配的数据体（不含头部）
 *       out_len  - 输出：数据体长度
 * 返回值：0 成功，-1 失败
 * 注意：调用者必须 free(*out_data)
 */
// return 返回值：
// 4：客户端关闭
// -1：接收过程出错

int recv_n(int  sock,recv_context_t *rct){
    // 1. 读 4 字节长度头
    long  ls_len=0;
    uint32_t net_len = 0;
    char *ptr = (char*)&net_len;
    size_t total = 0;
    while (total < 4) {

        ssize_t rd = recv(sock, ptr + total, 4 - total, 0);

        if (rd == 0) {
                  errno = ECONNRESET;
                 return  4;                    //客户端关闭
        }
        if (rd == -1) {
            if (errno == EINTR)  continue;

            return -1;
        }
        total += rd;
    }

    uint32_t total_len = ntohl(net_len);
    if (total_len < 4 || total_len > MAX_PACKET_SIZE) {
        errno = EPROTO;
        return -1;
    }

    int data_len = total_len - 4;

    // 2. 读数据体
    char *data = malloc(data_len + 1);  // +1 可放 '\0'（仅适用于字符串）
    if (!data) {
        errno = ENOMEM;
        return -1;
    }

    total = 0;
    while (total < data_len) {
        ssize_t rd = recv(sock, data + total, data_len - total, 0);
        if (rd == 0) {
              free(data);
             errno = ECONNRESET;
             return  4; }
        if (rd == -1) {
            if (errno == EINTR) continue;
            free(data);
            return -1;
        }
        total += rd;
    }
    data[data_len] = '\0';   // 仅当数据是文本时可用

   //  printf("hs:%d\n",data_len);
        rct->buff=data;
        rct->len=data_len;
        rct->recv_len=rct->recv_len+data_len;

       return 0;
}

//=============kfsocket()===============================================================
int  kfsocket(int port,char *ip){
       int sk=socket(AF_INET,SOCK_STREAM,0);
	  if(sk==-1){
       perror("socket");
	        return -1;
	}

	struct sockaddr_in  skaddr4;
	skaddr4.sin_family=AF_INET;
	skaddr4.sin_port=htons(port);
	inet_pton(AF_INET,ip, &skaddr4.sin_addr);

//int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

     	if(connect(sk,(struct sockaddr*)&skaddr4,sizeof(skaddr4))==-1){
         //   perror("connect");
         //    puts("服务器断开或者不在线");
	         return -1;
	}
       return sk;
}


 //========================================================
