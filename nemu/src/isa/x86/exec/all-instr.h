#include "cpu/exec.h"


//arith.c
make_EHelper(sub);
make_EHelper(inv);
make_EHelper(add);
make_EHelper(cmp);
make_EHelper(inc);
make_EHelper(imul2);
make_EHelper(dec);
make_EHelper(adc);
//cc.c
make_EHelper(setcc);
//control.c
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jmp);
make_EHelper(jcc);
//data-mov.c
make_EHelper(mov);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(lea);
make_EHelper(movzx);
make_EHelper(leave);
//exec.c

//logic.c
make_EHelper(xor);
make_EHelper(and);
make_EHelper(test);
make_EHelper(or);
make_EHelper(sar);
make_EHelper(shl);
make_EHelper(shr);
make_EHelper(not);
//prefix.c
make_EHelper(operand_size);
//special.c
make_EHelper(nemu_trap);
make_EHelper(nop);
//system.c
make_EHelper(lidt);