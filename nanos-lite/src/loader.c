#include "common.h"
#include "memory.h"
//#define DEFAULT_ENTRY ((void *)0x4000000)
#define DEFAULT_ENTRY ((void *)0x8048000)
size_t get_ramdisk_size();
void ramdisk_read(void *,off_t,size_t);

int fs_open(const char *pathname, int flags, int mode);
ssize_t fs_read(int fd, void *buf, size_t len);
ssize_t fs_write(int fd, const void *buf, size_t len);
size_t fs_filesz(int fd);
int fs_close(int fd);

uintptr_t loader(_Protect *as, const char *filename) 
{
  //ramdisk_read(DEFAULT_ENTRY,0,get_ramdisk_size());
// filename="/bin/hello";
   //Log("before open:%s",filename);
   //int fd=fs_open(filename,0,0);
   //Log("fd%d  filename%s",fd,filename);

//PA3.1
 /*   int fd = fs_open(filename, 0, 0);
	fs_read(fd, DEFAULT_ENTRY, fs_filesz(fd));
	fs_close(fd);
  return (uintptr_t)DEFAULT_ENTRY;*/
    int fd=fs_open(filename,0,0);
    int size=fs_filesz(fd);
   void *va,*pa;
   va=DEFAULT_ENTRY;
for(;va<size+DEFAULT_ENTRY;va+=PGSIZE)
    {
    pa=new_page();
Log("va 0x%x,pa 0x%x",va,pa);
    _map(as,va,pa);
  fs_read(fd,pa,PGSIZE);
// va+=PGSIZE;
 //size=size-PGSIZE;

    }
    fs_close(fd);
    return (uintptr_t)DEFAULT_ENTRY;
}
