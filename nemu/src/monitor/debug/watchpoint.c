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

int free_wp(int no){
  WP *p=head;
  if(p==NULL){printf("no watchpoint");return 0;}
  if(p->NO==no){
    head=head->next;
    p->next=free_;
    free_=p;
  }
  else{
    WP *pre;
    while(p->NO!=no){
      pre=p;
      p=p->next;
    }
    pre->next=p->next;
    p->next=free_;
    free_=p;
  }
  printf("delete watchpoint %d\n",no);
  return 0;
}

void set_watchpoint(char *args){
  WP *p=new_wp();
  printf("successfully set watchpoint %d",p->NO);
  strcpy(p->content,args);
  bool succ=true;
  p->old_v=expr(p->content,&succ);
  printf("old value=%d",p->old_v);
}

void delete_watchpoint(int no){
  free_wp(no);
}

void view_watchpoint(bool all,int no){
  if (all==true){
    WP *p=head;
    if(p==NULL){printf("no watchpoint currently");}
    else {
      while(p!=NULL){
	printf("watchpoint %d:%s,value is %#x",p->NO,p->content,p->old_v);
        p=p->next;
      }
    }
  }
  else {
    WP *p=head;
    if(p==NULL){printf("no watchpoint currently");}
    else {
      while(p->NO!=no && p!=NULL){
        p=p->next;
      }
      if(p!=NULL && p->NO==no){
        printf("watchpoint %d:%s,value is %#x",p->NO,p->content,p->old_v);
      }
      else {printf("not find watchpoint %d",no);}
    }
  }
}

WP *scan_watchpoint(){
  WP *p=head;
  if(p==NULL){printf("no watchpoint");return false;}
  while(p!=NULL){
    bool succ=true;
    p->new_v=expr(p->content,&succ);
    if(p->new_v!=p->old_v){
      return p;
    }
  }
  return 0;
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
