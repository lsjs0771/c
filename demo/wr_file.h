#ifndef WR_FILE_H_INCLUDED
#define WR_FILE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#define  CLIENT_FILE_PATH  "/home/wz/c/pull/"    //客户端存储文件目录

int echo_drient(char *drient_path,char (*file_buff)[75],int *len);

#endif // WR_FILE_H_INCLUDED
