#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <math.h>
void cpu_exec(uint64_t); /*unsigned long int*/
extern void isa_reg_display();
extern uint32_t paddr_read(paddr_t addr, int len);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}
static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args) {
  if (args==NULL){
    cpu_exec(1);  
    return 0;
  }
  char *ch=strtok(args," ");
  int num=atoi(ch);
  if (num<=0){
    cpu_exec(-1); 
    return 0;
  }
  cpu_exec(num);
  return 0;
}

static int cmd_info(char *args){
  if (args==NULL){
     printf("please input 'r' or 'w' after 'info' to command\n");
     return 0;
  }
  char *ch=strtok(args," ");
  if (strcmp(ch,"r")==0){
     isa_reg_display();
  }
  else if (strcmp(ch,"w")==0){
    view_watchpoint(true,-1);
  } 
  else {
     printf("please input 'r' or 'w' after 'info' to command\n");
  }
  return 0;
}

static int cmd_x(char *args){
  if (args==NULL){
     printf("please input an integer and expression\n");
     return 0;
  }
  char *ch=strtok(args," ");
  int num =atoi(ch);
  if (num<=0){
     printf("please input an positive integer\n");
     return 0;
  }
  char *expr=strtok(NULL," ");
  if (expr==NULL){
     printf("please input an hexadecimal address like 0x~\n");
     return 0;
  }
  paddr_t addr;
  sscanf(expr,"%x",&addr);
  double p=num;
  int n=ceil(p/4.0);
  for (int i=0;i<n;i++){
     printf("0x%08x: ",addr);
     if (i==num/4){
	for (int j=0;j<num%4;j++){
	  int memory=paddr_read(addr,4);
          printf("0x%08x ",memory);
          addr+=4;	  
	}
        }
     else{
        for (int j=0;j<4;j++){
          int memory=paddr_read(addr,4);
          printf("0x%08x ",memory);
          addr+=4;
         }
	}
     printf("\n");
  }
  return 0;
}

static int cmd_p(char *args){
  char *ch=strtok(NULL," ");
  bool success=true;
  uint32_t result=expr(ch,&success);
  if(success){printf("%#x\n",result);}
  else {printf("error p\n");}
  return 0;
}

static int cmd_w(char *args){
  set_watchpoint(args);
  return 0;
}

static int cmd_d(char *args){
  char *ch=strtok(args," ");
  int number=atoi(ch);
  if(number<0){printf("input an nonnegative integer");return 0;}
  else {delete_watchpoint(number);}
  return 0;
}

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Single execute" ,cmd_si},
  { "info","Print program status,'r' for register status and 'w' for watchpoint information",cmd_info},
  { "x","Scan memory",cmd_x},
  { "p","Expression evaluation",cmd_p},
  { "w","Set watchpoint",cmd_w},
  { "d","Delete watchpoint",cmd_d},
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
