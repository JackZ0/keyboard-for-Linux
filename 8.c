#include <stdio.h>
#include <sys/time.h>
#include <linux/input.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#define EVENT_NUM 12  
#define test_bit(bit) (mask[(bit)/8] & (1 << ((bit)%8)))  
#define BUFSZ PIPE_BUF
char *event_str[EVENT_NUM] =
{
 "IN_ACCESS",
 "IN_MODIFY",        //鏂囦欢淇敼  
 "IN_ATTRIB",
 "IN_CLOSE_WRITE",
 "IN_CLOSE_NOWRITE",
 "IN_OPEN",
 "IN_MOVED_FROM",    //鏂囦欢绉诲姩from  
 "IN_MOVED_TO",      //鏂囦欢绉诲姩to  
 "IN_CREATE",        //鏂囦欢鍒涘缓  
 "IN_DELETE",        //鏂囦欢鍒犻櫎  
 "IN_DELETE_SELF",
 "IN_MOVE_SELF"
};
int gs_event_num=0;
void read_event()
{
     char          name[64];           /* RATS: Use ok, but could be better */
     char          buf[256] = { 0, };  /* RATS: Use ok */
     unsigned char mask[EV_MAX/8 + 1]; /* RATS: Use ok */
     int           version;
     int           fd = 0;
     int           rc;
     int           i, j;
     char          *tmp;
     char name_event[19]="/dev/input/";
     FILE *fq;
     char buffer[7];
     fq = popen("ls -l /dev/input/by-id/ | grep kbd | tail -c 7","r");
     printf("fq = %d \n",fq);
     if (EINVAL == fq)
     {
       printf("errror");
     }
     fgets(buffer,sizeof(buffer),fq);
     pclose(fq);
     char *flag;
     flag = strstr(buffer,"event");
     if(flag)
     {
         strcat(name_event,buffer);
//     sprintf(name, "/dev/input/event%d", i);
         if ((fd = open(name_event, O_RDONLY, 0)) >= 0) {
         ioctl(fd, EVIOCGVERSION, &version);
         ioctl(fd, EVIOCGNAME(sizeof(buf)), buf);
         ioctl(fd, EVIOCGBIT(0, sizeof(mask)), mask);
     /*    printf("%s\n", name);
         printf("    evdev version: %d.%d.%d\n",  version >> 16, (version >> 8) & 0xff, version & 0xff);
         printf("    name: %s\n", buf);
         printf("    features:"); */
         for (j = 0; j < EV_MAX; j++) {
             if (test_bit(j)) {
                 const char *type = "unknown";
                 switch(j) {
                 case EV_KEY: type = "keys/buttons"; break;
                 case EV_REL: type = "relative";     break;
                 case EV_ABS: type = "absolute";     break;
                 case EV_MSC: type = "reserved";     break;
                 case EV_LED: type = "leds";        gs_event_num = i; break;
                 case EV_SND: type = "sound";        break;
                 case EV_REP: type = "repeat";       break;
                 case EV_FF:  type = "feedback";     break;
                 }
             }
         }
         printf("\n");
         close(fd);
      }
    }
    else
    {
        printf("not found \n");
    }
}
void err_quit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc,char **argv)
{
    FILE *fp;
    int flag_ie;
    char buf[BUFSZ];
    struct input_event ie;
    char command[40]="ps -C wfica|wc -l";
//    printf("%d \n",__LINE__);
    read_event();
//    printf("%d \n",__LINE__);
//    char name_event[30];
    int count;
    char name_event[19]="/dev/input/";
    FILE *fq;
    FILE *fp_ica;
    char buffer[7];
    int fread_flag;
    fq = popen("ls -l /dev/input/by-id/ | grep kbd | tail -c 7","r");
    fgets(buffer,sizeof(buffer),fq);
    pclose(fq);

    char *flag;
    flag = strstr(buffer,"event");
    if (flag)
   {
       strcat(name_event,buffer);
//    printf("%d \n",__LINE__);
  //  sprintf(name_event, "/dev/input/event%d", gs_event_num);
    fp = fopen(name_event, "r");
    if (NULL == fp)
    {
        printf("open error \n");
 //       system("/opt/qt_arma10/qt4/start/1.sh");
    }
  //  printf("%d \n",__LINE__);
//    if ((fp_ica = popen(command,"r"))==NULL);
 //        err_quit("popen");
    /*fp_ica = popen(command,"r");
    if(EINVAL == fp_ica)
    {
        printf("is error\n");
    }
    else
    {
        printf("%d \n",__LINE__);
        if((fgets(buf,BUFSZ,fp_ica))!=NULL)
        {
            count = atoi(buf);
        }
        printf("%d \n",__LINE__); 
        pclose(fp_ica);*/
        while (1)
        {
           // printf("%d \n",__LINE__);
           fread_flag = fread((void *)&ie, sizeof(ie), 1, fp);
            printf("%d %d \n",__LINE__,fread_flag);
           if(fread_flag == 0)
           {
                sleep(3);
           }
            if (ferror(fp))
            {
                 perror("fread input event0");
                 exit(1);
            }
            if((ie.type == 1) && (ie.code == 88) &&  (ie.value == 0))
            {
                if((count - 1) == 0)
                {
                       system("kill -9 $(ps -ef|grep -E 'firefox | /usr/bin/X | openbox | chvt '|grep -v grep|awk '{print $2}')");
  //                   system("ps -ef | grep firefox.sh | grep -v grep | cut -c 9-15 | xargs kill -s 9");
            //           printf("not found ica");
                }
                else
                {
            //     system("ps -ef | grep firefox.sh | grep -v grep | cut -c 9-15 | xargs kill -s 9");
                 ;
                }
             }
             else
             {
                 ;
     //       printf("[type:%d,code:%d,value:%d]\n",ie.type, ie.code, ie.value);
             }
         }
      //}
  }
  else
  {
    ;
  }
      return 0;
}

