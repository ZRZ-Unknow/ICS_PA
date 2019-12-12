#include "proc.h"

#define MAX_NR_PROC 4
extern void naive_uload(PCB *pcb, const char *filename);
extern void context_kload(PCB *pcb, void *entry); 
extern void context_uload(PCB *pcb, const char *filename);
static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
    break;
  }
}

void init_proc() {
  context_kload(&pcb[0],(void*)hello_fun);
  //context_uload(&pcb[0],"/bin/hello");
  context_uload(&pcb[1],"/bin/init");
  switch_boot_pcb();
  Log("Initializing processes...");

  // load program here
  //naive_uload(NULL,"/bin/init");
}

_Context* schedule(_Context *prev) {
  current->cp=prev;
  current=(current==&pcb[0]?&pcb[1]:&pcb[0]);
  return current->cp;
}
