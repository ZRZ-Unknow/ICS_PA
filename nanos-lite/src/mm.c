#include "memory.h"
#include "proc.h"
#define align(va) (va%PGSIZE==0)?(va):((va/PGSIZE+1)*PGSIZE) 
static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk, intptr_t increment) {
  if (current->max_brk == 0) {
    current->max_brk = brk;
  }
  else{
    if(brk+increment>current->max_brk){
      uintptr_t va=align(current->max_brk);
      while(va<brk+increment){
        _map(&current->as, (void *)va, new_page(1), 0);
        va+=PGSIZE;
      }
      current->max_brk=brk+increment;
    }
  }
  return 0;
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
