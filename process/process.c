#include "process.h"
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#define SIZE 1024 
int exit_flag = 0;

struct proc_struct must_run_proc[SIZE];
int must_run_size;

int check_proc(struct proc_struct *array)
{
    DIR *dp;
    char *dir = PROCESS;
    struct dirent *dirp;
    struct proc_struct *proc;
    struct proc_struct * tmp;
    if (!array)
    {
        return 0;
    }
    while((dirp = readdir(dp))!=NULL)
    {
        char data[30];
        sprintf(data,"%s",dirp->d_name);
        if(IsDigit(data))
        {
             proc = (struct proc_struct *)(malloc(sizeof(struct proc_struct)));
             tmp = proc;
             proc->pid = atoi(dirp->d_name);
             if(proc_find(proc,array))
             {
                 return 1;
             }
             free(tmp);
        }
    }
    closedir(dp);
    return 0;
}

int proc_find(struct proc_struct *src,struct proc_struct *dest)
{
    char buffer[4096],*p,cmd[20];
    int fd,len;
    sprintf(buffer,"%d/stat",src->pid);
    fd = open(buffer,O_RDONLY);
    if(fd == -1)
    {
        return 0;
    }
    memset(buffer,'\0',sizeof(buffer));
    len = read(fd,buffer,sizeof(buffer)-1);
    close(fd);
    if(len <=0 )
    {
        return 0;
    }
    p = buffer;
    p = strrchr(p,'(');
    {
        char *q=strrchr(p,')');
        int len = q-p-1;
        if(len >= sizeof(src->name))
        {
            len = sizeof(src->name)-1;
        }
        memcpy(src->name,p+1,len);
        src->name[len] = 0;
        p = q + 1;
    }
    return (strcmp(src->name,dest->name) == 0)?1:0;

}

int get_proc(struct proc_struct *array,int size,char *file)
{
    int nRet = 0;
    char *tmp;
    if (!array || (size <= 0) || !file)
    {
        myprintf("invalid parameter \n");
        return 0;
    }
    char line[4096];
    FILE *fp = fopen(file,"r");
    if (!fp)
    {
        printf("open file %s fail \n",file);
        return 0;
    }
    memset(line,0,4096);
    while(fgets(line,4095,fp)&&nRet<size)
    {
        memcpy((void *)((&array[nRet])->cmdline),(void *)line,strlen(line)-2);
        tmp = strrchr(line,'/');
        tmp +=1;
        memcpy((&array[nRet])->name,tmp,strlen(tmp)-2);
        nRet++;
    }
    fclose(fp);
    return (nRet);
}

int startProc(struct proc_struct *proc,struct proc_struct* must_run_proc,int must_run_size)
{
    static int i = 0;
    if (!proc)
      return 0;
    if(strlen(proc->cmdline) <= 0)
      return 0;

    int pid = fork();
    int a = 1;
    if (pid == 0)
    {
        pid = fork();
        if(pid == 0)
        {
            execl((char *)proc->cmdline,(char *)proc->name,NULL);
        }
        else 
        {
            exit(0);
        }
        sleep(2);
    }
    else 
    {
        wait(NULL);
        sleep(1);
    }
    if(i == must_run_size-1)
    {
        i = -1;
    }
    i++;
    if(check_proc(&must_run_proc[i]) == 0)
    {
        startProc(&must_run_proc[i],must_run_proc,must_run_size);
    }
    else 
    {
        if(i == must_run_size -1 )
            i = 0;
        else
        {
            i++;
            startProc(&must_run_proc[i],must_run_proc,must_run_size);
        }
    }
}


int moniter_run(struct proc_struct * must_run_proc,int proc_size)
{
    int i = 0;
    for (i = 0; i < must_run_size; i++)
    {
        if(check_proc(&(must_run_proc[i]))<=0)
        {
            startProc(&(must_run_proc[i]),must_run_proc,proc_size);
        }
    }
    return 1;
}

void exit_proc(int ar)
{
    int i;
    struct proc_struct* proc;

    for(i = 0; i < must_run_size; i++)
    {
        proc = &(must_run_proc[i]);
        kill(proc->pid,SIGTERM);
    }
    exit_flag=1;
    exit(0);
}

void * run_moniter(void *data)
{
    must_run_size = get_proc(must_run_proc,SIZE,data);
    if (must_run_size <= 0)
    {
        return 0;
    }
    struct sigaction act,oldact;
    act.sa_handler = exit_proc;
    act.sa_flags = SA_ONESHOT;
    sigaction(SIGTERM,&act,NULL);
    sigaction(SIGINT,&act,NULL);
    sigaction(SIGHUP,&act,NULL);
    
    moniter_run(must_run_proc,must_run_size);
    return NULL;
}

int creat_and_run_moniter(char *file)
{
    pthread_t moniter_thread;
    if(pthread_create(&moniter_thread,NULL,run_moniter,file)==0)
    {
        printf("thread create Ok ,check thread start\n");
        return 0;
    }
    else
    {
        printf("thread check creat erro \n");
        return -1;
    }
}

static int IsDigit(char a[])
{
    int size,i;
    size = strlen(a);
    if (size == 0)
     return 0;
    for(i = 0; i < size; i++)
    {
         if(a[i] < '0' || a[i] > '9')
         {
             return 0;
         }
    }
    return 0;
}



