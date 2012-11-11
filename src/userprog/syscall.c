#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "userprog/process.h"
#include "vm/shared_memory.h"
#ifdef FILESYS
#include "filesys/inode.h"
#endif

static void syscall_handler (struct intr_frame *);
static int sys_write (int fd, const void *buffer, unsigned size);
static int sys_fork (void *eipf);
static void sys_exec (char *file);
int sys_mkdir (char *dirname);

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
  
  switch (sys_call){
  case SYS_WRITE:
    f -> eax = sys_write(arguments[0], (char *)arguments[1], arguments[2]);
    break;
  case SYS_EXIT:
    if (current->shared_mem == true)
      shared_memory_close_sys();
    thread_exit() ;
  case SYS_FORK:
    f->eax = sys_fork(f);
    break;
  case SYS_EXEC:
    sys_exec((char *)arguments[0]);
    break;
  case SYS_SHM_OPEN:
    f->eax = shared_memory_open_sys(arguments[0]);
    break;
  case SYS_SHM_CLOSE:
    shared_memory_close_sys();
    break;
  case SYS_MKDIR:
    f->eax = sys_mkdir((char *)arguments[0]);
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

  a->t = t;

  pid = fork_execute(a);
  
  sema_down(&(t->fork_sema));
  
  return pid;
}

static void sys_exec (char *file){
  process_execute(file);
  thread_exit();
}

int sys_mkdir (char *dirname) {
  //if empty or root return
  if(strlen(dirname) ==  0 || strcmp( dirname, "/") == 0) { 
    return -1;
  }
  
  bool success = filesys_create(dirname, 0,FILE_DIR);
  
  if ( success ) return 0;
  else return -1;
} 
