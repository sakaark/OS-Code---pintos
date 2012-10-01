#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"
#include "lib/debug.h"
#include "userprog/syscall.h"

void printing2(void){
  printf("harrah 2!!\n");
  pthread_exit("abcd");
}

void printing(void){
  pthread_t *t;
  pthread_attr_t x;
  x.detachstate = JOINED;
  x.inheritsched = 1;
  x.schedpolicy = SCHED_FCFS;
  x.sched_priority = 31;
  t = malloc(sizeof(pthread_t));
  pthread_create(t, &x, &printing2, NULL);
  printf("hurrah!!\n");
  char *p;
  char z='c';
  p = &z;
  //  pthread_cancel(*t);
  int i = pthread_join(*t, &p);
  if(i == 0) printf("Correct return status = %d\n", i);
  else if(i == EINVAL) printf("Maybe correct status = %d\n", i);
  else if(i == ESRCH) printf("No such thread! = %d\n", i);
  else printf("i = %d\n", i);
  printf("joined = %s\n", p);
  pthread_exit("hi");
}

void test_mytest(){
  pthread_t *t;
  pthread_attr_t x;
  x.detachstate = DETACHED;
  x.inheritsched = 1;
  x.schedpolicy = SCHED_FCFS;
  x.sched_priority = 31;
  printf("Hello World! %s\n", thread_name());
  t = malloc(sizeof(pthread_t));
  int i = pthread_create(t, &x, &printing, NULL);
  //hello();
  thread_exit();
}
