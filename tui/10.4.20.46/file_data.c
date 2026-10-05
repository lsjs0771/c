#include "file_data.h"

int pull_server(int sock,node *nd4,node *nd5,node *nd2){

     char  pullname[128]={0};
     strcpy(pullname,nd4->node_buff);
     char *pull_ls_name=strtok(pullname,"  ");
     pull_ls_name=strtok(NULL," ");

     if(pull_ls_name==NULL){
          clear(nd5);
          printf("\033[%d;%dH",nd5->H_start,nd5->L_start);
          printf("\033[%d;%dm%s",nd5->foreground_color,nd5->background_color,"pull:No input file name ");

          return -1;
     }

     //向服务器发送pull命令 //
        data_type_t  dtt;
        dtt.type='m';                               //设置为命令模式
        strcpy(dtt.ls,"12345678");                        //此9个字节为保留，现在不
        char *ls_ml=malloc(10+nd4->buff_len);     //拼接请求类型和请求数据，m代表命令
        memcpy(ls_ml,&dtt,10);
        sprintf(ls_ml+10,"%s",nd4->node_buff);
        int bz=send_n(sock,ls_ml,nd4->buff_len+10);//向服务器发送查询  //
        free(ls_ml);
        ls_ml=NULL;

  //接收服务器返回数据//
        recv_context_t  rct;
        write_context_t wct;
        memset(&rct,0,sizeof(rct));
        memset(&wct,0,sizeof(wct));
        rct.state=0;
        FILE *f=NULL;
        while(1){

            if(rct.state==0){

                 recv_n(sock,&rct);                  //首先接收文件名或者“file no open"(服务器没有查询的文件）
                 char * filename=rct.buff;

                 if(memcmp(filename,"file no open",sizeof(filename))==0) {     //无文件退出循环
                 //    puts("服务器没有此查询文件");
                //break;
                     clear(nd5);
                     printf("\033[%d;%dH",nd5->H_start,nd5->L_start);
                     printf("\033[%d;%dm%s",nd5->foreground_color,nd5->background_color,"pull:server no file");
                     break;
                 }

                 if(memcmp(filename,"please into filename",sizeof(filename))==0) {     //无文件退出循环
                 //   puts("没有输入文件名");
                 //   break;
                       clear(nd5);
                       printf("\033[%d;%dH",nd5->H_start,nd5->L_start);
                       printf("\033[%d;%dm%s",nd5->foreground_color,nd5->background_color,"pull:please into filename");
                       break;
                 }


                 char pull_filename[1024];
                 snprintf(pull_filename,1024,"%s%s",PULL_PATH,filename);
                 f=fopen(pull_filename,"w+b");                   // 写入文件
                 if(f==NULL){
                  //     perror("FILE build");
                  //     break;
                  }

                  recv_n(sock,&rct);                      //接收文件总字节数
                  long ls_len=strtol(rct.buff,NULL,10);   //字符串转long int

                  rct.recv_len=0;                        //开始记录接收的字节数
                  rct.state=2;                           //进入接收文件状态
                  rct.total_len=ls_len;                  // 总文件长度

                clear(nd5);
                printf("\033[%d;%dH",nd5->H_start,nd5->L_start);
                printf("\033[%d;%dm%s",nd5->foreground_color,nd5->background_color,"pulling...");

                 }

                recv_n(sock,&rct);                            //循环接收文件，以64k 为单位
                write_n(f,&wct,&rct);                         //循环写入文件，以64k为单位

               if((rct.total_len!=0)&&(rct.total_len==wct.recv_len)){     //当写入的总字节数等于文件总字节数退出接收状态
                    fclose(f);
               //   printf("成功接收:%ld字节\n",rct.recv_len);
                    clear(nd5);
                    printf("\033[%d;%dH",nd5->H_start,nd5->L_start);
                    printf("\033[%d;%dm%s",nd5->foreground_color,nd5->background_color,"pull:over");

                    break;
              }
        }

       read_drient_echobuff(PULL_PATH,nd2);
        echo_left_txt(nd2);

    return 0;
}

int read_drient_echobuff(char *drient_path,node *nd){
     int hang=nd->H_start;
     int lie_len=nd->width;

     DIR *dr=opendir(drient_path);
     if(dr==NULL){
        return -1;
     }
     struct dirent *drt=NULL;
     char *base=nd->node_buff;
     int nn=0;
     while((drt=readdir(dr))!=NULL){
          if(strcmp(drt->d_name,".")==0)  continue;
          if(strcmp(drt->d_name,"..")==0)  continue;

          nn=sprintf(nd->node_buff,"%s  ",drt->d_name);
          nd->node_buff=nd->node_buff+nn;

     }
      nd->buff_len=nn;
      nd->node_buff=base;      //初始化位置
      closedir(dr);

      return 0;
}
 //========================================================
 int read_server_dirent_buff(int sock,node *nd4,node *nd3){
        data_type_t  dtt;
        dtt.type='m';                               //设置为命令模式
        strcpy(dtt.ls,"12345678");                        //此9个字节为保留，现在不

        char *ls_ml=malloc(10+nd4->buff_len);     //拼接请求类型和请求数据，m代表命令
        memcpy(ls_ml,&dtt,10);
        sprintf(ls_ml+10,"%s",nd4->node_buff);

        int bz=send_n(sock,ls_ml,nd4->buff_len+10);//向服务器发送查询  //
        free(ls_ml);
        ls_ml=NULL;

         recv_context_t  rct;
         int wz=recv_n(sock,&rct);

         int  hang=nd3->H_start;
        int  lie=nd3->L_start;
        char *xs_name=strtok(rct.buff,"  ");
        while(xs_name!=NULL){
             printf("\033[%d;%dH",hang,lie);
             printf("\033[%d;%dm%s",nd3->foreground_color,nd3->background_color,xs_name);
             xs_name=strtok(NULL,"   ");
            hang++;
      }

        free(ls_ml);
        return 0;
 }
