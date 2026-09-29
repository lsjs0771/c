#include "write_data.h"

int  write_n(FILE *f,write_context_t *wct,recv_context_t *rct){

        ssize_t ret=fwrite(rct->buff,1,rct->len,f);
        if(ret==0){
            perror("fwrite");
            return  -1;
        }
        wct->recv_len=wct->recv_len+ret;
        return 0;
}
