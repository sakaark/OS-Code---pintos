#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "userprog/process.h"

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
  int *arguments = f -> esp;
  arguments++;
  struct thread *current = thread_current();

  int sys_call = *((int *)f -> esp);
  
  if (current->forked == true && sys_call != SYS_FORK)
    return;

  switch (sys_call){
  case SYS_WRITE:
    f -> eax = sys_write(arguments[0], (char *)arguments[1], arguments[2]);
    break;
  case SYS_EXIT:
    thread_exit() ;
  case SYS_FORK:
    if (current->forked == true){
      f->eax = 0;
      current->forked = false;
    }
    else {
      f -> eax = sys_fork(f);
    }
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
  uint32_t *esp;
  asm ("mov %%esp, %0" : "=g" (esp));
  struct thread *t = thread_current();
  tid_t pid=-1;
  char *file_name = t->name;
  struct aux_fork *a;
  a = (struct aux_fork *)malloc(sizeof(struct aux_fork));
  a->f = (struct intr_frame *)malloc(sizeof(struct intr_frame));
  memcpy(a->f, (struct intr_frame *)eipf, sizeof(struct intr_frame));
  if(strcmp(t->name, "forked"))
    pid = fork_execute(a);
  enum intr_level old_level;
  return pid;
}
