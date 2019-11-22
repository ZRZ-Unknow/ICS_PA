#include "proc.h"
#include <elf.h>
#include "fs.h"
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
  int fd=fs_open(filename,0,0);
  printf("fd is %d\n",fd);
  assert(fd!=-1);
  Elf_Ehdr elf;
  fs_read(fd,(void*)&elf,sizeof(Elf_Ehdr));
  Elf_Phdr phdr[elf.e_phnum];
  //fs_read(fd,(void*)&phdr,sizeof(Elf_Phdr)*elf.e_phnum);
  for(size_t i=0;i<elf.e_phnum;i++){
    printf("fd is %d for i is %d\n",fd,i);
    fs_lseek(fd,elf.e_phoff+i*elf.e_phentsize,SEEK_SET);
    fs_read(fd,(void*)&phdr[i],elf.e_phentsize);
    if(phdr[i].p_type==PT_LOAD){
      fs_lseek(fd,phdr[i].p_offset,SEEK_SET);
      fs_read(fd,(void*)phdr[i].p_vaddr,phdr[i].p_filesz);
      memset((void*)(phdr[i].p_vaddr+phdr[i].p_filesz),0,phdr[i].p_memsz-phdr[i].p_filesz);
    }
  }
  fs_close(fd);
  return elf.e_entry;
  /*Elf_Ehdr elf;
  ramdisk_read((void*)&elf,0,sizeof(Elf_Ehdr));
  Elf_Phdr phdr[elf.e_phnum];
  Elf_Phdr *ph=phdr;
  ramdisk_read((void*)ph,elf.e_ehsize,sizeof(Elf_Phdr)*elf.e_phnum);
  for(size_t i=0;i<elf.e_phnum;i++,ph++){
    ...if(phdr[i].p_type==PT_LOAD){
      ramdisk_read((void*)phdr[i].p_vaddr,phdr[i].p_offset,phdr[i].p_filesz);
      memset((void*)(phdr[i].p_vaddr+phdr[i].p_filesz),0,phdr[i].p_memsz-phdr[i].p_filesz);
    }...
    if(ph->p_type==PT_LOAD){
      ramdisk_read((void*)ph->p_vaddr,ph->p_offset,ph->p_filesz);
      memset((void*)(ph->p_vaddr+ph->p_filesz),0,ph->p_memsz-ph->p_filesz);
    }
  }
  return elf.e_entry;*/
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
