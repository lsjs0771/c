#include "tui.h"

int echo_left_txt(node *nd){

     int hang=nd->H_start;
     char *xs_name=strtok(nd->node_buff,"  ");
     while(xs_name!=NULL){
          printf("\033[%d;%dH",hang,1);
          printf("\033[%d;%dm%s",nd->foreground_color,nd->background_color,xs_name);
          xs_name=strtok(NULL,"   ");
          hang++;
     }

     return 0;
}

int echo_right_txt(node *nd,char *server_char){
     int  hang=nd->H_start;
     int  lie=nd->L_start;
     char *xs_name=strtok(server_char,"  ");
     while(xs_name!=NULL){
          printf("\033[%d;%dH",hang,lie);
          printf("\033[%d;%dm%s",nd->foreground_color,nd->background_color,xs_name);
          xs_name=strtok(NULL,"   ");
          hang++;
     }
    return 0;
}

int clear(node *nd){

    int len=nd->height*nd->width;
    int hang=nd->H_start;
    int lie=nd->L_start;
    int high=nd->height;
    int witd=nd->width;

   for(int n=0;n<high;n++){
        for(int m=0;m<witd;m++){

            printf("\033[%d;%dH",n+hang,lie+m);
            printf("\033[%d;%dm%c",nd->foreground_color,nd->background_color,' ');
        }
    }

     fflush(stdout);
     printf("\033[0m");
    return 0;
}

int fill_background(node *nd){

    int hang=nd->H_start;
    int lie=nd->L_start;
    int lie_len=nd->width;
    int hang_len=nd->height;

    for(int n=lie;n<lie+lie_len;n++){
         for(int m=hang;m<hang+hang_len;m++){
              printf("\033[%d;%dH",m,n);
              printf("\033[%dm%c",nd->background_color,' ');
         }
    }
    fflush(stdout);
    printf("\033[0m");

    return 0;
}
int echo_node(node *nd){

    clear(nd);

    nd->buff_len=nd->height*nd->width;
    int len=nd->buff_len;
    int hang=nd->H_start;
    int lie=nd->L_start;

    int  h_len=nd->L_start+nd->width-1;

   for(int n=0;n<len;n++){

       if(((n+nd->L_start)%h_len)==0){

            printf("\033[%d;%dH",hang,n%nd->width+lie-1);
            printf("\033[%d;%dm%c",nd->foreground_color,nd->background_color,nd->node_buff[n]);
            lie=nd->L_start;
            hang=hang+1;

        }else{

            printf("\033[%d;%dH",hang,(lie+n)%nd->width+lie-1);
            printf("\033[%d;%dm%c",nd->foreground_color,nd->background_color,nd->node_buff[n]);
        }
    }
     nd->isbuff=0;
     fflush(stdout);
     printf("\033[0m");
    return 0;
}
int  init_cx(int hang,int lie, struct winsize * ws){        //初始化终端窗口
     ioctl(STDOUT_FILENO, TIOCGWINSZ, ws);
     int orig_rows = ws->ws_row;
     int orig_cols = ws->ws_col;              //保存启动时终端大小

     printf("\033[?1049h");
     printf ("\033[8;%d;%dt",hang,lie);
     printf( "\033[2J");
     return 0;
}
int  exit_cx(struct winsize* ws){                 //退出窗口
      printf("\033[?1049l");
      printf ("\033[8;%d;%dt",ws->ws_row,ws->ws_col);   //恢复启动时终端大小
      return 0;
}
int keyboad(char *buf,int *len,node *nd){

  //  memset(nd->node_buff,0,LIE/2);

    struct termios oldt, newt;
    int nm = 0;
    char ch=0;

    // 保存并设置终端
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON | IEXTEN );  // 关回显 + 关行缓冲 //newt.c_lflag &= ~(ECHO | ICANON);
    newt.c_cc[VMIN]  = 1;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

     printf("\033[%d;%dH",nd->H_start,nm+1);
     printf("\033[%d;%dm%s",nd->foreground_color,nd->background_color,"Input:");

    while(1){

     ch = getchar();
     if (ch == '\r' || ch == '\n'){
        buf[nm]='\0';
        break;
     }

     printf("\033[%d;%dH",nd->H_start,nm+7);
     printf("%c",ch);
     buf[nm]=ch;
     nm++;
    }
     *len=nm;
    // 恢复终端
      nd->buff_len=nm;
    for(int m=0;m<LIE/2;m++){
         printf("\033[%d;%dH",nd->H_start,m);
         printf("\033[%d;%dm%c",nd->foreground_color,nd->background_color,' ');

    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

 //   printf("%s\n",buf);
    return 0;
}
