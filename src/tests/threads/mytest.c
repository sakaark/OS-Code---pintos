#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"
#include "lib/debug.h"
#include "threads/mqueue.h"

openmq_list b;
mq_list c;

void mq_test(){
  
  b.head = NULL;
  b.tail = NULL;
  c.head = NULL;
  c.tail = NULL;
  
  mq_in *t;
  mq_opened *t1;
  const char s[] = "NikhilA";
  const char p[] = "sakaar";
  const char r[] = "aditi";
  
  mqd_t x = mq_open(s,131);
  mqd_t y = mq_open(p,121);
  mqd_t z = mq_open(r,111);
  
  
  t = c.head;
  while(t != NULL)
    {
      printf("initial mqueue list = %u \n",t);
      t = t->next;
    }
  int a1 = mq_unlink(s);
  printf("unlinked = %d \n",a1);
  
  t = c.head;
  while(t != NULL)
    {
      printf("mqueue after unlink = %u \n",t);
      t = t->next;
    }
  
  t1 = b.head;
  while(t1 != NULL)
    {
      printf("open mqueue = %u \n",t1);
      t1 = t1->next;
    }
  
  t1 = b.head;
  while(t1 != NULL)
    {
      printf("open mqueue2 = %u \n",t1);
      t1 = t1->next;
    }
    
  int a5 = mq_send(x,s,strlen(s),2);
  int a7 = mq_send(x,p,strlen(p),1);
  char *s1 = (char *)malloc(100 * sizeof(char));
  char *s2 = (char *)malloc(100 * sizeof(char));
  int *pr = (int *)malloc(sizeof(int));
  int *pr2 = (int *)malloc(sizeof(int));
  int a6 = mq_receive(x,s1,1001,pr);
  int a8 = mq_receive(x,s2,1001,pr2);
  printf("%d %d %s %s %d %d %d %d\n", a5, a7, s1, s2, a6, a8, *pr, *pr2);
}

void test_mytest(){
  //pthread_test();
  mq_test();
  thread_exit();
}
