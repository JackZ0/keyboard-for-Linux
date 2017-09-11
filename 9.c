#include <stdio.h>
#include <sys/types.h>

int main()
{
    //fork a child process
    pid_t pid = fork();

    if (pid > 0)   //parent process
    {
        printf("in parent process, sleep for one miniute...zZ...\n");
        sleep(60);
        printf("after sleeping, and exit!\n");
    }
    else if (pid == 0)  
    {
        //child process exit, and to be a zombie process
        printf("in child process, and exit!\n");
        exit(0);
    }

    return 0;
}
