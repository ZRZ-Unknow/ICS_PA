#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  for (int i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;
  head = NULL;
  free_ = wp_pool;
}

bool init=false;

WP* new_wp(){
  if (init==false){
    init_wp_pool();
    init=true;
  }
  if(free_!=NULL){
    WP *p=free_;
    free_=free_->next;
    p->next=head;
    head=p;
    return head;
  }
  else {assert(0);}
}

void in_free_wp(int no){
  if(head==NULL){return;}
  if(head->NO==no){
     WP *p=head;
     head=head->next;
     p->next=free_;
     free_=p;
     return;
  }
  WP *p=head;
  while(p!=NULL){
    	  if(p->NO==no){break;}
     	  p=p->next;
  }
  if (p==NULL){return;}
  if (p->NO==no){
     	  WP *q=head;
     	  while(q->next!=p){
	   	  q=q->next;
     	  }
     	  q->next=p->next;
     	  p->next=free_;
     	  free_=p;
     	  return;
  }
  else {assert(0);}
  return;
}
void free_wp(int no){
  if(head==NULL){printf("no watchpoint to delete\n");return;}
  if(head->NO==no){
    WP *p=head;
    head=head->next;
    p->next=free_;
    free_=p;
    printf("delete watchpoint %d\n",no);
    return;
  }
  WP *p=head;
  while(p!=NULL){
    if(p->NO==no){break;}
    p=p->next;
  }
  if (p==NULL){printf("have no watchpoint %d\n",no);return;}
  if (p->NO==no){
    WP *q=head;
    while(q->next!=p){
      q=q->next;
    }
    q->next=p->next;
    p->next=free_;
    free_=p;
    printf("delete watchpoint %d\n",no);
    return;
  }
  else {assert(0);}
  return;
}

void set_watchpoint(char *args){
  WP *p=new_wp();
  strcpy(p->content,args);
  bool succ=true;
  p->old_v=expr(p->content,&succ);
  if(succ==false){
    in_free_wp(p->NO);
    return;
  }
  printf("successfully set watchpoint %d\n",p->NO);
  printf("expression:%s\n",p->content);
  printf("old value:%#x\n",p->old_v);
}

void view_watchpoint(){
  if(head==NULL){printf("no watchpoint currently\n");}
  else {
    WP *p=head;
    while(p!=NULL){
      printf("watchpoint %d:%s,value is %#x\n",p->NO,p->content,p->old_v);
      p=p->next;
    }
  }
}

WP *no_to_wp(int no){  
  if (no<0){return NULL;}
  else {
    WP *p=head;
    while(p!=NULL){
      if (p->NO==no){return p;}
      p=p->next;
    }
    printf("watchpoint %d not exit\n",no);
    return NULL;
  }
}

int check__watchpoint(){
  WP *p=head;
  while(p!=NULL){
    bool succ=true;
    p->new_v=expr(p->content,&succ);
    if(p->new_v!=p->old_v){
      p->old_v=p->new_v;
      return p->NO;
    }
    p=p->next;
  }
  return -1;
}


/* TODO: Implement the functionality of watchpoint */

