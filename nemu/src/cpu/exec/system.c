#include "cpu/exec.h"

void diff_test_skip_qemu();
void diff_test_skip_nemu();

make_EHelper(lidt) {
//  TODO();
cpu.idtr.limit=vaddr_read(id_dest->addr,2);
if(decoding.is_operand_size_16)
{
    
    cpu.idtr.base=vaddr_read(id_dest->addr+2,4)&0x00ffffff;
}
else
{  // cpu.IDTR.limit=vaddr_read(id_dest->addr,2);
    cpu.idtr.base=vaddr_read(id_dest->addr+2,4);
}


  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
//  TODO();
switch(id_dest->reg){
    case 0:
	cpu.cr0.val=id_src->val;
	break;
    case 3:
	cpu.cr3.val=id_src->val;
        break;
    default:
	assert(0);
	break;
}
  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  //TODO();
switch(id_src->reg)
{
    case 0:
    id_dest->val=cpu.cr0.val;
    break;
    case 3:
    id_dest->val=cpu.cr3.val;
    break;
    default:
    assert(0);
    break;

}
  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}

make_EHelper(int) {
//  TODO();
void raise_intr(uint8_t , vaddr_t);
//t0=cpu.eip+2;
//printf("int eip:%x\n:",decoding.seq_eip); 
raise_intr(id_dest->val,decoding.seq_eip);
  print_asm("int %s", id_dest->str);

#ifdef DIFF_TEST
  diff_test_skip_nemu();
#endif
}

make_EHelper(iret) {
//  TODO();
//printf("seq eip before:%x\n",decoding.seq_eip);
rtl_pop(&t2);
//Log("eip:%d",t2);
//printf("t2:%x\n",t2);
decoding.jmp_eip=t2;
//printf("eip:%x\n",cpu.eip);
//printf("seq eip after:%x\n",decoding.seq_eip);
//decoding.seq_eip=decoding.seq_eip-3;
//cpu.eip=decoding.seq_eip;
//printf("jmp eip:%x\n",decoding.jmp_eip);
rtl_pop((rtlreg_t *)&cpu.cs);
//cpu.CS=t3&0xffff;
rtl_pop(&cpu.eflags.eflags_val);
decoding.is_jmp=1;
print_asm("iret");
}

uint32_t pio_read(ioaddr_t, int);
void pio_write(ioaddr_t, int, uint32_t);

make_EHelper(in) {
//  TODO();
//  t0=id_dest->val&0xffff;
//printf("the id_dest->val in IN:%x the id_src->val:%x the id_dest->width:%x\n",id_dest->val,id_src->val,id_dest->width);
  t0=pio_read(id_src->val,id_dest->width);
operand_write(id_dest,&t0);
//printf("the id_destnow:%x\n",id_dest->val);
  print_asm_template2(in);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}

make_EHelper(out) {
  //TODO();
//  t0=id_dest->val&0xffff;
// printf("OUT:id_dest->val:%x id_dest->width:%x id_src->val:%x id_src->width:%x\n",id_dest->val,id_dest->width,id_src->val,id_src->width);
pio_write(id_dest->val,id_src->width,id_src->val);
//operand_write(id_dest,&id_dest->val);
//Log("out :the id_dest->val now:%x\n",id_dest->val);
  
print_asm_template2(out);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}
