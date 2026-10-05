#ifndef TUI_H_INCLUDED
#define TUI_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

#define  HANG 30
#define  LIE 160

#define PULL_PATH "/home/wz/c/pull/"

typedef  struct NODE node;


typedef struct OOP{

      int (*clear)(node *nd);
      int (*fill_background)(node *nd);
      int (*write_node)(node *nd);

}oop;

//typedef struct nbuff   NBUFF;

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


//-----------------------------------------------------------------------
int clear(node *nd);
int fill_background(node *nd);
int write_node(node *nd);
int  init_cx(int hang,int lie, struct winsize * ws);
int  exit_cx(struct winsize* ws);
int keyboad(char *buf,int *len,node *nd);
/* 全局或静态：一张共享的操作表 */

static oop node_ops = {
    .clear           = clear,
    .fill_background = fill_background,
    .write_node      = write_node,
};

#endif // TUI_H_INCLUDED
