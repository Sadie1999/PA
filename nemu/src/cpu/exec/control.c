#include "cpu/exec.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  decoding.is_jmp = 1;

  print_asm("jmp %x", decoding.jmp_eip);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint8_t subcode = decoding.opcode & 0xf;
  rtl_setcc(&t2, subcode);
  decoding.is_jmp = t2;
//printf("decoding.is_jmp in makeEHelper(jcc):%x\n",decoding.is_jmp);
  print_asm("j%s %x", get_cc_name(subcode), decoding.jmp_eip);
}

make_EHelper(jmp_rm) {
  decoding.jmp_eip = id_dest->val;
  decoding.is_jmp = 1;

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // the target address is calculated at the decode stage
//  TODO();
rtl_li(&t2,decoding.seq_eip);
rtl_push(&t2);
//Log("call:%d",t2);
decoding.is_jmp=1;
  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
//  TODO();
rtl_pop(&t0);
decoding.jmp_eip=t0;

//Log("eip_ret:%d",t0);
 //rtl_pop(&t1);
//rtl_pop(&decoding.seq_eip);
//rtl_pop(&t1);
decoding.is_jmp=1;
//printf("the eax in ret is:%x\n",cpu.eax);
//printf("the ecx in ret is:%x\n",cpu.ecx);
//printf("the edx in ret is:%x\n",cpu.edx);
//printf("the ebx in ret is:%x\n",cpu.ebx);
//printf("the esp in ret is:%x\n",cpu.esp);
//printf("the ebp in ret is:%x\n",cpu.ebp);
//printf("the esi in ret is:%x\n",cpu.esi);
//printf("the edi in ret is:%x\n",cpu.edi);
  print_asm("ret");
}

make_EHelper(call_rm) {
//  TODO();
 rtl_li(&t2,decoding.seq_eip);
 rtl_push(&t2);
 
//Log("call_rm:%d",t2);
//printf("the id_dest->val:%x\n",id_dest->val);
  //printf("the id_dest->str:%s\n",id_dest->str);
 decoding.jmp_eip=id_dest->val;
decoding.is_jmp=1; 
 print_asm("call *%s", id_dest->str);
}
