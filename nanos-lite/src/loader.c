#include "proc.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();  
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);
static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  Elf_Ehdr *elf;
  Elf_Phdr *phdr;
  uint8_t buf[get_ramdisk_size()];
  ramdisk_read(buf,0,get_ramdisk_size());
  elf=(void*)buf;
  phdr=(void*)(buf+elf->e_phoff);
  for(int i=0;i<elf->e_phnum;i++,phdr++){
    //phdr=(void*)buf+elf->e_phoff+i*elf->e_phentsize;
    printf("ddd\n");
    if(phdr->p_type==PT_LOAD){
      //phdr->p_vaddr=malloc();
      ramdisk_read((void*)phdr->p_vaddr,phdr->p_offset,phdr->p_filesz);
      memset((void*)(phdr->p_vaddr+phdr->p_filesz),0,phdr->p_memsz-phdr->p_filesz);
    }
  }
  return (uintptr_t)elf->e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
