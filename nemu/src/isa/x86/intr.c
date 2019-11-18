#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  uint32_t hi,lo;
  assert(NO<=cpu.IDTR.limit);
  rtl_push(&cpu.eflags.flags);
  rtl_push(&cpu.CS);
  rtl_push(&ret_addr);
  lo=vaddr_read(cpu.IDTR.base+8*NO,4) & 0x0000ffff;
  hi=vaddr_read(cpu.IDTR.base+8*NO+4,4) & 0xffff0000;
  rtl_j(hi|lo);
}

bool isa_query_intr(void) {
  return false;
}
