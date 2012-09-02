#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"
#include "lib/kernel/list.h"
#include <string.h>

typedef int mqd_t;
typedef int ssize_t;

struct message{
  struct list_elem elem;
  char msg[50];
  unsigned msg_prio;
};

struct queue{
  struct list_elem elem;
  struct list message_list;
  mqd_t qid;
  char name[50];
};

struct list queue_list;
int queue_id = 1;

int mq_open(char *name, int oflag){
  struct queue new;
  
  if(list_empty(&queue_list)){
    new.qid = queue_id++;
    make_equal(new.name, name);
    return new.qid;
  }

  struct list_elem *e;
  
  for (e = list_begin (&queue_list); e != list_end (&queue_list); e = list_next (e)){
    struct queue *f = list_entry (e, struct queue, elem);
    
    if(strcmp(f->name, name))
      continue;
    else{
      if (f->qid < 0)
	f->qid = queue_id++;
      return f->qid;
    }
  }
  new.qid = queue_id++;
  make_equal(new.name, name);
  return new.qid;
}

int mq_send(mqd_t mqdes, const char *msg_ptr,size_t msg_len, unsigned msg_prio){
  struct list_elem *e;
  
  for (e = list_begin (&queue_list); e != list_end (&queue_list); e = list_next (e)){
    struct queue *f = list_entry (e, struct queue, elem);
    
    if(f->qid == mqdes){
      struct list_elem *k;
      for (k = list_begin (&(f->message_list)); k != list_end (&(f->message_list)); k = list_next (k)){
	  struct message *g = list_entry (k, struct message, elem);
	  if(g->msg_prio < msg_prio){
	    list_insert(k, g->elem);
	  }
	  else
	    continue;
      }
      return 0;
    }
    else
      continue;
  }
  return -1;
}

ssize_t mq_receive(mqd_t mqdes, char *msg_ptr,size_t msg_len, unsigned *msg_prio){
  struct list_elem *e;
  
  for (e = list_begin (&queue_list); e != list_end (&queue_list); e = list_next (e)){
    struct queue *f = list_entry (e, struct queue, elem);

    if(f->qid == mqdes){
      message *front = list_front(&(f->message_list));
      make_equal(msg_ptr, front->msg);
      int a = front->msg_prio;
      msg_prio = &a;
      list_pop_front(f->message_list);
      return strlen(msg_ptr);
    }
    else
      continue;
  }

  return -1;
}

int mq_close(mqd_t mqdes){
  struct list_elem *e;
  
  for (e = list_begin (&queue_list); e != list_end (&queue_list); e = list_next (e)){
    struct queue *f = list_entry (e, struct queue, elem);

    if(f->qid == mqdes){
      f->qid= -1;
      return 0;
    }
    else
      continue;
  }
  return -1;
}

int mq_unlink(const char *name){
struct list_elem *e;
  
  for (e = list_begin (&queue_list); e != list_end (&queue_list); e = list_next (e)){
    struct queue *f = list_entry (e, struct queue, elem);
 
    if(strcmp(f->name, name))
      continue;
    else{
      list_remove(e);
      return 0;
    }
  }
  return -1;
}


void make_equal(char a[], char b[]){
  int i;
  for(i = 0; b[i] != 0; i++)
    a[i] = b[i];

  a[i] = 0;
}
