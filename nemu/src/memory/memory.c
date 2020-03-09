#include "nemu.h"
#include "device/mmio.h"
#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
     })

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
    int mmio_id;
    int data;
    mmio_id=is_mmio(addr);
   if(mmio_id!=-1){
   data=mmio_read(addr,len,mmio_id);
   return data;
   } 
    return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
}

void paddr_write(paddr_t addr, int len, uint32_t data) {
   int mmio_id;
   mmio_id=is_mmio(addr);
   if(mmio_id!=-1){
   mmio_write(addr,len,data,mmio_id);
 //  return NULL;
   }
   else memcpy(guest_to_host(addr), &data, len);
}

uint32_t vaddr_read(vaddr_t addr, int len) {
uint32_t page_translate(vaddr_t,bool);
    if(cpu.cr0.paging){
if(((addr&0xfff)+len)>4096){
   paddr_t paddr,low,high;
   int x;
   x=(int)((addr&0xfff)+len-4096);
paddr=page_translate(addr,false);
low=paddr_read(paddr,len-x);
paddr=page_translate(addr+len-x,false);
high=paddr_read(paddr,x);
paddr=(high<<((len-x)<<3))+low;
return paddr;
} 
else  {
paddr_t paddr=page_translate(addr,false);
return paddr_read(paddr,len);
}  
}  
else
    return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, uint32_t data) {
 uint32_t page_translate(vaddr_t,bool);
     if(cpu.cr0.paging){ 
  if(((addr&0xfff)+len)>4096){ 
  //assert(0);
  paddr_t paddr,low,high;
  int x;
  x=(int)((addr&0xfff)+len-0x1000);
  low=(data<<(x<<3))>>(x>>3);
  high=data>>((len-x)<<3);
  paddr=page_translate(addr,true);
  paddr_write(paddr,len-x,low);

  paddr=page_translate(addr+len-x,true);
  paddr_write(paddr,x,high);


  }   
  else{
  paddr_t paddr=page_translate(addr,true);
  paddr_write(paddr,len,data);
  }    
   }  
  else
    paddr_write(addr, len, data);
}

uint32_t page_translate(vaddr_t addr,bool iswrite)
{
  PDE pde;
  PTE pte;
 // Log("addr:0x%x",addr);
//  pde.val=cpu.cr3.val;
  paddr_t base1=cpu.cr3.val;
  //Log("pagefram:0x%x",pde_base);
  paddr_t pde_address=base1+((addr>>22)<<2);
//  Log("pde_address:0x%x",pde_address);
  pde.val=paddr_read(pde_address,4);
  //Log("pde.val:0x%x",pde.val);
  if(!pde.present) 
  assert(0);
   
  paddr_t base2=pde.val&0xfffff000;
  paddr_t pte_address=base2+((addr&0x003ff000)>>10);
  //Log("pte_address 0x%x",pte_address);
  pte.val=paddr_read(pte_address,4);
 // Log("pte.val:0x%x",pte.val);
  if(!pte.present)
  assert(0); 
      
  paddr_t paddress=(pte.val&0xfffff000)+(addr&0xfff);

//检验access
   if (!pde.accessed){
  pde.accessed=1;
  paddr_write(pde_address,4,pde.val);
  }   
  if((!pte.accessed)||((!pte.dirty)&&iswrite))
  //if(!pte.accessed)
   { 
  pte.accessed=1;
  pte.dirty=1;
  paddr_write(pte_address,4,pte.val);
  } 
  return paddress;
}

