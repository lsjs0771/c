#include "tui.h"


int clear_in_h( int hang,int lie){
    printf("\033[%d;%dH",START_H-1,START_L+10);
    for(int n=0;n<LIE/2-12;n++){
        putchar(' ');
    }
    return 0;
}

int clear_txt(int start_hang,int start_lie,int over_hang,int over_lie){
    for(int n=start_hang;n<over_hang;n++){
         for(int m=start_lie;m<over_lie;m++){
              printf("\033[%d;%dH",n,m);
              putchar(' ');
         }

    }

     printf("\033[%d;%dH",start_hang,start_lie);
}
