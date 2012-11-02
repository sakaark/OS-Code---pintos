#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

struct aux_fork{
  char *file;
  struct intr_frame *f;
  char *stack_ptr;
  char *stack;
  size_t size;
};

tid_t process_execute (const char *file_name);
tid_t fork_execute (void *eipf);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

#endif /* userprog/process.h */
