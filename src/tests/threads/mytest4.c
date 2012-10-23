#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"
#include "lib/debug.h"
#include "userprog/syscall.h"

void prin3(void){
  printf("Entered prin3\n");
  int a = 4;
  pthread_exit(&a);
}

void prin2(void){
  printf("Entered prin2\n");
  int *p;
  int i = pthread_join(3, &p);
  if(i == 0) printf("Correct return status = %d\n", i);
  else if(i == EINVAL) printf("Maybe correct status = %d\n", i);
  else if(i == ESRCH) printf("No such thread! = %d\n", i);
  else printf("i = %d\n", i);
  printf("prin2 joined = %d\n", *p);

  int a = 10;
  pthread_exit(&a);
}

void prin1(void){
  printf("Entered prin1\n");
  /*pthread_t *t;
  t = malloc(sizeof(pthread_t));

  pthread_attr_t x;
  pthread_attr_init(&x);
  x.schedpolicy = SCHED_PRIORITY;
  x.inheritsched = 0;
  x.sched_priority = 20;

  pthread_create(t, &x, &prin3, NULL);*/

  int *p;
  int i = pthread_join(3, &p);
  if(i == 0) printf("Correct return status = %d\n", i);
  else if(i == EINVAL) printf("Maybe correct status = %d\n", i);
  else if(i == ESRCH) printf("No such thread! = %d\n", i);
  else printf("i = %d\n", i);
  printf("prin1 joined = %d\n", *p);

  int a = 10;
  pthread_exit(&a);
}

void test_mytest4(){
  pthread_t *t;
  t = malloc(sizeof(pthread_t));

  pthread_t *z;
  z = malloc(sizeof(pthread_t));

  pthread_t *k;
  k = malloc(sizeof(pthread_t));

  pthread_attr_t x;
  pthread_attr_init(&x);
  x.schedpolicy = SCHED_PRIORITY;
  x.inheritsched = 0;
  x.sched_priority = 40;

  printf("Hello World! %s\n", thread_name());

  pthread_create(t, &x, &prin1, NULL);
  x.sched_priority = 50;
  pthread_create(z, &x, &prin2, NULL);
  x.sched_priority = 20;
  pthread_create(k, &x, &prin3, NULL);

  pthread_attr_destroy(&x);
  thread_exit();
}

