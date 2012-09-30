#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"
#include "lib/debug.h"
#include "userprog/syscall.h"
int b = 10;
char c[100] = "abcd";
void printing2(void){
  printf("harrah 2!!\n");
  int k = 5;
  char a[100]= "ghij";
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
  //halt();
  //debug_backtrace_all();
  //halt();
  pthread_create(t, &x, &printing2, NULL);

  printf("hurrah!!\n");
  int k = 4;
  void **p;
  p = malloc(sizeof(void *));
  int i = pthread_join(2, p);
  /*if(i == 0) printf("Correct return status = %d\n", i);
  else if(i == EINVAL_J) printf("Maybe correct status = %d\n", i);
  else if(i == ESRCH_J) printf("No such thread! = %d\n", i);
  else printf("i = %d\n", i);*/
  printf("joined = %s\n", (char *)p);
  pthread_exit(&k);
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
  int i = pthread_create(t, &x, &printing, NULL);
  //hello();
  thread_exit();
}
