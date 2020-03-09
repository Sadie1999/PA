#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include "nemu.h"
#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

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

/* TODO: Implement the functionality of watchpoint */
WP *new_wp()
{ WP *p;
//WP *q;
//q=head;
//    bool success=true;
  if(free_==NULL) {printf("There is no free watchpoint\n");assert(0);}
//if(head==NULL) 
p=free_->next;
free_->next=head;
  head=free_;
  free_=p;

  
  //strcpy(head->expr,args);
//  head->old_val=expr(args,&success);
  return head;
}

void free_wp(WP *wp)
{WP *p=head;
    if(p==wp)
     {p=head->next;
	head->next=free_;
	free_=head;
	head=p;
    }
     else{
      while(p)
    {  if(p->next==wp)
	{p->next=wp->next;
	    wp->next=free_;
	    free_=wp;
	    break;
 	}
	else {p=p->next;}
     }
    
     }

}
WP *now_wp()
{//if(head)
    return head;
    //else return NULL;
}

int set_watchpoint(char *e)
{  WP *now_wp();
   bool success=true;
  WP *p=new_wp();
  p->old_val=expr(e,&success);
 strcpy(p->expr,e);
 printf("Set Watchpoint #%d\n",p->NO);
 printf("exp=%s\n",p->expr);
printf("old value=0c%x\n",p->old_val);
return p->NO;
}

bool delete_watchpoint(int NO)
{ WP *now_wp();
    bool result=false;
    void free_wp(WP *);
    WP *p=now_wp();
    while(p)
    {if(p->NO==NO) {result=true;break;}
	else p=p->next;
    }
free_wp(p);
printf("Watchpoint %d delete\n",NO);
return result;
}
void list_watchpoint()
{  WP *now_wp();
    printf("NO Expr  Old Value\n");
    WP *p=now_wp();
    while(p)
    {printf("%d %s	0x%x\n",p->NO,p->expr,p->old_val);
    p=p->next;
    }
}
WP *scan_watchpoint()
{   WP *now_wp();
    WP *p=now_wp();
    bool success=true;
    while(p)
    { p->new_val=expr(p->expr,&success);
	if(p->new_val!=p->old_val)
	{printf("Hit watchpoint %d at a ddress 0x%x\n",p->NO,cpu.eip);
	    printf("expr=%s\n",p->expr);
	    printf("old value=0x%x\n",p->old_val);
	    printf("new value=0x%x\n",p->new_val);
	    printf("program paused\n");
	    p->old_val=p->new_val;
	return p;
	}
	p=p->next;

     }
return p;
}
