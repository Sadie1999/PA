#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

 // TODO();
 union{
 GateDesc i;
 struct{
 uint32_t low;
 uint32_t high;
 };
}inf; 
//Log("base:%d",cpu.IDTR.base);
inf.low=vaddr_read(NO*8+cpu.idtr.base,4);
inf.high=vaddr_read(NO*8+cpu.idtr.base+4,4);
if(!inf.high>>15&0x1) assert(0);

//Log("123213");
//Log("base:%d",cpu.IDTR.base);
rtl_push(&cpu.eflags.eflags_val);
rtl_push((rtlreg_t *)&cpu.cs);
//Log("ret_addr:%d",ret_addr);
rtl_push(&ret_addr);
decoding.jmp_eip=(inf.i.offset_15_0)|((inf.i.offset_31_16)<<16);
decoding.is_jmp=1;
/*rtl_push((rtlreg_t *)&cpu. eflags);
rtl_push((rtlreg_t *)&cpu.cs);
rtl_push((rtlreg_t *)&ret_addr);
uint32_t idtr_base = cpu.idtr.base;
uint32_t eip_low,eip_high,offset;
eip_low = vaddr_read(idtr_base + NO*8, 4)&0x0000ffff;
eip_high = vaddr_read(idtr_base + NO*8, 4)&0xffff0000;
offset = eip_low | eip_high;
decoding.jmp_eip = offset;
decoding.is_jmp = true;
*/



}
void dev_raise_intr() {
}
