#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"
#include "lib/debug.h"
#include "userprog/syscall.h"

void printing2(void){
  printf("harrah 2!!\n");
  thread_exit();
}

void printing(void){
  pthread_t *t;
  pthread_attr_t x;
  x.detachstate = DETACHED;
  x.inheritsched = 1;
  x.schedpolicy = SCHED_FCFS;
  x.sched_priority = 31;
  t = malloc(sizeof(pthread_t));
  //halt();
  //debug_backtrace_all();
  //halt();
  pthread_create(t, &x, &printing2, NULL);

  printf("hurrah!!\n");
  thread_exit();
}

void test_mytest(){
  pthread_t *t;
  pthread_attr_t x;
  x.detachstate = DETACHED;
  x.inheritsched = 1;
  x.schedpolicy = SCHED_FCFS;
  x.sched_priority = 31;
  printf("Hello World!\n");
  t = malloc(sizeof(pthread_t));
  //halt();
  //debug_backtrace_all();
  //halt();
  pthread_create(t, &x, &printing, NULL);
  
  printf("Caught!!\n");
  //hello();
  thread_exit();
}
