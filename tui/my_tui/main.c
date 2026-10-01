//设计TUI 布局;整个TUI分为无数个NODE
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define  HANG 30
#define  LIE 20


typedef  struct NODE node;


typedef struct OOP{

      int (*clear)(node *nd);
      int (*fill_background)(node *nd);
      int (*write_node)(node *nd);

}oop;

 struct NODE{

      int  isbuff; //内存是否修改 1：修改  0：未修改

      int  H_start;   //node行开始
      int  L_start;   //node列开始
      int height;   // 高（行数）
      int width;    // 宽（列数）

      int  background_color;
      int  foreground_color;

      char *node_buff;
      int  buff_len;

      oop*  ops;
};

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

int write_node(node *nd){

    int len=nd->buff_len;
    int hang=nd->H_start;
    int lie=nd->L_start;

  //   printf("\033[%d;%dH",hang,lie);
 //    printf("\033[%d;%dm%c",nd->foreground_color,nd->background_color,nd->node_buff[0]);
   for(int n=0;n<len;n++){

       if(((n+nd->L_start)%LIE)==0){

            printf("\033[%d;%dH",hang,n%LIE);
            printf("\033[%d;%dm%c",nd->foreground_color,nd->background_color,nd->node_buff[n]);
            lie=nd->L_start;
            hang=hang+1;

        }else{

            printf("\033[%d;%dH",hang,(lie+n)%LIE);
            printf("\033[%d;%dm%c",nd->foreground_color,nd->background_color,nd->node_buff[n]);
        }
    }

     fflush(stdout);
     printf("\033[0m");
    return 0;
}
/* 全局或静态：一张共享的操作表 */
static oop node_ops = {
    .clear           = NULL,          /* 你还没实现，先留空 */
    .fill_background = fill_background,
    .write_node      = NULL,
};


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

int main()
{
    struct winsize ws;
    init_cx(HANG,LIE,&ws);

    node node1;
    node1.ops=&node_ops;

    node1.H_start=1;
    node1.L_start=1;
    node1.height=20;
    node1.width=LIE;
    node1.background_color=46;
    node1.foreground_color=30;

    node node2;
    node2.ops=&node_ops;

    node2.H_start=21;
    node2.L_start=1;
    node2.height=HANG-20;
    node2.width=LIE;
    node2.background_color=47;
    node2.foreground_color=30;

    node1.ops->fill_background(&node1);
    node2.ops->fill_background(&node2);
    fflush(stdout);

    sleep(3);

    char buff1[300]={0};

    buff1[0]='1';
    buff1[19]='z';
    buff1[20]='s';
    buff1[21]='2';
    buff1[39]='d';
    buff1[40]='s';
    node1.node_buff=buff1;
    node1.buff_len=300;

    write_node(&node1);

    getchar();
    exit_cx(&ws);
    return 0;
}
