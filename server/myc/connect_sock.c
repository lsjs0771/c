#include  "connect_sock.h"

//==========fsqsocket()==========================================
int fwqsocket(int port,char *ip,connect_sock_t *cns){
      
    int sk=socket(AF_INET,SOCK_STREAM,0);
    if(sk==-1){
	    perror("socket");
	    return -1;
    }
    int opt = 1;
    if (setsockopt(sk, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
         perror("setsockopt");
    }

    struct sockaddr_in  skaddr4;
    skaddr4.sin_family=AF_INET;
    skaddr4.sin_port=htons(port);
    inet_pton(AF_INET,ip, &skaddr4.sin_addr);

// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
   if(bind(sk,(struct sockaddr*)(&skaddr4),sizeof(skaddr4))==-1){
	   perror("bind");
	   return -1;
   }

   if(listen(sk,100)==-1){
	 perror("listen");
	 return -1;
   }
	
	 cns->sockname=sk;
      return  0;
}