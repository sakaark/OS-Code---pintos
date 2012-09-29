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


#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H



void syscall_init (void);
//static void syscall_handler (struct intr_frame *);

#endif /* userprog/syscall.h */
