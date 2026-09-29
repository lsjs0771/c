#include "wr_file.h"

int echo_drient(char *drient_path,char (*file_buff)[75],int *len){
     DIR *dr=opendir(drient_path);
     struct dirent *drt=NULL;
     int n=0;
     while((drt=readdir(dr))!=NULL){
          if(strcmp(drt->d_name,".")==0)  continue;
          if(strcmp(drt->d_name,"..")==0)  continue;

          strcpy(file_buff[n],drt->d_name);
          n++;
     }
      *len=n;
      closedir(dr);
      return 0;
}
