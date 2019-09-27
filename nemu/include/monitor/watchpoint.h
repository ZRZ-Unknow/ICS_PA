#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  char content[64];
  int new_v;
  int old_v;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */


} WP;
extern void set_watchpoint(char *args);
extern void free_wp(int no);
extern void view_watchpoint();
extern int scan_watchpoint();
extern WP *no_to_wp(int no);
#endif
