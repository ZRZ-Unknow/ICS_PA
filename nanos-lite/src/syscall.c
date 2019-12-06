#include "common.h"
#include "syscall.h"
#include "fs.h"
#include "proc.h"
extern void naive_uload(PCB *pcb, const char *filename);
/*static inline int32_t sys_write(int fd,const void *buf,size_t len){
  if(fd==1||fd==2){
    char *b=(char*)buf;
    printf("len is %d\n",len);
    for(int i=0;i<len;i++){
      _putc(*(b++));
    }
    return len;
  }
  return -1;
}*/

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] =c->GPR2;
  a[2]=c->GPR3;
  a[3]=c->GPR4;
  switch (a[0]) {
    case SYS_yield:Log("sys_yield");_yield();c->GPRx=0;break;
    case SYS_exit:Log("sys_exit");c->GPRx=-1;naive_uload(NULL,"/bin/init");break;//_halt(a[1]);break;
    case SYS_brk:Log("sys_brk");c->GPRx=0;break;
    case SYS_open:Log("sys_open");c->GPRx=fs_open((void*)a[1],a[2],a[3]);break;
    case SYS_read:Log("read");c->GPRx=fs_read(a[1],(void*)a[2],a[3]);break;
    case SYS_write:Log("sys_write");c->GPRx=fs_write(a[1],(void*)a[2],a[3]);break;
    case SYS_close:Log("sys_close");c->GPRx=fs_close(a[1]);break;
    case SYS_lseek:Log("sys_lseek");c->GPRx=fs_lseek(a[1],a[2],a[3]);break;
    case SYS_execve:Log("sys_execve");c->GPRx=-1;naive_uload(NULL,(char*)a[1]);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return c;
}


