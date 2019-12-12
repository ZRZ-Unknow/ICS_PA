#include "nemu.h"
#include "memory.h"
paddr_t page_translate(vaddr_t ad){
  TODO();
  return 0;
}


uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  if(cpu.cr0.paging==1){
    if(0){
      assert(0);
    }
    else{
      paddr_t paddr=page_translate(addr);
      return paddr_read(paddr,len);
    }
  }
  else{
    return paddr_read(addr, len);
  }
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if(cpu.cr0.paging==1){
    if(0){
      assert(0);
    }
    else{
      paddr_t paddr=page_translate(addr);
      paddr_write(paddr,data,len);
    }
  }
  else{
    paddr_write(addr, data,len);
  }
}
