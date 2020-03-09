#include "nemu.h"
#include "monitor/monitor.h"
#include "monitor/watchpoint.h"
//#include "monitor/ui.h"
#include "monitor/expr.h"
 #include <stdlib.h>
 #include <readline/readline.h>
#include <readline/history.h>
void ui_mainloop(int);
/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 10000000

//int init_monitor(int,char *[]);
int nemu_state = NEMU_STOP;

void exec_wrapper(bool);
//WP *scanf_watchpoint();
/* Simulate how the CPU works. */
void cpu_exec(uint64_t n) {
  //WP  *now_wp();
  WP *scan_watchpoint();
  //int is_batch_mode = init_monitor(int argc, char *argv[]);
        if (nemu_state == NEMU_END) {
    printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
    return;
  }  
  nemu_state = NEMU_RUNNING;

  bool print_flag = n < MAX_INSTR_TO_PRINT;

   for  (; n > 0; n --) {
       /* Execute one instru ction, including instruction fetch,
     *  instruction decode, and the actual execution. */
    exec_wrapper(print_flag);
  //bool success=true;
#ifdef DEBUG
    /* TODO: check watchpoints here. */
  WP *p=scan_watchpoint();
  
 
 if(p) {nemu_state=NEMU_STOP;
     //    ui_mainloop(0);
 printf("program paused!\n");
 return ;
 } 
  
  
#endif

#ifdef HAS_IOE
    extern void device_update();
    device_update();
#endif

    if (nemu_state != NEMU_RUNNING) { return; }
  
}
  if (nemu_state == NEMU_RUNNING) { nemu_state = NEMU_STOP; }
}