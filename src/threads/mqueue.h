#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"
#include "lib/debug.h"

//for mqueues
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

