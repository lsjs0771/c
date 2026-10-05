#ifndef FILE_DATA_H_INCLUDED
#define FILE_DATA_H_INCLUDED

#include "write_data.h"
#include "tui.h"
#include "sock_data.h"
#include "send_data.h"
#

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>


int read_drient_echobuff(char *drient_path,node *nd);
int read_server_dirent_buff(int sock,node *nd4,node *nd3);
int pull_server(int sock,node *nd4,node *nd5,node *nd2);
#endif // FILE_DATA_H_INCLUDED
