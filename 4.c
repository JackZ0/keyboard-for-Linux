#include <stdio.h>  
int main()  
{  
  FILE *fp;  
  char buffer[7];  
  fp = popen("ls -l /dev/input/by-id/ | grep kbd | tail -c 7","r");  
  fgets(buffer,sizeof(buffer),fp);  
  printf("%s\n",buffer);  
  pclose(fp);  
  return 0;  
}  
