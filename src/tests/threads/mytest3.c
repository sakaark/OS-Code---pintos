#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"
#include "lib/debug.h"
#include "userprog/syscall.h"

void printing5(void){
  printf("harrah 2!!\n");
  int a = 4;
  pthread_exit(&a);
}

void printing4(void){
  pthread_t *t;
  pthread_attr_t x;
  pthread_attr_init(&x);
  printf("proved!!\n");
  t = malloc(sizeof(pthread_t));
  pthread_create(t, &x, &printing5, NULL);
  int l = pthread_setschedparam(*t, SCHED_FCFS, 39);
  printf("hurrah !!\n");
  int *p;
  char z='c';
  p = &z;
  //pthread_cancel(*t);
  int i = pthread_join(*t, &p);
  if(i == 0) printf("Correct return status = %d\n", i);
  else if(i == EINVAL) printf("Maybe correct status = %d\n", i);
  else if(i == ESRCH) printf("No such thread! = %d\n", i);
  else printf("i = %d\n", i);
  printf("joined = %d\n", *p);
  pthread_exit("hi");
}

void test_mytest3(){
  //  print_ready_queue();
  pthread_t *t;
  pthread_attr_t x;
  pthread_attr_init(&x);
  x.schedpolicy = SCHED_FCFS;
  x.inheritsched = 1;
  t = malloc(sizeof(pthread_t));
  int i = pthread_create(t, &x, &printing4, NULL);
  pthread_attr_destroy(&x);
  thread_exit();
}


