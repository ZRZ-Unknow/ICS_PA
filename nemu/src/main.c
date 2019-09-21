#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor/expr.h"
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
    char *p=strtok(buf," ");
    char *q=strtok(NULL,"\n");
    bool success=true;
    int res=expr(q,&success);
    if(success){printf("%s=%s,myresult=%d\n",q,p,res);}
    else {printf("fail to eval");}
  } 
  fclose(fp);  
  ui_mainloop(is_batch_mode);
  
  return 0;
}
