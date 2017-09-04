#include<stdio.h>
#include<string.h>
int main()
{
    //strcat
    char a[30]="string1ABCDE";
    char b[]="STRING2";
    printf( "before strcat() : %s\n",a );
    printf( "after strcat()  : %s\n",strcat( a, b ) );
   
    //strncat
    char a1[30]="string1";
    char b1[]="STRING2";
    printf( "before strnact(): %s\n", a1 );
    printf( "after strncat() : %s\n", strncat( a1, b1, 6 ) );   
    getchar();
    return 0;
}
