#include "nemu.h"
#include "memory.h"
#define PDXSHFT 22
#define PTXSHFT 12
#define PDX(va)          (((uint32_t)(va) >> PDXSHFT) & 0x3ff)
#define PTX(va)          (((uint32_t)(va) >> PTXSHFT) & 0x3ff)
#define OFF(va)          ((uint32_t)(va) & 0xfff)
#define PTE_ADDR(pte)    ((uint32_t)(pte) & ~0xfff)
#define PTE_P          0x001 
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
      printf("succ read\n");
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
      printf("succ write\n");
      paddr_write(page_translate(addr),data,len);
    }
    else{
      Log("write across page\n");
      assert(0);
    }
  }
  else{
    paddr_write(addr, data,len);
  }
}
/*
inline paddr_t page_translate(vaddr_t va) {
  paddr_t ptab = paddr_read(cpu.cr3.val + sizeof(PDE) * PDX(va), sizeof(PDE));
  if (!(ptab & PTE_P)) {
    printf("ERROR:page_translate(): page table doesn't exists!\n");
    assert(0); 
  }

  paddr_t pg = paddr_read(PTE_ADDR(ptab) + sizeof(PTE) * PTX(va), sizeof(PTE));
  if (!(pg & PTE_P)) {
    printf("ERROR:page_translate(): page doesn't exists!\n");
    assert(0); 
  }

  return (PTE_ADDR(pg) | OFF(va));
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  if (cpu.cr0.paging) { 
    if (PTE_ADDR(addr) != PTE_ADDR(addr + len - 1)) { 
      uint8_t byte[4];
      for (int i = 0; i < len; i++)
        byte[i] = isa_vaddr_read(addr + i, 1);
      if (len == 2)
        return *(uint16_t *)byte;
      else
        return *(uint32_t *)byte;
    } else {
      paddr_t paddr = page_translate(addr);
      return paddr_read(paddr, len);
    }
  } else {
    return paddr_read(addr, len);
  }
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if (cpu.cr0.paging) { 
    if (PTE_ADDR(addr) != PTE_ADDR(addr + len - 1)) { 
      uint8_t byte[4];
      if (len == 2)
        *(uint16_t *)byte = data;
      else
        *(uint32_t *)byte = data;
      for (int i = 0; i < len; i++)
        isa_vaddr_write(addr + i, byte[i], 1);
    } else {  
      paddr_t paddr = page_translate(addr);
      paddr_write(paddr, data, len);
    }
  } else {  
    paddr_write(addr, data, len);
  }
}*/
