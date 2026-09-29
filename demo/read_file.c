#include "read_file.h"

int  read_file(FILE *file,read_context_t *rdct){

      if(rdct->state==0){                    //开始读文件长度
            int fd=fileno(file);
            struct stat st;
            fstat(fd,&st);

            rdct->total_len=st.st_size;
            rdct->state=2;
      }

      size_t n=fread(rdct->read_buff,1,READ_N,file);
      if(n>0){
         rdct->len=n;
         rdct->read_len=rdct->read_len+n;
     //    printf("read:%ld\n",rdct->read_len);
         if(rdct->read_len==rdct->total_len)  rdct->state=3;
     }else{
          if (feof(file)) {
            printf("已到达文件末尾！\n");
        } else {
            perror("读取失败");
        }
     }

     return 0;
}
