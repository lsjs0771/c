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

     node1.node_buff=buff1;   //显示标题node
     write_node(&node1);

     node5.node_buff=buff5;    //显示输出框

     node2.isbuff=1;
     node3.isbuff=1;

     //==========socket===============================================================================
//======socket=====================================================================
     int port=8000;
     char *ip="192.168.101.233";
     int  sock=kfsocket(port,ip);

     if(sock==-1){
    //      memset(node5.node_buff,0,node5.buff_len);
          strcpy(node5.node_buff,"server no line");
     }else{

   //         memset(node5.node_buff,0,node5.buff_len);
           strcpy(node5.node_buff,"connect server");
     }
        data_type_t  dtt;
        dtt.type='m';                               //设置为命令模式
        strcpy(dtt.ls,"12345678");                        //此9个字节为保留，现在不用

        char *ls_ml=malloc(10+file_int_len);     //拼接请求类型和请求数据，m代表命令
        memcpy(ls_ml,&dtt,10);
        memcpy(ls_ml+10,file_in,file_int_len);


     //============================================================================================


    while(1){

        node2.node_buff=buff2;
        if(node2.isbuff==1){              //只显示一次
            write_node(&node2);
            memset(node2.node_buff,0,node2.buff_len);
        }
        node3.node_buff=buff3;
        if(node3.isbuff==1){             //只显示一次
             write_node(&node3);
             memset(node3.node_buff,0,node3.buff_len);
        }

         node4.node_buff=buff4;    //显示输入框
         write_node(&node4);


         write_node(&node5);


//----------输入---------------------------------------------------------------------
        keyboad(node4.node_buff,&len,&node4);

        if(strcmp(node4.node_buff,"exit")==0){     //退出程序
            break;
        }

        if(strcmp(node4.node_buff,"dirc")==0){    //显示本地pull目录
                node2.isbuff=1;                    //client node 可写
                memset(node4.node_buff,0,node4.buff_len);  //清空输入 buff

                int bz=echo_drient(PULL_PATH,&node2);
                if(bz==0){
                     node2.isbuff=1;
                     memset(node5.node_buff,0,node5.buff_len);
                     strcpy(node5.node_buff,"dirc ok");
                }else{
                     memset(node5.node_buff,0,node5.buff_len);
                     strcpy(node5.node_buff,"dirc no ok");

                }
                continue;
        }

        if(strcmp(node4.node_buff,"dir")==0){
               memset(node5.node_buff,0,node5.buff_len);    //显示没有这个功能
               strcpy(node5.node_buff,"dir ok");
               continue;
        }

        if(strcmp(node4.node_buff,"pull")==0){
              memset(node5.node_buff,0,node5.buff_len);    //显示没有这个功能
              strcpy(node5.node_buff,"pull ok");
             continue;
        }

        if(strcmp(node4.node_buff,"push")==0){
               memset(node5.node_buff,0,node5.buff_len);    //显示没有这个功能
               strcpy(node5.node_buff,"push ok");
              continue;

        }

         memset(node5.node_buff,0,node5.buff_len);    //显示没有这个功能
         strcpy(node5.node_buff," not supported ");

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
