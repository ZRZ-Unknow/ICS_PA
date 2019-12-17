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
#define PTE_ADDR(pte)    ((uint32_t)(pte) & ~0xfff)
#define OFF(va)          ((uint32_t)(va) & 0xfff)

static inline uint32_t get_size(int32_t fsize,int32_t usize){
  return fsize<usize ? fsize:usize;
}
static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  int fd=fs_open(filename,0,0);
  assert(fd!=-1);
  Elf_Ehdr elf;
  fs_read(fd,(void*)&elf,sizeof(Elf_Ehdr));
  Elf_Phdr phdr[elf.e_phnum];
  for(size_t i=0;i<elf.e_phnum;i++){
    fs_lseek(fd,elf.e_phoff+i*elf.e_phentsize,SEEK_SET);
    fs_read(fd,(void*)&phdr[i],elf.e_phentsize);
    if(phdr[i].p_type==PT_LOAD){
      fs_lseek(fd,phdr[i].p_offset,SEEK_SET);
      void *vaddr = (void *)phdr[i].p_vaddr;
      void *paddr=new_page(1);
      int32_t fsize = phdr[i].p_filesz;
      printf("before map\n");
      _map(&pcb->as, vaddr, paddr, 0);
      printf("after map\n");
      uint32_t wsize = get_size(fsize, PTE_ADDR((uint32_t)vaddr + PGSIZE) - (uint32_t)vaddr);
      fs_read(fd, (void *)(PTE_ADDR(paddr) | OFF(vaddr)), wsize);
      fsize -= wsize;
      vaddr += wsize;
      for (; fsize > 0; fsize -= wsize, vaddr += wsize) {
        assert(((uint32_t)vaddr & 0xfff) == 0);
        paddr = new_page(1);
        _map(&pcb->as, vaddr, paddr, 0);
        wsize = get_size(fsize, PGSIZE);
        fs_read(fd, paddr, wsize);
      }
      fsize = phdr[i].p_memsz - phdr[i].p_filesz;
      if (((uint32_t)vaddr & 0xfff) != 0) {
        wsize = get_size(fsize, PTE_ADDR((uint32_t)vaddr + PGSIZE) - (uint32_t)vaddr);
        memset((void *)(PTE_ADDR(paddr) | OFF(vaddr)), 0, wsize);
        fsize -= wsize;
        vaddr += wsize;
      }
      for (wsize = PGSIZE;fsize > 0; fsize -= wsize, vaddr += wsize) {
        assert(((uint32_t)vaddr & 0xfff) == 0);
        paddr = new_page(1);
        _map(&pcb->as, vaddr, paddr, 0);
        memset(paddr, 0, wsize);
      }
      /*//4.2第一次写的loader
      void *va=(void*)phdr[i].p_paddr;
      void *pa;
      int32_t filesz=phdr[i].p_filesz;
      while(filesz>0){
        pa=new_page(1);
        _map(&pcb->as,va,pa,0);
        fs_read(fd,pa,PGSIZE);
        va+=PGSIZE;
        filesz-=PGSIZE;
      }*/
      //fs_read(fd,(void*)phdr[i].p_vaddr,phdr[i].p_filesz);
      //memset((void*)(phdr[i].p_vaddr+phdr[i].p_filesz),0,phdr[i].p_memsz-phdr[i].p_filesz);
    }
  }
  fs_close(fd);
  printf("%s,%x\n",filename,elf.e_entry);
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
