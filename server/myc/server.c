//用服务器socket
#include "connect_sock.h"
#include "recv_data.h"
#include "send_data.h"
#include "write_data.h"
#include "read_file.h"





typedef  enum{
            PULL,
            PUSH,
            DIR,
            DEL,
            CD,
            NONE

}keyboad_state_t;

//==========main() start=========================================
int main(void){

         int  port=8000;
         char *ip="192.168.101.144";

	     connect_sock_t  cns;
         fwqsocket(port,ip,&cns);

         cns.is_nonblocking=false;
         cns.state=SOCK_STATE_CONNECTED;

        struct sockaddr      kfaddr;                                 //客户端发回的struct
        size_t kflen=sizeof(kfaddr);                         //发回的struct 长度

        puts("server waiting...");

        data_type_t    dtt;
        memset(&dtt,0,sizeof(dtt));

        while(1){

                int acp=accept(cns.sockname,&kfaddr,(socklen_t *)&kflen);   //acp 发送接收文件描述符
                if(acp==-1){
                      perror("accept");
                      return -1;
                }

           //--fork()处理收发-----------------------------------------------------------------------

                pid_t  pd=fork();
                if(pd<0){
                            perror("fork");
                            return  0;
               }

              if(pd==0){                   //子进程处理收发

                close(cns.sockname);    //关掉子进程的sock

	            struct sockaddr_in * kfsk=(struct sockaddr_in*)&kfaddr;        //客户端ip port
	            cns.sin_port=ntohs(kfsk->sin_port);
                inet_ntop(AF_INET,&(kfsk->sin_addr),cns.sin_addr,16);     //网路序转小端序
                printf("client:%s:%d\n",cns.sin_addr,cns.sin_port);           //客户端ip:port

                printf("子进程：%d\n",getpid());

               while (1) {           //一个长连接内的循环发送接收

                      recv_context_t   rct;                      //定义recv上下文
                       memset(&rct,0,sizeof(rct));

                      read_context_t   rdct;                      //定义读本地文件上下文
                      memset(&rdct,0,sizeof(rdct));
                      send_context_t    sct;                         //定义发送上下文
                      memset(&sct,0,sizeof(sct));
                      long   send_len=0;

                      int ret= recv_n(acp,&rct);        //第一次接收：命令数据   第二次开始是存数据
                     char *buff = rct.buffer;              //接收数据指针
                     if (ret == -1) {
                                 if (errno == ECONNRESET){
                                        printf("接收过程错误\n");
                                        break;
                                 }
                     }
                     if(ret==4){                          //客户端关闭，在recvd_data.c定义返回值
              //            puts("客户端关闭");

                          break;
                     }
//-----判断接收类型  PULL  PUSH  DIR  CD  DEL  NENO------------------------------------

                   keyboad_state_t     kst;

                  char ls[1024]={0};
                     memcpy(ls,buff,rct.total_len);
                     char *ml=NULL;
                    if(rct.dtt.type=='m'){                 //判断为命令
                                      ml=strtok(ls," ");           //PULL  PUSH  DIR  CD  DEL
                                     char  *p=strtok(NULL," ");       //文件名
                                     if(p!=NULL){                                        //没有文件名
                                           strcpy(buff,p);
                                      }else{
                                            if( strcmp(ml,"pull")==0) {                           //如果pull 没有输入文件名
                                                      puts("没有输入文件名");
                                                     memset(&rct,0,sizeof(rct));
                                                     char  ls_file1[]="please into filename";              //向客户端发送“please into filename”，让客户端返回循环接收状态
                                                     size_t    ls_file_len1=strlen(ls_file1);
                                                    send_n(acp,ls_file1,ls_file_len1);
                                                    continue;
                                             }
                                      }

                    }
            if(strcmp(ml,"cd")==0){}

//--------del-------------------------------------------------------------------------------------------
            if(strcmp(ml,"del")==0){

                             char  filename[128]={0};
                             sprintf(filename,"%s%s",WRITE_PATH,buff);
            //                 puts(filename);
                              int   del_bz=remove(filename);
                              if(del_bz==0){

                                       char    buff_dir[]="Successfully: del  file";
                                       send_n(acp,buff_dir,strlen(buff_dir));
                             }else{

                                      char    buff_dir1[]="fail: del file";
                                       send_n(acp,buff_dir1,strlen(buff_dir1));

                                       puts("文件删除错误");

                             }

            }
     //------PUSH 接收---------------------------------------------------------------------------------------
                if(strcmp(ml,"push")==0){
                            char  filename[128]={0};
                            sprintf(filename,"%s%s",WRITE_PATH,buff);
                             FILE *  write_f=fopen(filename,"wb");
                             if(write_f==NULL){
                                      perror("write fopen");
                                       continue;
                             }
//int  recv_n(int  acp,recv_context_t *rct);
                             char  long_buff[32]={0};
                             recv_n(acp,&rct);                                                       //循环接收push纯文件数据，第一个数据是文件总长度
                             memcpy(long_buff,rct.buffer,rct.total_len);

//long int strtol(const char *nptr, char **endptr, int base);
                            long    file_total_len=strtol(long_buff,NULL,10);
              //            printf("%ld\n",file_total_len);
                             write_context_t   wct;
                             memset(&wct,0,sizeof(wct));

                             long   writed_len=0;

                            while(1){

                                       recv_n(acp,&rct);
                                        ssize_t   n=fwrite(rct.buffer,1,rct.total_len,write_f);
                                        writed_len=writed_len+n;

                                        printf("file_total_len:%ld   wct.total_len%ld\n",file_total_len,writed_len);
                                        if(file_total_len==writed_len) {
                                                          fclose(write_f);
                                                          printf("接收%ld\n字节",writed_len);
                                                          break;
                                        }
                                               if(file_total_len<writed_len) {
                                                          fclose(write_f);
                                                          puts("push error");
                                                          break;
                                        }
                            }

                 }

    //--------DIR-------------------------------------------------------------------------------------------
            if(strcmp(ml,"dir")==0){
                             char    buff_dir[2048];
                             dir_data(buff_dir);
                             send_n(acp,buff_dir,strlen(buff_dir));

             }
//-------PULL start-----------------------------------------------------------------------------------------

             if(strcmp(ml,"pull")==0){

                      char  pull_filename[1024];
                      snprintf(pull_filename,1024,"%s%s",SERVER_PATH,buff);

                      FILE*f=fopen(pull_filename,"rb");    //buff
                     if(f==NULL){                                                                  //文件不存在
                                perror("FILE no open");
                                memset(&rct,0,sizeof(rct));

                                char  ls_file[]="file no open";              //向客户端发送“file no open”，让客户端返回循环接收状态
                                size_t    ls_file_len=strlen(ls_file);
                                send_n(acp,ls_file,ls_file_len);

                                continue;                                                              //重新接收客户端查询输入
                      }

                     char  *filename=buff;

                      while(1){                                                           //分段读文件
                            read_file(f,&rdct);
                            if(sct.state==0){                                    //首先发送文件名，再发送文件总长度
                                        send_n(acp,filename,sizeof(filename));

                                         char  bz_send[32]={'\0'};
                                        snprintf(bz_send,sizeof(bz_send),"%ld",rdct.total_len);
		                                 size_t    bz_send_len=strlen(bz_send);
		                //                 printf("total:%s\n",bz_send);
		                                 send_n(acp,bz_send,bz_send_len);    //第一次发送总字节数
		                                 sct.state=2;
                             }

                            send_n(acp,rdct.buff,rdct.len);              //发送文件数据
                            send_len=send_len+rdct.len;                      //累加发送字节数

                            if(rdct.state==3)    break;                            //读文件，发送完跳出
                    }
       //---------PULL OVER-----------------------------------------------------------------


                     if(rdct.total_len==rdct.read_len){
                            printf("成功发送%ld字节\n",rdct.total_len);
                     }

		           free(rct.old_data);                     // 必须释放
		           rct.old_data=NULL;

                  }
            }
                   close(acp);
                   puts("客户端已关闭，等待下一个...");

                   _exit(0);        //正常退出子进程

         }       //子进程over

          //-----主进程-------------------------------------------------------------------------------
          if(pd>0){
                 printf("父进程%d\n",getpid());
          }

     }      //accept() over

      close(cns.sockname);
       return 0  ;
}
//==========main() over==========================================






