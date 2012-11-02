#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/synch.h"

static void syscall_handler (struct intr_frame *);
static int sys_write (int fd, const void *buffer, unsigned size);
static int sys_fork (void *eipf);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
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
    //while(1); 
    thread_exit() ;
  case SYS_FORK:
    f -> eax = sys_fork(f);
    break;
  default:
    break;
  }
}

static int sys_write (int fd, const void *buffer, unsigned size){
  if (fd == 1){
    putbuf (buffer, size);
    return size;
  }
}

static int sys_fork (void *eipf){
  struct thread *t = thread_current();
  tid_t pid=0;
  char *file_name = t->name;
  printf("forking:\n");
  //process_execute(file_name);
  //printf("forked!!\n");
  struct intr_frame *k;
  k = (struct intr_frame *)malloc(sizeof(struct intr_frame));
  memcpy(k, (struct intr_frame *)eipf, sizeof(struct intr_frame));
  pid = fork_execute(k);
  printf("pid=%d\n", pid);
  //thread_exit();
  printf("forked!\n");
  enum intr_level old_level;
  old_level = intr_disable();
  print_ready_list();
  print_all_list();
  intr_set_level (old_level);
  return pid;
}
