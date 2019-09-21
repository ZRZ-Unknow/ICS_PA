#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);
  FILE*fp;
  fp=fopen("/home/zrz/ics2019/nemu/tools/gen-expr/input","r");
  /* Receive commands from user. */
  char buf[101];
  while(fgets(buf,sizeof(buf),fp)){
    //int ptr;char *p=NULL;
    //ptr=strtok_r(buf," ",&p);
    //printf("%s:%d\n",p,ptr); }
    for (int i=0;i<30;i++)
    { printf("%d&&",buf[i]);}
		    printf("\n");}
  fclose(fp);  
  ui_mainloop(is_batch_mode);
  
  return 0;
}
