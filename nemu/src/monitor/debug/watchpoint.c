#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;
bool init=false;
void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

WP* new_wp(){
  if (init==false){init_wp_pool();}
  if(free_!=NULL){
    WP *p=free_;
    free_=free_->next;
    p->next=head;
    head=p;
    init=true;
    return head;
  }
  else {assert(0);}
}

void free_wp(WP *wp){
  WP *p=head;
  if(p==NULL){assert(0);}
  if(p==wp){
    head=head->next;
    p->next=free_;
    free_=p;
    printf("delete watchpoint\n");
    return;
  }
  else{
    WP *pre;
    while(p!=wp){
      pre=p;
      p=p->next;
    }
    pre->next=p->next;
    wp->next=free_;
    free_=wp;
    printf("delete watchpoint\n");
    return;
  }
  return;
}






/* TODO: Implement the functionality of watchpoint */

/*
typedef struct watchpoint {
  int NO;
  char ch[64];
  int new_v;
  int old_v;
  struct watchpoint *next;
} WP;
*/
