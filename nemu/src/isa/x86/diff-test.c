#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  //printf("debug:ref_r->pc is 0x%08x\n",ref_r->pc);
  if(ref_r->eax!=cpu.eax||
     ref_r->ecx!=cpu.ecx||
     ref_r->edx!=cpu.edx||
     ref_r->ebx!=cpu.ebx||
     ref_r->esp!=cpu.esp||
     ref_r->ebp!=cpu.ebp||
     ref_r->esi!=cpu.esi||
     ref_r->edi!=cpu.edi//||
     //ref_r->pc!=pc
    ){
      printf("qemu:eax is 0x%08x,nemu eax is 0x%08x, at pc:0x%08x\n",ref_r->eax,cpu.eax,pc);
      printf("qemu:ecx is 0x%08x,nemu ecx is 0x%08x, at pc:0x%08x\n",ref_r->ecx,cpu.ecx,pc);
printf("qemu:edx is 0x%08x,nemu edx is 0x%08x, at pc:0x%08x\n",ref_r->edx,cpu.edx,pc);
printf("qemu:ebx is 0x%08x,nemu ebx is 0x%08x, at pc:0x%08x\n",ref_r->ebx,cpu.ebx,pc);
printf("qemu:esp is 0x%08x,nemu esp is 0x%08x, at pc:0x%08x\n",ref_r->esp,cpu.esp,pc);
printf("qemu:ebp is 0x%08x,nemu ebp is 0x%08x, at pc:0x%08x\n",ref_r->ebp,cpu.ebp,pc);
printf("qemu:esi is 0x%08x,nemu esi is 0x%08x, at pc:0x%08x\n",ref_r->esi,cpu.esi,pc);
printf("qemu:edi is 0x%08x,nemu edi is 0x%08x, at pc:0x%08x\n",ref_r->edi,cpu.edi,pc);
printf("qemu:eip is 0x%08x,nemu pc is 0x%08x, at pc:0x%08x\n",ref_r->pc,pc,pc);
    return false;
  }
  if(ref_r->eflags.CF!=cpu.eflags.CF||
     ref_r->eflags.ZF!=cpu.eflags.ZF||
     ref_r->eflags.SF!=cpu.eflags.SF||
     ref_r->eflags.IF!=cpu.eflags.IF||
     ref_r->eflags.OF!=cpu.eflags.OF
  ){
    printf("qemu's eflags!=nemu's eflags\n");
    return false;
  }
  return true;
}

void isa_difftest_attach(void) {
}
