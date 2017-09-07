#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<stdio.h> 
#include<stdlib.h> 
#include<fcntl.h> 
#include<limits.h> 
#define BUFSZ PIPE_BUF 
void err_quit(char *msg) 
{ 
    perror(msg); 
    exit(EXIT_FAILURE); 
} 
int main(int argc, char *argv[]) 
{ 
    FILE* fp; 
    int count; 
    char buf[BUFSZ]; 
    char command[150]; 
    if(argc != 2) 
    {
        printf("USAGE: example <process name>\n"); 
        exit(EXIT_SUCCESS); 
    } 
    else
        sprintf(command, "ps -C %s|wc -l", argv[1] ); 
    if((fp = popen(command,"r")) == NULL) 
        err_quit("popen"); 
    if( (fgets(buf,BUFSZ,fp))!= NULL ) 
    {
        count = atoi(buf); 
        if((count - 1) == 0) 
            printf("%s not found\n",argv[1]); 
        else
            printf("process : %s total is %d\n",argv[1],(count - 1)); 
    } 
    pclose(fp); 
    exit(EXIT_SUCCESS); 
} 
