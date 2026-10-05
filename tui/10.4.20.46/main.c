//设计TUI 布局;整个TUI分为无数个NODE
//
#include "tui.h"
#include "file_data.h"
#include "read_file.h"
#include "send_data.h"
#include "sock_data.h"
#include "write_data.h"

int main()
{
     //==========socket===============================================================================

     int port=8000;
     char *ip="192.168.101.144";
     int  sock=kfsocket(port,ip);

     //============================================================================================

//==============TUI 设置============================================================
    struct winsize ws;
    init_cx(HANG,LIE,&ws);

 //----------设定TUI-----------------------------------

    node node1;
    node1.ops=&node_ops;

    node1.H_start=1;      //1
    node1.L_start=1;      //1
    node1.height=1;      //20
    node1.width=LIE;       //LIE
    node1.background_color=46;
    node1.foreground_color=30;

    node node2;
    node2.ops=&node_ops;

    node2.H_start=2;     //20
    node2.L_start=1;       //1
    node2.height=28;   //HANG-20
    node2.width=LIE/2;         //LIE
    node2.background_color=47;
    node2.foreground_color=30;

    node node3;
    node3.ops=&node_ops;

    node3.H_start=2;      //1
    node3.L_start=LIE/2+1;      //1
    node3.height=28;      //20
    node3.width=LIE/2;       //LIE
    node3.background_color=45;
    node3.foreground_color=30;

    node node4;
    node4.ops=&node_ops;

    node4.H_start=30;      //1
    node4.L_start=1;      //1
    node4.height=1;      //20
    node4.width=LIE/2;       //LIE
    node4.background_color=43;
    node4.foreground_color=30;


    node node5;
    node5.ops=&node_ops;

    node5.H_start=30;      //1
    node5.L_start=LIE/2+1;      //1
    node5.height=1;      //20
    node5.width=LIE/2;       //LIE
    node5.background_color=46;
    node5.foreground_color=30;

    node1.ops->fill_background(&node1);
    node2.ops->fill_background(&node2);
    node3.ops->fill_background(&node3);
    node4.ops->fill_background(&node4);
    node5.ops->fill_background(&node5);

    fflush(stdout);
 //-------------TUI各个node 显示内容-----------------------------------------------------------------------------
     char *buff1=realloc(0,node1.height*node1.width);
     char *buff2=realloc(0,node2.height*node2.width);
     char *buff3=realloc(0,node3.height*node3.width);
     char *buff4=realloc(0,node4.height*node4.width);
     char *buff5=realloc(0,node5.height*node4.width);

    strcpy(buff1,"               client                                                                                       server");
    strcpy(buff2,"client");
    strcpy(buff3,"server");
    strcpy(buff4,"input:");
    strcpy(buff5,"output:");

  //------------循环显示TUI每个node-----------------------------------------------------------------------------------------
     int len=0;

     node2.isbuff=1;
     node3.isbuff=1;

     node1.node_buff=buff1;
     node2.node_buff=buff2;
     node3.node_buff=buff3;
     node4.node_buff=buff4;
     node5.node_buff=buff5;    //显示输出框

     echo_node(&node1);       //显示标题node
     echo_node(&node2);
     echo_node(&node3);
     echo_node(&node4);
     echo_node(&node5);
     //---------判断服务器状态----------------------------------------------------------
       if(sock==-1){
    //      memset(node5.node_buff,0,node5.buff_len);
          strcpy(node5.node_buff,"Cannot connect to the server,Exit in 2 seconds ");
          echo_node(&node5);
          sleep(2);
          return 0;
     }else{

   //         memset(node5.node_buff,0,node5.buff_len);
           strcpy(node5.node_buff,"connect to server");
           echo_node(&node5);
     }
     //========循环键盘输入，接收。发送================================
    node5.isbuff=0;

    while(1){

//----------输入--------------------------------------------------------------------


        keyboad(node4.node_buff,&len,&node4);

        if(strcmp(node4.node_buff,"exit")==0){     //退出程序
            break;
        }

        if(strcmp(node4.node_buff,"dirc")==0){    //显示本地pull目录
          //      memset(node4.node_buff,0,node4.buff_len);  //清空输入 buff
                 clear(&node2);

                memset(node2.node_buff,0,node2.buff_len);
                int bz=read_drient_echobuff(PULL_PATH,&node2);     //显示本地目录
                echo_left_txt(&node2);

                if(bz==0){
                     memset(node5.node_buff,0,node5.buff_len);
                     strcpy(node5.node_buff,"Successfully displayed the local directory.");
                     echo_node(&node5);
                }else{
                     memset(node5.node_buff,0,node5.buff_len);
                     strcpy(node5.node_buff,"Failed to display the local directory");
                     echo_node(&node5);
                }
                continue;
        }

        if(strcmp(node4.node_buff,"dir")==0){
              clear(&node3);

              int bz=read_server_dirent_buff(sock,&node4,&node3);         //显示服务器目录

               memset(node5.node_buff,0,node5.buff_len);
               strcpy(node5.node_buff,"Successfully displayed the server directory");
               echo_node(&node5);

               continue;
        }

        if(strspn(node4.node_buff,"pull")==4){

              int bz=pull_server(sock,&node4,&node5,&node2);
              if(bz==-1){                            //pull 没有输入文件名
                  continue;
              }
              continue;
        }

        if(strcmp(node4.node_buff,"push")==0){
               memset(node5.node_buff,0,node5.buff_len);    //显示没有这个功能
               strcpy(node5.node_buff,"Successfully uploaded the file to the server");
               echo_node(&node5);
              continue;

        }

         memset(node5.node_buff,0,node5.buff_len);    //显示没有这个功能
         strcpy(node5.node_buff,"Command not supported");
         echo_node(&node5);

    //------------------------------------------------------------------
    }

    free(buff1);
    free(buff2);
    free(buff3);
    free(buff4);
    free(buff5);
    exit_cx(&ws);
    return 0;
}
