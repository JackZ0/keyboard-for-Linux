#include<stdio.h>  
#include<string.h>  
#include<stdlib.h>  
#include<errno.h>  
#include<dirent.h>  
#include<sys/types.h>  
#include<sys/stat.h>  
#include<unistd.h>  
#include<limits.h>  
char *fullpath;  
int readfile(char *k)  
{  
    DIR * dp;  
    struct dirent * dirrp;  
    struct stat buf;  
  
    //创建一个p指针指向fullpath的最末尾  
    char *p;  
    p = fullpath + strlen(fullpath);  
  
    if(-1 == lstat(fullpath,&buf))  
    {  
        printf("%s[lstat]%s\n",fullpath,strerror(errno));  
        return -1;  
    }  
  
//***************如果不是目录文件则返回********************************//  
    if(S_ISDIR(buf.st_mode)==0)  
    {  
        printf("%s ",k);  
        return 1;  
    }  
  
//***************如果是目录文件则继续********************************//  
  
    //如果是目录且在尾部没有/符号,那么就在这个文件名尾部加一个/符号  
    if('/' != *(p-1))  
    {  
        *p++ = '/';  
        *p = 0;  
    }  
  
    //输出这个目录  
    printf("%s:\n",fullpath);  
    if(NULL == (dp=opendir(fullpath)))  
    {  
        printf("%s[opendir]%s\n",fullpath,strerror(errno));  
        return -1;  
    }  
    while(NULL != (dirrp=readdir(dp)))  
    {  
    //将目录下的..和.文件忽略  
    if((strcmp(dirrp->d_name,".")==0) || (strcmp(dirrp->d_name,"..")==0))  
        continue;  
    strcpy(p,dirrp->d_name);  
  
    if(0 == readfile(p)) //遍历完一个目录文件  
        printf("\n");  
    }  
    p[-1]=0;    //将后面递归增加的名称全部去掉  
    if( -1 == closedir(dp))  
    {  
    printf("%s[closedir]%s\n",fullpath,strerror(errno));  
    return -1;  
    }  
  
    return 0;  
}  
int main(int argc,char *argv[])  
{  
    if(argc != 2)  
    {  
        printf("Usage:%s <Dir name>\n",argv[0]+2);  
        exit(EXIT_FAILURE);  
    }  
  
    //最终文件的路径是保存在fullpath中的，这里对fullpath进行了初始化  
    fullpath = (char *)malloc(sizeof(char)*PATH_MAX);  
    memset(fullpath,0,PATH_MAX);  
  
    strcpy(fullpath,argv[1]);  
    readfile(fullpath);  
    printf("\n");  
      
    return 0;  
}  
