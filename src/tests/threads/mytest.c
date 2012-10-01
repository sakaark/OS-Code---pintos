#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"
#include "lib/debug.h"
#include "userprog/syscall.h"
//#include "userprog/mytest.c"
typedef long long int mqd_t;


typedef struct mq_attr {
long mq_flags;
long mq_maxmsg;
long mq_msgsize;
long mq_curmsgs;
}mq_attr;

typedef struct msg_in {
char *cnt;
int msg_len;
int msg_prio;
struct msg_in *prev;
struct msg_in *next;
}msg_in;

typedef struct mq {
char name[100];
mq_attr attr;
msg_in *head;
msg_in *tail;
}mq;

typedef struct mq_in {
mq queue;
struct mq_in *prev;
struct mq_in *next;
}mq_in;

typedef struct mq_list {
mq_in *head;
mq_in *tail;
}mq_list;

typedef struct mq_opened {
int oflag;
mq_in *queue;
struct mq_opened *prev;
struct mq_opened *next;
}mq_opened;

typedef struct openmq_list {
mq_opened *head;
mq_opened *tail;
}openmq_list;


////////////////////////////////////////////////////////////////////////////////////////

openmq_list b;
mq_list c;
//typedef long long int mqd_t;
/*
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
/*printf("joined = %s\n", (char *)p);
  pthread_exit(&k);
}*/

/*void pthread_test(){
 *pthread_t *t;
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

  }*/

void mq_test(){
  
  b.head = NULL;
  b.tail = NULL;
  c.head = NULL;
  c.tail = NULL;
  
  mq_in *t;
  mq_opened *t1;
  const char s[] = "Nikhil";
  const char p[] = "sakaar";
  const char r[] = "aditi";
  
  mqd_t x = mq_open(s,131);
  mqd_t y = mq_open(p,121);
  mqd_t z = mq_open(r,111);
  
  
  t = c.head;
  while(t != NULL)
    {
      printf("%lld \n",t);
      t = t->next;
    }
  int a1 = mq_unlink(s);
  printf("%d \n",a1);
  
  t = c.head;
  while(t != NULL)
    {
      printf("%lld \n",t);
      t = t->next;
    }
  
  t1 = b.head;
  while(t1 != NULL)
    {
      printf("%lld \n",t1);
      t1 = t1->next;
    }
  //int a2 = mq_close(y);
  //int a3 = mq_close(z);
  //int a4 = mq_close(x);
  mqd_t z1 = mq_open(s,111);
  
  t1 = b.head;
  while(t1 != NULL)
    {
      printf("%lld \n",t1);
      t1 = t1->next;
    }

  int a5 = mq_send(x,s,strlen(s),2);
  //int a7 = mq_send(x,p,strlen(p),1);
  char *s1 = (char *)malloc(1001 * sizeof(char));
  int *pr = (int *)malloc(sizeof(int));
  int a6 = mq_receive(x,s1,1001,pr);
  printf("%d %s %d %d\n",a5,s1,a6,pr[0]);
}

void test_mytest(){
  //pthread_test();
  mq_test();
  thread_exit();
}
