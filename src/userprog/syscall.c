#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/synch.h"

static void syscall_handler (struct intr_frame *);
static int sys_write (int fd, const void *buffer, unsigned size);
static int sys_fork ();

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  //printf ("system call!\n");
  int *arguments = f -> esp;
  arguments++;
  struct thread *t = thread_current();
 
  int sys_call = *((int *)f -> esp);
  
  switch (sys_call){
  case SYS_WRITE:
    f -> eax = sys_write(arguments[0], (char *)arguments[1], arguments[2]);
    break;
  case SYS_EXIT:
    thread_exit();
  case SYS_FORK:
    sys_fork();
  default:
    break;
  }
}

static int sys_write (int fd, const void *buffer, unsigned size){
  struct file *file;
  int result;
  
  if (fd == 1){
    putbuf (buffer, size);
    return size;
  }
}

static int sys_fork (){
  struct thread *t = thread_current();
  t->forking = true;
  char *file_name = t->name;
  printf("forking:\n");
  process_execute(file_name);
  printf("forked!!\n");
  return 1;
}
