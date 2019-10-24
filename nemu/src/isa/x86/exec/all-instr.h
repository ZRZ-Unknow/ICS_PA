#include "cpu/exec.h"


//arith.c
make_EHelper(sub);
make_EHelper(inv);

//cc.c

//control.c
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jmp);

//data-mov.c
make_EHelper(mov);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(lea);
//exec.c

//logic.c
make_EHelper(xor);
make_EHelper(and);
//prefix.c
make_EHelper(operand_size);
//special.c
make_EHelper(nemu_trap);
//system.c