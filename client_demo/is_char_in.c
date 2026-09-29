////判断键盘输入的命令是否有效
//
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

#include "keyboad.h"
#include "write_data.h"

int is_char_in(char file_in[1024],keyboad_state_t * kst){

    char ls[1024]={'\0'};
    strncpy(ls, file_in, strlen(file_in) + 1);   // 带上 '\0'

    char *token = strtok(ls, " ");               // 在副本上切分

    if (token != NULL) {
          *kst=NONE;
        if(strcmp(token,"pull")==0)  *kst=PULL;
        if(strcmp(token,"push")==0)  *kst=PUSH;
        if(strcmp(token,"cd")==0)    *kst=CD;
        if(strcmp(token,"del")==0)    *kst=DEL;
        if(strcmp(token,"dir")==0)    *kst=DIR_K;
    }

    return  0;
}
