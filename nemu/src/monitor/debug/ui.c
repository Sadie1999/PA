#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
   }

  line_read = readline("(nemu) ");

   if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_x(char *args);
static int cmd_p(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);
static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  {"si","si execution success",cmd_si},
  {"info","Info success",cmd_info},
  {"x","x success",cmd_x},
  {"p","evaluate expr",cmd_p},
  {"w","watchpoint expr",cmd_w},
  {"d","watchpoint number",cmd_d},
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}
static int cmd_si(char *args){
   char *arg=strtok(args," ");
   if(arg==NULL)
   {cpu_exec(1);
       return 0;
  }

   int n;//int i=0;
   sscanf(arg,"%d",&n);
   if(n<0)
   {cpu_exec(-1);
   } 

   else if(n>0)
   {//for(i=0;i<n;i++)
       cpu_exec(n);
   }
   return 0;
}
static int cmd_info(char *args)
{ //WP *now_wp();
    void list_watchpoint();
char *arg=strtok(args," ");
int i=0;
 if(*arg=='r')
 { for(i=0;i<8;i++)
     {printf("%s:\t0x%08x\t%d\n",regsl[i],cpu.gpr[i]._32,cpu.gpr[i]._32);
	//printf("%d\n",cpu.eip); 
      }
     printf("eip:\t0x%08x\t%d\n",cpu.eip,cpu.eip);

 }
 else if(*arg=='w')  
{/*printf("NO  Expr   Old  Value\n");
 WP *p=now_wp();   
 while(p)
 {printf("%d  %s    0x%x\ n",p->NO,p->expr,p->old_val);
     p=p->next;
 }
 }*/
list_watchpoint();}
 return 0;
}
static int cmd_x(char *args)
{
    char *arg=strtok(args," ");
   // char *str;
    int time;
    int  address;
    int  number;
    sscanf(arg,"%d",&time);
    arg=strtok(NULL," ");
    sscanf(arg,"%x",&address);
   // printf("%d\t%08x",address,address);
    int i=0;
    printf("Address \tDworld block\tByte sequence\n");
    for(i=0;i<time;i++)
    {//printf("0x%08x\t",address);
     unsigned char str[4];
    // memcpy(str,&number,sizeof(int));
     number=vaddr_read(address,4);
     printf("0x%08x\t0x%08x\t",address,number);
    memcpy(str,&number,sizeof(int));
     for(int j=0;j<4;j++)
     {printf("%02x ",str[j]);}
     printf("\n");
     address=address+4; 
    } return 0;
}
static int cmd_p(char *args)
{//char *arg=strtok(args," ");
    bool success=true;
//    flag_success=make_token(args);

    printf("%d\n" , expr(args,&success));
 return 0;
}

static int cmd_w(char *args)
{  //：WP *new_wp();
    int set_watchpoint(char*);
//    bool success=true;
 /*   WP *p=new_wp();
    p->old_val=expr(args,&success);
    strcpy(p->expr,args);
    printf("Set watchpoint #%d\n",p->NO);
    printf("exp   =%s\n", p->expr);
   printf("old value =0x%x\n",p->old_val);*/
   //////int set_watchpoint()////////////////////////
   printf("--------following is the set_watchpoint work:-------------\n");
   printf("\nthe number is:%d\n",set_watchpoint(args));

return 0;
}
static int cmd_d(char *args)
{ // WP *now_wp();
   //void free_wp(WP *);
    bool delete_watchpoint(int);
    int x;
    sscanf(args,"%d",&x);
    /*WP *p=now_wp();
    while(p)
    {if(p->NO==x) break;
	else p=p->next;
    }
    free_wp(p);
    printf("Watchpoint %d deleted\n",x);*/
    delete_watchpoint(x);
return 0;
}
void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
