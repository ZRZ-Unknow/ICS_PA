#include "cpu/exec.h"
#include "cc.h"

make_EHelper(test) {
  //TODO();
  rtl_and(&s1,&id_dest->val,&id_src->val);
  s0=0;
  rtl_set_CF(&s0);
  rtl_set_OF(&s0);
  rtl_update_ZFSF(&s1,id_dest->width);
  print_asm_template2(test);
}

make_EHelper(and) {
  rtl_and(&s1,&id_dest->val,&id_src->val);
  s0=0;
  rtl_set_OF(&s0);
  rtl_set_CF(&s0);
  rtl_update_ZFSF(&s1,id_dest->width);
  operand_write(id_dest,&s1);

  print_asm_template2(and);
}

make_EHelper(xor) {
  //TODO();
  rtl_xor(&s1,&id_dest->val,&id_src->val);
  s0=0;
  rtl_set_OF(&s0);
  rtl_set_CF(&s0);
  rtl_update_ZFSF(&s1,id_dest->width);
  operand_write(id_dest,&s1);

  print_asm_template2(xor);
}

make_EHelper(or) {
  TODO();

  print_asm_template2(or);
}

make_EHelper(sar) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decinfo.opcode & 0xf;

  rtl_setcc(&s0, cc);
  operand_write(id_dest, &s0);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  TODO();

  print_asm_template1(not);
}
