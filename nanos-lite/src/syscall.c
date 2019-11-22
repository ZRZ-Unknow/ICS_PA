#include "common.h"
#include "syscall.h"


static inline int32_t sys_write(int fd,const void *buf,size_t len){
  if(fd==1||fd==2){
    char *b=(char*)buf;
    //printf("--%d\n",len);
    for(int i=0;i<len;i++){
      //printf("----%s\n",b[i]);
      _putc(*(b++));
    }
    return len;
  }
  return -1;
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] =c->GPR2;
  a[2]=c->GPR3;
  a[3]=c->GPR4;
  switch (a[0]) {
    case SYS_yield:_yield();c->GPRx=0;break;
    case SYS_exit:_halt(c->GPR2);break;
    case SYS_write:printf(";;;%d\n",a[3]);c->GPRx=sys_write(a[1],(void*)a[2],a[3]);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}


