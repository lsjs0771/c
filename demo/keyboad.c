#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>



int keyboad(char file_in[1024],int *char_in_len){

	char sr[1024]={'\0'};
	read(0,sr,1024);
	char *o=strtok(sr,"\n");

	int n=0;
	while(o[n]!='\0'){
	   n++;
	}
//	printf("%d\n",n);
    o[n]='\0';
	strncpy(file_in,o,n+1);
	*char_in_len=n+1;
	return 0;
}
