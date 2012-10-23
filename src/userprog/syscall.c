//#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <stdlib.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "lib/debug.h"
#include "threads/synch.h"
#include "lib/kernel/list.h"
#include "threads/interrupt.h"

/****************** Datatypes and functions for Pthread implementation ********************/
#define PTHREAD_THREADS_MAX 10

//Datatypes for containing the data for all the ready threads.
struct pthread_info{
  int pthread_id;
  int detachstate;
  void *value_ptr;
  struct semaphore running;
  struct list_elem elem;
  int done;
};

int thread_count = 0;
int id_count = 1;

struct list pthread_list;
struct lock threadcount; //to prevent simultanious use of thread_count.
struct lock listuse; //to prevent simultaneous use of pthread_list
 

int
sys_pthread_create (pthread_t *thread, 
		const pthread_attr_t *attr, 
		void (*start_routine) (void *), void *arg);

void sys_pthread_exit (void *value_ptr);
int sys_pthread_join(pthread_t thread, void **retval);
int sys_pthread_cancel(pthread_t thread);
int sys_pthread_attr_init(pthread_attr_t *attr);
int sys_pthread_attr_destroy(pthread_attr_t *attr);
int sys_pthread_setschedparam (pthread_t thread, int policy, int sched_priority);
/*******************************************************************************************/

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
  int k;
  
  switch (sys_call){
  case SYS_HALT:                  shutdown_power_off();
  case SYS_PTHREADS_CREATE:       k = sys_pthread_create(*(pthread_t **)(sp + 1), *(pthread_attr_t **)(sp + 2), *(thread_func **)(sp + 3), *(void **)(sp + 4)); 
                                  f -> eax = k; break;
  case SYS_PTHREADS_EXIT:         sys_pthread_exit(*(void **)(sp + 1)); break;
  case SYS_PTHREADS_JOIN:         k = sys_pthread_join(*(pthread_t *)(sp + 1), *(void ***)(sp + 2)); f -> eax = k; break;
  case SYS_PTHREADS_CANCEL:       k = sys_pthread_cancel(*(pthread_t *)(sp + 1)); f -> eax = k; break;
  case SYS_PTHREADS_ATTR_INIT:    k = sys_pthread_attr_init(*(pthread_attr_t **)(sp + 1)); f -> eax = k; break;
  case SYS_PTHREADS_ATTR_DESTROY: k = sys_pthread_attr_destroy(*(pthread_attr_t **)(sp + 1)); f -> eax = k; break;
  case SYS_PTHREADS_SETSCHEDPARAM: k = sys_pthread_setschedparam(*(pthread_t **)(sp + 1), *(int **)(sp + 2), *(int **)(sp + 3)); f -> eax = k; break;
  default:                        printf ("system call!\n"); thread_exit();
  }
  return;
}

