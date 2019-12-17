#include "nemu.h"
#include "memory.h"
#define PDXSHFT 22
#define PTXSHFT 12
#define PDX(va)          (((uint32_t)(va) >> PDXSHFT) & 0x3ff)
#define PTX(va)          (((uint32_t)(va) >> PTXSHFT) & 0x3ff)
#define OFF(va)          ((uint32_t)(va) & 0xfff)
#define PTE_ADDR(pte)    ((uint32_t)(pte) & ~0xfff)

paddr_t page_translate(vaddr_t ad){
  PDE pde;
  PTE pte;  
  pde.val=paddr_read(cpu.cr3.val+4*PDX(ad),4);
  if(pde.present==0) assert(0);

  pte.val=paddr_read(PTE_ADDR(pde.val)+4*PTX(ad),4);
  if(pte.present==0) assert(0);

  return PTE_ADDR(pte.val) | OFF(ad);
}


uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  if(cpu.cr0.paging==1){
    if( (addr&0xfff) +len-1 <= 0xfff){
      return paddr_read(page_translate(addr),len);
    }
    //跨页
    else{
      Log("read across page\n");
      assert(0);
      return 0;
    }
  }
  else{
    return paddr_read(addr, len);
  }
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if(cpu.cr0.paging==1){
    if((addr&0xfff) +len-1 <= 0xfff){
      paddr_write(page_translate(addr),data,len);
    }
    else{
      printf("write across page\n");
      assert(0);
    }
  }
  else{
    paddr_write(addr, data,len);
  }
}
