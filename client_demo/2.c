//客户端socket
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
#include <stdbool.h>

#include "tui.h"
#include "wr_file.h"

#include "write_data.h"
#include "keyboad.h"
#include "read_file.h"
#include "send_data.h"

#define  RECV_N  64*1024   //socket接收单位，必须与服务器的发送单位相同

#define  PULL_PATH   "/home/wz/c/pull/"    //pull 下载文件路径

int kfsocket(int port,char *ip);
int send_n(int sock,char *buff,size_t len);
int recv_n(int sock, recv_context_t * rct) ;
int is_char_in(char *file_in,keyboad_state_t *kst);                  //判断键盘输入的命令是否有效
int keyboad(char file_in[1024],int *char_in_len);

int main(){
      //画TUI框架
    printf("\033[?1049h");                 //切到备用屏
    printf ("\033[8;%d;%dt",HANG,LIE);    //设置终端行，列大小
    for(int n=1;n<HANG+1;n++){
        printf("\033[%d;%dH",n,LIE/2);
        printf("%c",'.');
    }
    printf("\033[%d;%dH",1,1);
    printf("客户端文件列表");
    printf("\033[%d;%dH",1,LIE/2+1);
    printf("服务器文件列表");

  //程序出入框光标位置
    printf("\033[%d;%dH",START_H,START_L);
    printf("pull:下载 push:上传 dir:显示服务器文件");


       //---------------------------------------------------------------------------------------

	    int port=8000;
        char *ip="192.168.101.233";
        int  sock=kfsocket(port,ip);
        if(sock==-1)  return 0;

        while(1){                              //socket长连接
                 keyboad_state_t  kst;         //键盘输入的命令enum

                 data_type_t  dtt;             // 数据包首先发送的数据类型：10字节长
                 memset(&dtt,0,sizeof(dtt));     //规定占10个字节

                 printf("\033[%d;%dH",START_H-1,START_L);
                 printf("%s","输入命令：");

                fflush(0);
        	    char  file_in[1024];            //
        	    int file_int_len=0;

                      printf("\033[%d;%dH",START_H-1,START_L+10);
                      keyboad(file_in,&file_int_len);             //可以输入空格的scanf()    输入字符长度不能用strlen,因为带有空格



                if(strcmp(file_in,"exit")==0){     //exit 退出客户端程序
                        puts("exit");
                        break;
                }

                  char direntname[256]=CLIENT_FILE_PATH;
                  char echo_buff[HANG][75];
                  int  echo_len=0;

                 if(strcmp(file_in,"dirc")==0){
                      echo_drient(direntname,echo_buff,&echo_len);
                      clear_txt(CLIENT_TEXT_H,CLIENT_TEXT_L,CLIENT_TEXT_OVER_H,CLIENT_TEXT_OVER_L);
                      for(int file_n=0;file_n<echo_len;file_n++){
                          printf("\033[%d;%dH",CLIENT_TEXT_H+file_n,1);
                          printf("%s",echo_buff[file_n]);
                      }

                      int  hang=START_H;
                      int  lie=START_L;
                      clear_in_h(hang,lie);                //清空输入行
                      continue;
                }

           //-----------------------------------------------------------------------------------------


     //--------输入指令：pull  下载文件    dir 查询目录    cd  进入目录   push 上传文件   del 删除文件-----------------------

               is_char_in(file_in,&kst);                     //带空格输入
                if(kst==NONE){                               //判断输入非法命令
                     puts("服务器没有此功能，重新输入");                                                                       //ppppppppppppppppppp
                    continue;
                }

                 dtt.type='m';                               //设置为命令模式
                 strcpy(dtt.ls,"12345678");                        //此9个字节为保留，现在不用

                 char *ls_ml=malloc(10+file_int_len);     //拼接请求类型和请求数据，m代表命令
                 memcpy(ls_ml,&dtt,10);
                 memcpy(ls_ml+10,file_in,file_int_len);

     //------------解析push 后面的文件名------------------------------------------------------------------------------------
                  char filename[1024]={0};
                 if(kst==PUSH){                //输入push 命令时
                         //块读文件  块上传
                         char *ls_filename=strtok(file_in," ");    //命令：push

                         ls_filename=strtok(NULL," ");        //文件名
                         if(ls_filename!=NULL){
                              int bz=sprintf(filename,"%s%s",PULL_PATH,ls_filename);
                              if(bz<0){
                                 perror("filename sprintf");
                                 continue;
                              }
                         }else{
                              puts("PUSH 没有输入文件");
                              continue;
                         }
                   }

             //      puts(filename);
    //---------首先向服务器发送请求--------------------,---------------------------------------------------------------------------------------

                send_n(sock,ls_ml,file_int_len+10);//向服务器发送查询  //
                free(ls_ml);
                ls_ml=NULL;
     //-------------------------------------------------------------------------------------------------------------------
                recv_context_t   rct;
                memset(&rct,0,sizeof(rct));
                write_context_t  wct;
                memset(&wct,0,sizeof(wct));
                FILE *f=NULL;

        while(1){

        //--------DEL接收----------------------------------------------------------------------------------------------
                   if(kst==DEL){
                             if(rct.state==0){
                             char  echo_dirent[2048];
                             recv_n(sock,&rct);
                            printf("%s\n",rct.buff);
                       //--------------------------------------------------

                       //--------------------------------------------------

                             break;
                        }
                   }
        //---------PUSH  发送处理----------------------------------------------------------------------------------------------
                   if(kst==PUSH){
                      FILE *file=fopen(filename,"rb");
                      if(file==NULL){
                          perror("READ file");
                          break;
                      }
                //int send_n(int sock,char *buff,size_t len);
                     char   send_buff[SEND_N]={'\0'};
                     read_context_t   rdct;
                     memset(&rdct,0,sizeof(rdct));

                     send_context_t sct;
                     memset(&sct,0,sizeof(sct));

                     while(1){

                           rdct.read_buff=send_buff;
                           read_file(file,&rdct);

                        if((sct.state==2)){                           //发文件数据包 READ_N

                               send_n(sock,rdct.read_buff,rdct.len);
                               sct.send_len=sct.send_len+rdct.len;
                    //           printf("send:%ld\n",sct.send_len);
                           }

                        if(sct.state==0){                        //首发文件总字节数

                               char out[32]={'\0'};
                               sprintf(out,"%ld",rdct.total_len);
                               send_n(sock,out,strlen(out));

                               send_n(sock,rdct.read_buff,rdct.len);
                               sct.send_len=sct.send_len+rdct.len;
                    //           printf("send:%ld\n",sct.send_len);
                               sct.state=2;
                        //       puts(out);
                           }
                           if(rdct.state==3)  break;

                           if(rdct.total_len==sct.send_len){
                                 printf("发送：%ld\n",sct.send_len);
                                 fclose(file);
                                 break;
                           }
                     }

                    break;
                  }
      //-----------DIR-接收处理------------------------------------------------------------------------------------------------------------

                   if(kst==DIR_K){
                        if(rct.state==0){
                             char  echo_dirent[2048];
                             recv_n(sock,&rct);
                   //          printf("%s\n",rct.buff);
                      //------------------------------------------------------------
                              char *xs_file=strtok(rct.buff," ");
                              int  hh=SERVER_TEXT_H;
                              while(xs_file!=NULL){
                                   printf("\033[%d;%dH",hh,SERVER_TEXT_L);
                                   printf("%s",xs_file);
                                   xs_file=strtok(NULL," ");
                                   hh++;
                              }

                      //-----------------------------------------------------------


                             break;
                        }
                  }
    //-------------PULL--接收处理---------------------------------------------------------------------------------------------------------------------
                 if(kst==PULL) {
                      if(rct.state==0){

                           recv_n(sock,&rct);                  //首先接收文件名或者“file no open"(服务器没有查询的文件）
                           char * filename=rct.buff;

                           if(memcmp(filename,"file no open",sizeof(filename))==0) {     //无文件退出循环
                                puts("服务器没有此查询文件");
                                break;
                            }

                           if(memcmp(filename,"please into filename",sizeof(filename))==0) {     //无文件退出循环
                                puts("没有输入文件名");
                                break;
                            }

                            char pull_filename[1024];
                            snprintf(pull_filename,1024,"%s%s",PULL_PATH,filename);
                            f=fopen(pull_filename,"w+b");                   // 写入文件
                            if(f==NULL){
                                perror("FILE build");
                                break;
                            }

                            recv_n(sock,&rct);                      //接收文件总字节数
                            long ls_len=strtol(rct.buff,NULL,10);   //字符串转long int

                            rct.recv_len=0;                        //开始记录接收的字节数
                            rct.state=2;                           //进入接收文件状态
                            rct.total_len=ls_len;                  // 总文件长度
                      }
                      recv_n(sock,&rct);                            //循环接收文件，以64k 为单位
                      write_n(f,&wct,&rct);                         //循环写入文件，以64k为单位

                      if((rct.total_len!=0)&&(rct.total_len==wct.recv_len)){     //当写入的总字节数等于文件总字节数退出接收状态
                            fclose(f);
                            printf("成功接收:%ld字节\n",rct.recv_len);

                 //-----------显示客户端存储目录---------------------------------------------------------------------------
                      char direntname[256]=CLIENT_FILE_PATH;
                      char echo_buff[HANG][75];
                      int  echo_len=0;

                      echo_drient(direntname,echo_buff,&echo_len);
                      clear_txt(CLIENT_TEXT_H,CLIENT_TEXT_L,CLIENT_TEXT_OVER_H,CLIENT_TEXT_OVER_L);
                      for(int file_n=0;file_n<echo_len;file_n++){
                          printf("\033[%d;%dH",CLIENT_TEXT_H+file_n,1);
                          printf("%s",echo_buff[file_n]);
                      }

                      int  hang=START_H;
                      int  lie=START_L;
                      clear_in_h(hang,lie);                //清空输入行


                 //---------------------------------------------------------------------------------------

                            break;                                     // 接收完成
                      }
                  }

               }
        }

	close(sock);
	return 0;

}

//==========main() over=====================================================

//===========recv_n()====================================================
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <arpa/inet.h>   // 需要 ntohl

#define MAX_PACKET_SIZE (1024*1024)

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
             puts("服务器断开或者不在线");
	         return -1;
	}
       return sk;
}


 //========================================================

