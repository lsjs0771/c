#include "send_data.h"

//======send_n()==================================================================
#include <errno.h>  // 需要引入 errno

// 功能：确保把 buff 中的 len 个字节（不含头部）全部发出去
int send_n(int sock, char *buff, size_t len) {
    // 1. 打包：拼接 4 字节头部 + 数据
    uint32_t total_len_net = htonl(len + 4);
    char *data = malloc(4 + len);
    if (!data) {
        perror("malloc");
        return -1;
    }
    memcpy(data, &total_len_net, 4);
    memcpy(data + 4, buff, len);

    // 2. 核心：循环发送，直到发完为止
    size_t total = len + 4;
    size_t sent = 0;
    while (sent < total) {
        ssize_t n = send(sock, data + sent, total - sent, 0);
        if (n == -1) {
            // 如果被信号中断（EINTR），继续发送，不要报错
            if (errno == EINTR) continue;
            perror("send");
            free(data);
            return -1;
        }
        if (n == 0) {
            // 对方连接已关闭（极少发生，但防御处理）
            fprintf(stderr, "send: 连接已关闭\n");
            free(data);
            return -1;
        }
        sent += n;
    }

    free(data);
    return 0; // 全部发完，成功
}
