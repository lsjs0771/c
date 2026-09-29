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
#include <dirent.h>

#include "recv_data.h"


int  dir_data(char *  out){
         DIR  *dr=opendir(SERVER_PATH);
         if (dr==NULL){
                 perror("DIR no open");
                 return -1;
         }
          struct dirent *   drt=NULL;

          int   p=0;
          while((drt=readdir(dr))!=NULL){
                       if(strcmp(drt->d_name,".")==0)   continue;
                       if(strcmp(drt->d_name,"..")==0)  continue;
                      int  n =sprintf(out+p,"%s    ",drt->d_name);
                       p=p+n;
          }
          closedir(dr);

        return 0;
}
