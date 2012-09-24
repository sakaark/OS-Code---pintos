//#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void go(void){
  ASSERT (2==3);
  return;
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int sys_call = *((int *)f->esp);
  if(sys_call == SYS_HALT)
    printf("gotcha!");
  shutdown_power_off();
  printf ("system call!\n");
  go();
  thread_exit ();
}
