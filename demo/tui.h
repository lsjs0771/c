#ifndef TUI_H_INCLUDED
#define TUI_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define  HANG  29
#define  LIE   150

#define  CLIENT_TEXT_H   3      //客户端显示文本开始位置
#define CLIENT_TEXT_L     1
#define  CLIENT_TEXT_OVER_H   HANG-2
#define CLIENT_TEXT_OVER_L    LIE/2

#define  SERVER_TEXT_H         3            //服务器显示文本开始位置
#define   SERVER_TEXT_L         LIE/2+1
#define   SERVER_TEXT_OVER_H    HANG-2
#define   SERVER_TEXT_OVER_L    LIE


#define  START_H    29
#define  START_L    1



int clear_in_h(int hang,int lie);  //清输入行
int clear_txt(int start_hang,int start_lie,int over_hang,int over_lie);

#endif // TUI_H_INCLUDED
