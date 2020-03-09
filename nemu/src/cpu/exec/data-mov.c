#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}
make_EHelper(rol){
 t0=id_src->val;
 while(t0!=0)
 { 
  rtl_msb(&t1,&id_dest->val,id_dest->width);
  id_dest->val=id_dest->val*2+t1;
  t0=t0-1;
 }
if(id_src->val==1)
    {
	rtl_msb(&t2,&id_dest->val,id_dest->width);
	rtl_get_CF(&t3);
	if(t2!=t3)
	{t1=0;
	 rtl_set_OF(&t1);
	}
	else{
	t1=1;
	rtl_set_OF(&t1);
	}

    }
}


make_EHelper(push) {
//  TODO();
rtl_push(&id_dest->val);
  print_asm_template1(push);
}

make_EHelper(pop) {
//  TODO();
rtl_pop(&t0);
//printf("the id_src->valis%x\n",t0);
operand_write(id_dest,&t0);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
//  TODO();
/* if(decoding.is_operand_size_16)
 {
t2=cpu.esp&0xffff;
 t1=cpu.eax&0xffff;
 rtl_push(&t1);
 t1=cpu.ecx&0xffff;
 rtl_push(&t1);
 t1=cpu.edx&0xffff;
 rtl_push(&t1);
 t1=cpu.ebx&0xffff;
 rtl_push(&t1);
 rtl_push(&t2);
 t1=cpu.ebp&0xffff;
 rtl_push(&t1);
 t1=cpu.esi&0xffff;
 rtl_push(&t1);
 t1=cpu.edi&0xffff;
 rtl_push(&t1);
 }*/
 //else {
 t2=cpu.esp;
     rtl_push(&cpu.eax);
 rtl_push(&cpu.ecx);
 rtl_push(&cpu.edx);
 rtl_push(&cpu.ebx);
 rtl_push(&t2);
 //printf("pusha ebx:%x\n",cpu.ebx);
 rtl_push(&cpu.ebp);
 rtl_push(&cpu.esi);
 rtl_push(&cpu.edi);
 
 //}  
   
     print_asm("pusha");
}

make_EHelper(popa) {
//  TODO();
/*if(decoding.is_operand_size_16)
{   rtl_pop(&t1);
    cpu.edi=t1&0x0000ffff;
    rtl_pop(&t1);
    cpu.esi=t1&0x0000ffff;
    rtl_pop(&t1);
    cpu.ebp=t1&0x0000ffff; 
    rtl_pop(&t2);
    rtl_pop(&t1);
    cpu.ebx=t1&0x0000ffff;
    rtl_pop(&t1);
    cpu.edx=t1&0x0000ffff;
    rtl_pop(&t1);
    cpu.ecx=t1&0x0000ffff;
    rtl_pop(&t1);
    cpu.eax=t1&0x0000ffff;
}*/
//else{
    rtl_pop(&cpu.edi);
    rtl_pop(&cpu.esi);
    rtl_pop(&cpu.ebp);
    rtl_pop(&t2);

  //  printf("before:%x ",cpu.ebx);
    rtl_pop(&cpu.ebx);
    //printf("after:%x\n",cpu.ebx);
    rtl_pop(&cpu.edx);
    rtl_pop(&cpu.ecx);
    rtl_pop(&cpu.eax);

//} 
  print_asm("popa");
}

make_EHelper(leave) {
//  TODO();
  cpu.esp=cpu.ebp;
  rtl_pop(&t0);
  cpu.ebp=t0;

  print_asm("leave");
}

make_EHelper(cltd) {
   if (decoding.is_operand_size_16) {
    //TODO();
  //  t0=cpu.eax&0xffff;
 //   rtl_sext(&cpu.eax,&t0,2);
  //  rtl_msb(&t1,&cpu.eax,2);
 //   if(t1==1) cpu.edx=0xffff;
   // else cpu.edx=0; 
 rtl_lr_w(&t0,R_AX);
 rtl_sext(&t0,&t0,2);
 rtl_sari(&t0,&t0,16);
 rtl_sr_w(R_DX,&t0);
  }
  else {
    //TODO();
 //   t0=cpu.eax;
 //   rtl_sext(&cpu.eax,&t0,4);
   // rtl_msb(&t1,&cpu.eax,4);
   // if(t1==1) cpu.edx=0xffffffff;
   // else cpu.edx=0;
  rtl_lr_l(&t0,R_EAX);
      rtl_sari(&t0,&t0,31);
          rtl_sari(&t0,&t0,1);
	      rtl_sr_l(R_EDX,&t0); 
  
  } 

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    //TODO();
  //  t0=cpu.eax&0xff;
  //  rtl_sext(&cpu.eax,&t0,2);
    

       rtl_lr_b(&t0,R_AX);
       rtl_sext(&t0,&t0,1);
       rtl_sr_w(R_AX,&t0);
  }
  else {
    //TODO();
  //  t0=cpu.eax&0xffff;
   // rtl_sext(&cpu.eax,&t0,4);
    rtl_lr_w(&t0,R_AX);
        rtl_sext(&t0,&t0,2);
	    rtl_sr_l(R_EAX,&t0);
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t2, &id_src->val, id_src->width);
  operand_write(id_dest, &t2);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  rtl_li(&t2, id_src->addr);
  operand_write(id_dest, &t2);
  print_asm_template2(lea);
}
