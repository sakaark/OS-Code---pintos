#ifndef SUP_TABLE_H
#define SUP_TABLE_H

#include "lib/kernel/list.h"

struct sup_entry{
  uint8_t *page_no;
  uint8_t *kpool_no;
  bool writable;
  bool stack_page;
  bool shared_mem;
  struct list_elem elem;
};

void swapsapce_init();
uint8_t *swap_get_page();
void swap_free_page(uint8_t *pages);
#endif
