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
extern void delete_watchpoint(int no);
extern void view_watchpoint(bool all,int no);
extern int scan_watchpoint();
#endif
