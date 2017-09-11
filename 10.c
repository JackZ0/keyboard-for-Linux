#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

sig_atomic_t child_exit_status;

void clean_up_child_process(int signal_num)
{
    /* clean up child process */
    int status;
    wait (&status);

    /* store its exit status in a global variable */
    child_exit_status = status;
}

int main()
{
    /* handle SIGCHLD by calling clean_up_child_process  */
    struct sigaction sigchild_action;
    memset(&sigchild_action, 0, sizeof(sigchild_action));
    sigchild_action.sa_handler = &clean_up_child_process;
    sigaction(SIGCHLD, &sigchild_action, NULL);

    /* fork a child, and let the child process dies before parent */
    pid_t c_pid;
    c_pid = fork();
    if (c_pid > 0)
    {
        printf("in parent process, and sleep for on mininute...zZ...\n");
        sleep(60);
    }
    else if(c_pid == 0)
    {
        printf("in child process, and exit now\n");
        exit(0);
    }
    else
    {
        printf("fork failed!\n");
    }

    return 0;
}   
