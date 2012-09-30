//#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "lib/debug.h"
#include "threads/synch.h"
#include "lib/kernel/list.h"

/* datatypes and functions for Pthread implementation */
#define PTHREAD_THREADS_MAX 10

//pthread_create errors (1 = max threads already running, 2 = invalid attr)
#define EAGAIN 1
#define EINVAL 2

/*typedef int pthread_t;*/

/*
typedef enum{DETACHED, JOINED} attr_detach_state;
typedef enum{SCHED_FCFS, SCHED_RR, SCHED_PRIORITY} attr_sched_policy;

//Attributes object
typedef struct attr{
  int detachstate;
  int inheritsched;
  int schedpolicy;
  int sched_priority;
} pthread_attr_t;
*/
//Datatypes for containing the data for all the ready threads.
struct pthread_info{
  int pthread_id;
  int detachstate;
  void *value_ptr;
  struct semaphore running;
  struct list_elem elem;
};

int thread_count = 0;

struct list pthread_list;
struct lock threadcount; //to prevent simultanious use of thread_count.
struct lock listuse; //to prevent simultaneous use of pthread_list

int
sys_pthread_create (pthread_t *thread, 
		const pthread_attr_t *attr, 
		void (*start_routine) (void *), void *arg);
/******************************************************/

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");

  lock_init(&threadcount);
  lock_init(&listuse);
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int *sp = ((char *)(&(f -> vec_no)) + 24);
  int sys_call = *sp;
  
  switch (sys_call){
  case SYS_HALT:            shutdown_power_off();
  case SYS_PTHREADS_CREATE: return sys_pthread_create(*(pthread_t **)(sp + 1), *(pthread_attr_t **)(sp + 2), 
						      *(thread_func **)(sp + 3), *(void **)(sp + 4));
  default:                  printf ("system call!\n"); thread_exit();
  }
}

int
sys_pthread_create (pthread_t *thread, 
		const pthread_attr_t *attr, 
		void (*start_routine) (void *), void *arg){

  if(thread_count >= PTHREAD_THREADS_MAX){
    return EAGAIN;
  }

  //check for valid attr object
  if((((attr -> detachstate) != DETACHED) && 
      ((attr -> detachstate) != JOINED)) || 
     (((attr -> inheritsched) != 0) && 
      ((attr -> inheritsched) != 1)) || 
     (((attr -> schedpolicy) != SCHED_FCFS) && 
      ((attr -> schedpolicy) != SCHED_RR) &&
      ((attr -> schedpolicy) != SCHED_PRIORITY)) ||
     (((attr -> sched_priority) < 0) || 
      ((attr -> sched_priority) > 63)))
    return EINVAL;

  lock_acquire(&threadcount);
  if(thread_count == 0)
    list_init(&pthread_list);

  thread_count++;

  *thread = thread_count;
  lock_release(&threadcount);

  struct pthread_info *t;
  t = malloc(sizeof(struct pthread_info));

  t -> pthread_id = *thread;
  t -> detachstate = attr -> detachstate;
  sema_init(&(t -> running), 0);

  lock_acquire(&listuse);
  list_push_back(&pthread_list, &(t -> elem));
  lock_release(&listuse);
 
  int td;
  td = thread_create("pthread", attr -> sched_priority, start_routine, arg);

  if(td == TID_ERROR){
    list_remove(&(t -> elem));
    lock_acquire(&threadcount);
    thread_count--;
    lock_release(&threadcount);
    return TID_ERROR;
  }

  return 0;
}
