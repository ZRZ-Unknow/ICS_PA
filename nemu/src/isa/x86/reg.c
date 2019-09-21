#include "nemu.h"
#include <stdlib.h>
#include <time.h>

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
  srand(time(0));
  uint32_t sample[8];
  uint32_t pc_sample = rand();
  cpu.pc = pc_sample;

  int i;
  for (i = R_EAX; i <= R_EDI; i ++) {
    sample[i] = rand();
    reg_l(i) = sample[i];
    assert(reg_w(i) == (sample[i] & 0xffff));
  }

  assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
  assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
  assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
  assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
  assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
  assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
  assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
  assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

  assert(sample[R_EAX] == cpu.eax);
  assert(sample[R_ECX] == cpu.ecx);
  assert(sample[R_EDX] == cpu.edx);
  assert(sample[R_EBX] == cpu.ebx);
  assert(sample[R_ESP] == cpu.esp);
  assert(sample[R_EBP] == cpu.ebp);
  assert(sample[R_ESI] == cpu.esi);
  assert(sample[R_EDI] == cpu.edi);

  assert(pc_sample == cpu.pc);
}

void isa_reg_display() {
  printf("eax  0x%08x\n",cpu.eax);
  printf("ecx  0x%08x\n",cpu.ecx);
  printf("edx  0x%08x\n",cpu.edx);
  printf("ebx  0x%08x\n",cpu.ebx);
  printf("esp  0x%08x\n",cpu.esp);
  printf("ebp  0x%08x\n",cpu.ebp);
  printf("esi  0x%08x\n",cpu.esi);
  printf("edi  0x%08x\n",cpu.edi);
  for (int i=0;i<8;i++){
    printf("%s   0x%x\n",regsw[i],cpu.gpr[i]._16);
  }
  for (int i=0;i<8;i++){
    printf("%s  ",regsb[i]);
    for (int j=0;j<2;j++){
      printf(" 0x%x",cpu.gpr[i]._8[j]);
    }
    printf("\n");
  }
}
uint32_t isa_reg_str2val(const char *s, bool *success) {
    if (strcmp(s,"eax")==0){return cpu.eax;}
    if (strcmp(s,"ecx")==0){return cpu.ecx;}
    if (strcmp(s,"edx")==0){return cpu.edx;}
    if (strcmp(s,"ebx")==0){return cpu.ebx;}
    if (strcmp(s,"esp")==0){return cpu.esp;}
    if (strcmp(s,"ebp")==0){return cpu.ebp;}
    if (strcmp(s,"esi")==0){return cpu.esi;}
    if (strcmp(s,"edi")==0){return cpu.edi;}
    if (strcmp(s,"ax")==0){return cpu.gpr[0]._16;}
    if (strcmp(s,"cx")==0){return cpu.gpr[1]._16;}
    if (strcmp(s,"dx")==0){return cpu.gpr[2]._16;}
    if (strcmp(s,"bx")==0){return cpu.gpr[3]._16;}
    if (strcmp(s,"sp")==0){return cpu.gpr[4]._16;}
    if (strcmp(s,"bp")==0){return cpu.gpr[5]._16;}
    if (strcmp(s,"si")==0){return cpu.gpr[6]._16;}
    if (strcmp(s,"di")==0){return cpu.gpr[7]._16;}
    if (strcmp(s,"al")==0){return cpu.gpr[0]._8[0];}
    if (strcmp(s,"cl")==0){return cpu.gpr[1]._8[0];}
    if (strcmp(s,"dl")==0){return cpu.gpr[2]._8[0];}
    if (strcmp(s,"bl")==0){return cpu.gpr[3]._8[0];}
    if (strcmp(s,"ah")==0){return cpu.gpr[4]._8[0];}
    if (strcmp(s,"ch")==0){return cpu.gpr[5]._8[0];}
    if (strcmp(s,"dh")==0){return cpu.gpr[6]._8[0];}
    if (strcmp(s,"bh")==0){return cpu.gpr[7]._8[0];}
  return 0;
}
