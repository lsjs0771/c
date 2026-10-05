#include "file_data.h"


int echo_drient(char *drient_path,node *nd){
     int hang=nd->H_start;
     int lie_len=nd->width;

     DIR *dr=opendir(drient_path);
     if(dr==NULL){
        return -1;
     }
     struct dirent *drt=NULL;

     while((drt=readdir(dr))!=NULL){
          if(strcmp(drt->d_name,".")==0)  continue;
          if(strcmp(drt->d_name,"..")==0)  continue;

          sprintf(nd->node_buff,"%s",drt->d_name);
          nd->node_buff=nd->node_buff+lie_len;

     }

      closedir(dr);
      return 0;
}
