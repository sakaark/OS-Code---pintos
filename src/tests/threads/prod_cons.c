#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"
#include "lib/debug.h"
#include "userprog/syscall.h"
#include "threads/synch.h"

int a[6];
struct semaphore empty, full, mutex;
int produce_at = 0, consume_from = 0;

void consumer(void)
{
  int consumed = 0;
  while(1){
    sema_down(&full);
    sema_down(&mutex);

    consumed = a[consume_from];              //removed the item from the buffer

    sema_up(&mutex);
    sema_up(&empty);

    printf("Consumed %d\n", consumed);       //consumed the item

    consume_from = (consume_from + 1) % 6;   //next item to be consumed updated

  }
}

void producer(void)
{
  int produced = 0;
  for(produced=0; ;produced++){
    printf("\t\tProduced %d\n", produced);              //produced the item

    sema_down(&empty);
    sema_down(&mutex);

    a[produce_at] = produced;                //added the item to the buffer

    sema_up(&mutex);
    sema_up(&full);

    produce_at = (produce_at + 1) % 6;      //updated the value to which next 
                                            //item is to be produced
    if(produced > 50)
      timer_sleep(100000000);
  }
}

void test_prod_cons(){
  sema_init(&empty, 6);
  sema_init(&full, 0);
  sema_init(&mutex, 1);
  pthread_t *t;
  t = malloc(sizeof(pthread_t));

  pthread_t *z;
  z = malloc(sizeof(pthread_t));

  pthread_attr_t x;
  pthread_attr_init(&x);

  printf("Producer Consumer Problem Begins!\n");

  pthread_create(t, &x, &producer, NULL);
  pthread_create(z, &x, &consumer, NULL);
}
