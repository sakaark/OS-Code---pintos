#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"
#include "lib/debug.h"
#include "userprog/syscall.h"

void printing3(int n){
  printf("The argument is %d\n", n);
  pthread_exit();
}

void printing31(int n){
  printf("The argument is %d\n", n);
  pthread_exit();
}

void test_mytest2(){
  pthread_t *t, *z;
  t = malloc(sizeof(pthread_t));
  z = malloc(sizeof(pthread_t));
  pthread_attr_t x;
  pthread_attr_init(&x);
  x.schedpolicy = SCHED_PRIORITY;
  x.sched_priority = 37;
  x.inheritsched = 0;
  pthread_create(t, &x, &printing3, 3);
  pthread_create(z, &x, &printing31, 31);
  pthread_attr_destroy(&x);
  pthread_setschedparam(*z, SCHED_PRIORITY, 40);
  printf("Main thread resumes!\n");
  thread_exit();
}
