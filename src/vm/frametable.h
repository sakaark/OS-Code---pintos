#ifndef FRAMETABLE_H
#define FRAMETABLE_H

#include "threads/thread.h"
#include <list.h>
#include "threads/synch.h"

struct frame_entry{
  struct thread *t;
  uint8_t *upage;
  uint8_t *kpage;
  struct list_elem elem;
};

struct list frame_list;
struct lock frame_lock;

void frametable_init();
#endif
