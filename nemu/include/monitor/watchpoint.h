#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  char ch[64];
  int new_v;
  int old_v;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */


} WP;

#endif
