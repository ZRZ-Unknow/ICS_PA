#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  rtl_push(&id_dest->val);
  print_asm_template1(push);
}

make_EHelper(pop) {
  //TODO();
  rtl_pop(&id_src->val);
  operand_write(id_dest,&id_src->val);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  //TODO();
  rtl_mv(&cpu.esp,&cpu.ebp);
  rtl_pop(&cpu.ebp);
  print_asm("leave");
}

make_EHelper(cltd) {
  if (decinfo.isa.is_operand_size_16) {
    //TODO();
    rtl_sext(&s0,&reg_l(R_EAX),2);
    rtl_shri(&reg_l(R_EDX),&s0,16);
  }
  else {
    //TODO();
    rtl_sari(&reg_l(R_EDX),&reg_l(R_EAX),31);
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decinfo.isa.is_operand_size_16) {
    //TODO();
    rtl_shli(&reg_l(R_EAX),&reg_l(R_EAX),24);
    rtl_sari(&reg_l(R_EAX),&reg_l(R_EAX),8);
    rtl_shri(&reg_l(R_EAX),&reg_l(R_EAX),16);
  }
  else {
    //TODO();
    rtl_sext(&reg_l(R_EAX),&reg_l(R_EAX),2);
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(&s0, &id_src->val, id_src->width);
  operand_write(id_dest, &s0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(movsb){
  /*printf("dddddddd\n");
  id_dest->width=decinfo.isa.is_operand_size_16?2:4;
  rtl_sext(&s0,&id_src->val,id_src->width);
  operand_write(id_dest,&s0);*/
  //int in=cpu.eflags.DF?-1:1;
  int in=-1;
  rtl_lr(&s0,R_ESI,4);
  rtl_lm(&s1,&s0,1);
  s0+=in;
  rtl_sr(R_ESI,&s0,4);
  rtl_lr(&s0,R_EDI,4);
  rtl_sm(&s0,&s1,1);
  s0+=in;
  rtl_sr(R_EDI,&s0,4);
  print_asm_template2(movsb);
}
make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
