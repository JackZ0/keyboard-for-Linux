#include<time.h>
#include<stdio.h>
int main()
{
    struct tm *t;
    time_t tt;
    time(&tt);
    t = localtime(&tt);
    printf("%4d年%02d月%02d日 %02d:%02d:%02d\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    return 0;
}

