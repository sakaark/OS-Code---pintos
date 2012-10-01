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

/*pthread errrs:
  1 = max threads already running, 
  2 = invalid attr(maybe detached= thread.detachstate),
  3 = invalid pthread_id)
 */
#define EAGAIN 1
#define EINVAL 2
#define ESRCH  3

//destry value for attr object
#define ATTR_DESTROY 100

#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H



void syscall_init (void);
//static void syscall_handler (struct intr_frame *);

#endif /* userprog/syscall.h */
