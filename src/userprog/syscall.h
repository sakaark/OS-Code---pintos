typedef enum {DETACHED, JOINED} ;
typedef enum {SCHED_FCFS, SCHED_RR, SCHED_PRIORITY} ;
typedef int pthread_t;

//Attributes object
typedef struct attri {
  int detachstate;
  int inheritsched;
  int schedpolicy;
  int sched_priority;
} pthread_attr_t;

//pthread_create errors (1 = max threads already running, 2 = invalid attr)
#define EAGAIN 1
#define EINVAL 2

//pthread_join errors (1 = detached thread. 2 = invalid pthread_t)
#define EINVAL_J 1
#define ESRCH_J  2

#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
//static void syscall_handler (struct intr_frame *);

#endif /* userprog/syscall.h */
