#include <stdio.h>
#include <sys/stat.h>

#include  "read_file.h"


int read_file(FILE *f,read_context_t *rdct){

    if(rdct->state==0){                    //初始化fd
           int  fd=fileno(f);
           if (fd==-1){
                perror("FILENO");
               return  -1;
           }

          rdct->fd=fd;
          rdct->state=2;                       //读取中

          struct stat   st1;
           memset(&st1,0,sizeof(st1));

          if(fstat(fd,&st1)==-1){
                perror("stat");
                return -1;
         }
           rdct->total_len=st1.st_size ;  //文件总长度
    }
//--------------------------------------------------------------------------
     memset((rdct->buff),0,READ_N);                                //存储数据区清0

     int   read_1_n=fread(rdct->buff,1,READ_N,f);        //读取存储数据

     if(read_1_n==READ_N){                                             //读取文件中
              rdct->state=2;                                               //状态读取中
              rdct->len=read_1_n;
              rdct->read_len=(rdct->read_len)+read_1_n;      //累计已读取长度
     }

     if((read_1_n<READ_N)&&(read_1_n!=0)){
              rdct->len=read_1_n;
              rdct->read_len=(rdct->read_len)+read_1_n;      //累计已读取长度
              rdct->state=3;                                                 //文件已读完

              fclose(f);                               //关闭文件

     }

     if(read_1_n==0){
            rdct->state=4;               //读取失败
            rdct->last_errno=4;    //保存读取错误
            perror("read_n_file");
     }

    return 0;
}
