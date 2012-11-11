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
static int sys_mkdir (char *dirname);
static int sys_chdir (char *dirname);
static int sys_open(char *filename);
static bool sys_readdir (int fd, char *name);

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
    break;
  case SYS_CHDIR:
    f->eax = sys_chdir((char *)arguments[0]);
    break;
  case SYS_OPEN:
    f->eax = sys_open((char *)arguments[0]);
    break;
  case SYS_READDIR:
    f->eax = sys_readdir(arguments[0], (char *)arguments[1]);
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

  a->t = t;

  pid = fork_execute(a);
  
  sema_down(&(t->fork_sema));
  
  return pid;
}

static void sys_exec (char *file){
  process_execute(file);
  thread_exit();
}

static int sys_mkdir (char *dirname) {
  //if empty or root return
  if(strlen(dirname) ==  0 || strcmp( dirname, "/") == 0) { 
    return -1;
  }
  
  bool success = filesys_create(dirname, 0,FILE_DIR);
  if ( success ) return 0;
  else return -1;
} 

static int sys_chdir (char *dirname) {
  //if empty or root return
  if(strlen(dirname) ==  0 || !strcmp( dirname, "/")) return -1;
  
  /* CLOSE FILE */
  struct file * file  = filesys_open(dirname);
  
  if (file == NULL) {
    return -1;
  }
  struct inode * myinode = file_get_inode (file);
  
  enum file_type type = inode_type (myinode);

  //if the file is a dir open it and set pwd to it
  if(type == FILE_DIR) {
    dir_close(thread_current()->pwd);
    thread_current()->pwd = dir_open(inode_reopen(myinode));
    //sys_mkdir("hello");
    //char name[16];
    //while (dir_readdir (thread_current()->pwd, name))
    //printf ("%s\n", name);
    return 0;
  }
  else return -1;
  
  file_close(file);
}

static int
sys_open(char *filename)
{
  int fd;

  struct file * file = filesys_open (filename);
  if (file == NULL) {
    fd = -1;
  } else {
    //if not a directory
    if(!file_direct(file)) { 
      fd = add_file(file);
    } else {
      struct inode * myinode = file_get_inode (file);
      struct dir * mydir = dir_open (myinode);		
      fd = add_filemmap(file,mydir);
    }
  }
  return fd;
}

static bool sys_readdir (int fd, char *name) {
  //get the fd and check if fd is a directory
  struct filed * filed = find_file(fd);

  if ( filed == NULL ) {
    return false;
  }
        
  struct file * file = filed->file;

  if ( file == NULL ) {
    return false;
  }

  //check if it is a directory
  if ( !file_direct(file) ) {
    return false;
  }
  //now we can start
  struct dir * mydir = (struct dir *)filed->vaddr;
  bool success = dir_readdir (mydir, name);

  if (success) return true;
  else return false;
}
