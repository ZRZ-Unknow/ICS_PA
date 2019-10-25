#include "cpu/exec.h"


//arith.c
make_EHelper(sub);
make_EHelper(inv);
make_EHelper(add);
make_EHelper(cmp);
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
make_EHelper(leava);
//exec.c

//logic.c
make_EHelper(xor);
make_EHelper(and);
make_EHelper(test);
//prefix.c
make_EHelper(operand_size);
//special.c
make_EHelper(nemu_trap);
//system.c