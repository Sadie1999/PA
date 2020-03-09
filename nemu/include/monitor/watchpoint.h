#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char expr[100];
  char new_val;
  char old_val;
 // struct watchpoint *next;
  /* TODO: Add more members if necessary */


} WP;


int set_watchpoint(char *e);
bool delete_watchpoint(int NO);
void list_watchpoint(void);
WP* scan_watchpoint();
#endif
