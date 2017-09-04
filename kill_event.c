#include <stdio.h>
#include <sys/time.h>
#include <linux/input.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <unistd.h>

#define EVENT_NUM 12  

char *event_str[EVENT_NUM] =
{
 "IN_ACCESS",
 "IN_MODIFY",        //文件修改  
 "IN_ATTRIB",
 "IN_CLOSE_WRITE",
 "IN_CLOSE_NOWRITE",
 "IN_OPEN",
 "IN_MOVED_FROM",    //文件移动from  
 "IN_MOVED_TO",      //文件移动to  
 "IN_CREATE",        //文件创建  
 "IN_DELETE",        //文件删除  
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

#define test_bit(bit) (mask[(bit)/8] & (1 << ((bit)%8)))  

     for (i = 0; i < 32; i++) {
         sprintf(name, "/dev/input/event%d", i);
         if ((fd = open(name, O_RDONLY, 0)) >= 0) {
             ioctl(fd, EVIOCGVERSION, &version);
             ioctl(fd, EVIOCGNAME(sizeof(buf)), buf);
             ioctl(fd, EVIOCGBIT(0, sizeof(mask)), mask);
             printf("%s\n", name);
             printf("    evdev version: %d.%d.%d\n",  version >> 16, (version >> 8) & 0xff, version & 0xff);  
             printf("    name: %s\n", buf);
             printf("    features:");
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
}
int main(int argc,char **argv)
{
    
    FILE *fp;
    struct input_event ie;
    read_event();
    char name_event[30];
    sprintf(name_event, "/dev/input/event%d", gs_event_num);

    fp = fopen(name_event, "r");
    if (NULL == fp)
    {
	printf("open error \n");
    }
    while (1)
    {
        fread((void *)&ie, sizeof(ie), 1, fp);
        if (ferror(fp)) 
        {
            perror("fread input event0");
            exit(1);
        }
        if((ie.type == 1) && (ie.code == 88) &&  (ie.value == 0))
        {
            printf("kill firefox \n");
            system("ps -ef | grep firefox.sh | grep -v grep | cut -c 9-15 | xargs kill -s 9");
	    sleep(3);
        }
        else
        {
	     ;
            printf("[type:%d,code:%d,value:%d]\n",ie.type, ie.code, ie.value);
        }
    }
    return 0;
}

