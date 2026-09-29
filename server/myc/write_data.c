#include "write_data.h"
#include  "recv_data.h"

//========readfile()==========================================
int    write_n(FILE *f,recv_context_t *rct,write_context_t *wct){

            ssize_t  ret=fwrite(rct->buffer,1,rct->total_len,f);
            if(ret=0){
                 perror("fwrite");
                 return  -1;

            }
           wct->write_len=wct->write_len+ret;
	      return 0;
}
