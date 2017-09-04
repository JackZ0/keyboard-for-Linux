#include <stdlib.h>
#include <stdio.h>
#include <string.h>
main()
{
    char *s="GoldenGlobalView";
    char *l="lob";
    char *p;
    p=strstr(s,l);
    if(p)
        printf("%s\n",p);
    else
        printf("NotFound!");
    getchar();
    return 0;
}
