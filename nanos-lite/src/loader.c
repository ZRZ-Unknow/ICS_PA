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
  printf("%s\n",filename);
  assert(fd!=-1);
  Elf_Ehdr elf;
  fs_read(fd,(void*)&elf,sizeof(Elf_Ehdr));
  Elf_Phdr phdr[elf.e_phnum];
  for(size_t i=0;i<elf.e_phnum;i++){
    fs_lseek(fd,elf.e_phoff+i*elf.e_phentsize,SEEK_SET);
    fs_read(fd,(void*)&phdr[i],elf.e_phentsize);
    if(phdr[i].p_type==PT_LOAD){
      fs_lseek(fd,phdr[i].p_offset,SEEK_SET);

      //4.2第一次写的loader
      void *va=(void*)phdr[i].p_paddr;
      void *pa;
      int32_t filesz=phdr[i].p_filesz;
      while(filesz>0){
        pa=new_page(1);
        _map(&pcb->as,va,pa,0);
        fs_read(fd,pa,PGSIZE);
        va+=PGSIZE;
        filesz-=PGSIZE;
      }
      //fs_read(fd,(void*)phdr[i].p_vaddr,phdr[i].p_filesz);
      //memset((void*)(phdr[i].p_vaddr+phdr[i].p_filesz),0,phdr[i].p_memsz-phdr[i].p_filesz);
    }
  }
  fs_close(fd);
  return elf.e_entry;
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
  _protect(&pcb->as);
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
