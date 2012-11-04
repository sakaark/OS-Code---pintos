#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "userprog/process.h"
#include "vm/shared_memory.h"

static void syscall_handler (struct intr_frame *);
static int sys_write (int fd, const void *buffer, unsigned size);
static int sys_fork (void *eipf);
static void sys_exec (char *file);

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
  
  if (current->forks != current->forks_done && sys_call != SYS_FORK)
    return;

  switch (sys_call){
  case SYS_WRITE:
    f -> eax = sys_write(arguments[0], (char *)arguments[1], arguments[2]);
    break;
  case SYS_EXIT:
    if (current->shared_mem == true)
      shared_memory_close_sys();
    thread_exit() ;
  case SYS_FORK:
    if (current->forks == current->forks_done){
      current->forks += 1;
      current->forks_done += 1;
      f->eax = sys_fork(f);
    }
    else{
      current->forks_done += 1;
      f->eax=0;
      if (current->forks_done == current->forks)
	f->eax = 0;
      else
	f->eax = 999;
    }
    break;
  case SYS_EXEC:
    //printf("exec pid=%d", current->tid);
    sys_exec((char *)arguments[0]);
    break;
  case SYS_SHM_OPEN:
    f->eax = shared_memory_open_sys(arguments[0]);
    break;
  case SYS_SHM_CLOSE:
    shared_memory_close_sys();
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

static void sys_exec (char *file){
  process_execute(file);
  thread_exit();
}
