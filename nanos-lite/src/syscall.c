#include "common.h"
#include "syscall.h"
#include "fs.h"

int mm_brk(uint32_t new_brk);

static inline uintptr_t sys_open(uintptr_t pathname, uintptr_t flags, uintptr_t mode) {
  //TODO();
  return fs_open((char *)pathname,(int)flags,(int)mode);
}

static inline uintptr_t sys_write(uintptr_t fd, uintptr_t buf, uintptr_t len) {
 // TODO();
Log("sys_write:fd %d  len %d",fd,len);    
  return fs_write((int)fd,(void*)buf,(size_t)len);
/*uintptr_t i = 0;
    if (fd == 1 || fd == 2) {
		for(; len > 0; len--) {
		_putc(((char*)buf)[i]);
		i++;;
	}
	   }
	return i;
*/
}

static inline uintptr_t sys_read(uintptr_t fd, uintptr_t buf, uintptr_t len) {
  //TODO();
  return fs_read((int)fd,(void*)buf,(size_t)len);
}

static inline uintptr_t sys_lseek(uintptr_t fd, uintptr_t offset, uintptr_t whence) {
  return fs_lseek((int)fd, (off_t)offset, (int)whence);
}

static inline uintptr_t sys_close(uintptr_t fd) {
  //TODO();
  return fs_close((int)fd);
}

static inline uintptr_t sys_brk(uintptr_t new_brk) {
  //TODO();
  return mm_brk(new_brk);
}
static inline uintptr_t sys_none()
{return 1;
}
/*static inline uintptr_t sys_write()
{
    _putc()

}*/

_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
//  Log("r->ebx %x",r->ebx);
  a[0] = SYSCALL_ARG1(r);
  a[1] = SYSCALL_ARG2(r);
  a[2] = SYSCALL_ARG3(r);
 //Log("a[2] 0x%x",a[2]);
  a[3] = SYSCALL_ARG4(r);
//  Log("a[3] 0x%x",a[3]);
  Log("do syscall %d 0x%x", a[0], r->eip);
  switch (a[0]) {
      case SYS_none:
	            SYSCALL_ARG1(r)=1;
		    break;
      case SYS_exit:
		    _halt(a[1]);
		    break;
      case SYS_write:
   //   Log("a1:%x a2:%x a3:%x",a[1],a[2],a[3]);
      SYSCALL_ARG1(r)=fs_write(a[1],(void *)a[2],a[3]);
//		     printf("sys_write r.eax:%x",r->eax);
		     break;
      case SYS_brk:
		    //Log("brk:%d\n",a[0]);
		    SYSCALL_ARG1(r)=sys_brk(a[1]);
		   break;
      case SYS_open:
		    SYSCALL_ARG1(r)=fs_open((char *)a[1],a[2],a[3]);break;
      case SYS_read:
		     SYSCALL_ARG1(r)=fs_read(a[1],(void *)a[2],a[3]);break;
      case SYS_lseek:
		     SYSCALL_ARG1(r)=sys_lseek(a[1],a[2],a[3]);break;
      case SYS_close:
		     SYSCALL_ARG1(r)=sys_close(a[1]);break;

      default: panic("Unhandled syscall ID = %d", a[0]);
   }   

  return NULL;
}
