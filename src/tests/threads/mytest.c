#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"
#include "lib/debug.h"
#include "userprog/syscall.h"

void printing2(void){
  printf("harrah 2!!\n");
  int a = 4;
  pthread_exit(&a);
}

void printing(void){
  pthread_t *t;
  pthread_attr_t x;
  pthread_attr_init(&x);
  t = malloc(sizeof(pthread_t));
  pthread_create(t, &x, &printing2, NULL);
  int l = pthread_setschedparam(*t, SCHED_RR, 39);
  printf("hurrah %d !!\n", l + 1);
  int *p;
  char z='c';
  p = &z;
  pthread_cancel(*t);
  int i = pthread_join(*t, &p);
  if(i == 0) printf("Correct return status = %d\n", i);
  else if(i == EINVAL) printf("Maybe correct status = %d\n", i);
  else if(i == ESRCH) printf("No such thread! = %d\n", i);
  else printf("i = %d\n", i);
  printf("joined = %d\n", *p);
  pthread_exit("hi");
}

void test_mytest(){
  pthread_t *t;
  pthread_attr_t x;
  pthread_attr_init(&x);
  printf("Hello World! %s\n", thread_name());
  t = malloc(sizeof(pthread_t));
  int i = pthread_create(t, &x, &printing, NULL);
  pthread_attr_destroy(&x);
  thread_exit();
}