int
sys_pthread_create (pthread_t *thread, 
		const pthread_attr_t *attr, 
		void (*start_routine) (void *), void *arg){

  if(thread_count >= PTHREAD_THREADS_MAX){
    return EAGAIN;
  }

  if (attr == NULL){
    attr = malloc(sizeof(pthread_attr_t));
    sys_pthread_attr_init(attr);
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

  *thread = id_count++;
  lock_release(&threadcount);

  struct pthread_info *t;
  t = malloc(sizeof(struct pthread_info));

  t -> pthread_id = *thread;
  t -> detachstate = attr -> detachstate;
  t -> done = 0;
  sema_init(&(t -> running), 0);
  char name[50];
  snprintf(name, 50, "%d", t -> pthread_id);

  lock_acquire(&listuse);
  list_push_back(&pthread_list, &(t -> elem));
  lock_release(&listuse);

  if(attr -> inheritsched != 1)
    set_sched_policy(attr -> schedpolicy);

  int td;
  td = thread_create(name, attr -> sched_priority, start_routine, arg);

  return 0;
}

void sys_pthread_exit (void *value_ptr){
  pthread_t pthread_id = atoi(thread_name());
  struct list_elem *e;
  struct pthread_info *req_elem;

  lock_acquire(&listuse);
  for (e = list_begin (&pthread_list); e != list_end (&pthread_list);
       e = list_next (e))
    {
      struct pthread_info *f = list_entry (e, struct pthread_info, elem);
      if(f -> pthread_id == pthread_id){
	req_elem = f;
	break;
      }
    }

  if(req_elem -> detachstate == DETACHED){
    list_remove(e);
    free(req_elem);
  }

  else{
    req_elem -> value_ptr = malloc(sizeof(void *));
    *(int *)(req_elem -> value_ptr) = *(int *)value_ptr;
    sema_up_all(&(req_elem -> running));
  }

  lock_release(&listuse);

  lock_acquire(&threadcount);
  thread_count--;
  lock_release(&threadcount);

  thread_exit();
}

int sys_pthread_join(pthread_t thread, void **retval){
  struct list_elem *e;
  struct pthread_info *req_elem;
  lock_acquire(&listuse);
  for (e = list_begin (&pthread_list); e != list_end (&pthread_list);
       e = list_next (e))
    {
      struct pthread_info *f = list_entry (e, struct pthread_info, elem);
      if(f -> pthread_id == thread){
	if(f -> detachstate == DETACHED){
	  lock_release(&listuse);
	  return EINVAL;
	}

	lock_release(&listuse);
	sema_down(&(f -> running));
	lock_acquire(&listuse);
	
	if(f->done == 0){
	  *retval = f -> value_ptr;
	  f -> pthread_id = -1;
	  f -> done = 1;
	  list_remove(&(f -> elem));
	  lock_release(&listuse);
	  sema_up(&(f -> running));
	  return 0;
	}
	else{
	  lock_release(&listuse);
	  sema_up(&(f -> running));
	  return ESRCH;
	}
      }
    }
  lock_release(&listuse);
  return ESRCH;
}

int sys_pthread_cancel(pthread_t thread){
  struct list_elem *e;
  enum intr_level old_level;
  lock_acquire(&listuse);
  for (e = list_begin (&pthread_list); e != list_end (&pthread_list);
       e = list_next (e))
    {
      struct pthread_info *f = list_entry (e, struct pthread_info, elem);
      if(f -> pthread_id == thread){
	old_level = intr_disable();
	thread_cancel(f->pthread_id, old_level);
	list_remove(e);
	lock_release(&listuse);
	free(f);

	lock_acquire(&threadcount);
	thread_count--;
	lock_release(&threadcount);
	
	return 0;
      }
    }
  lock_release(&listuse);
  return ESRCH;
}

int sys_pthread_attr_init(pthread_attr_t *attr){
  
  attr -> detachstate = JOINED;
  attr -> inheritsched = 1;
  attr -> schedpolicy = SCHED_RR;
  attr -> sched_priority = 31;

  return 0;
}

int sys_pthread_attr_destroy(pthread_attr_t *attr){
  attr -> detachstate = ATTR_DESTROY;
  attr -> inheritsched = ATTR_DESTROY;
  attr -> schedpolicy = ATTR_DESTROY;
  attr -> sched_priority = ATTR_DESTROY;
  return 0;
}

int sys_pthread_setschedparam (pthread_t thread, int policy, int sched_priority){
  if(((policy != SCHED_FCFS) &&
      (policy != SCHED_RR) &&
      (policy != SCHED_PRIORITY)) ||
     ((sched_priority < 0) ||
      (sched_priority >63)))
    return EINVAL;

  struct list_elem *e;
  lock_acquire(&listuse);
  for (e = list_begin (&pthread_list); e != list_end (&pthread_list);
       e = list_next (e))
    {
      struct pthread_info *f = list_entry (e, struct pthread_info, elem);
      if(f -> pthread_id == thread){
	thread_set_priority_now(thread, sched_priority);
	lock_release(&listuse);
	set_sched_policy(policy);
	return 0;
      }
    }
  lock_release(&listuse);
  return ESRCH;
}
