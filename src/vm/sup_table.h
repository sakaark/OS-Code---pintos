#ifndef SUP_TABLE_H
#define SUP_TABLE_H

#include "lib/kernel/list.h"

struct sup_entry{
  uint32_t page_no;
  uint32_t kpool_no;
  struct list_elem elem;
};

uint32_t *swap_store;
void swapsapce_init();
#endif
