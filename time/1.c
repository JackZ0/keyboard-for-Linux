#include <stdio.h>
#include <stddef.h>
#include <time.h>
int main(void)
{
    time_t timer;//time_t就是long int 类型
    struct tm *tblock;
    timer = time(NULL);
    tblock = localtime(&timer);
    printf("Local time is: %s\n", asctime(tblock));
    return 0;
}

