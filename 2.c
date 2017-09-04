#include <stdio.h>
int main()
{
 FILE *fp;
 char buffer[7];
 fp = popen("ls -l /dev/input/by-id/ | grep kbd | tail -c 7","r");
 if(fp == NULL)
 {
    printf("is nothing \n");
 }
 printf("fp = %d \n",fp);
 fgets(buffer,sizeof(buffer),fp);
 char *p;
 p= strstr(buffer,"event");
 if (p)
 {
     printf("%s\n",p);
 }
 else
 {
     printf("NotFound!\n");
 }
 printf("%s %d \n",buffer,fp);
 printf("%d \n",sizeof(buffer));
 pclose(fp);
 return 0;
}

