#include <stdio.h>
#include "threads/thread.h"
#include "shared_memory.h"
#include "vm/sup_table.h"
#include "threads/palloc.h"
#include "threads/vaddr.h"

static uint8_t *get_user_page();

void *shared_mem = NULL;

void *shared_memory_open_sys(int size){
  if (size > PGSIZE)
    return SHM_ERROR;
  struct thread *t = thread_current();

  if(shared_mem == NULL)
    shared_mem = palloc_get_page(PAL_USER);

  struct sup_entry *entry;
  entry = (struct sup_entry *)malloc(sizeof(struct sup_entry));
  
  if (t->shared_mem == true){
    struct list_elem *e;
    for (e = list_begin (&t->sup_list); e != list_end (&t->sup_list); e = list_next (e)){
      struct sup_entry *f = list_entry (e, struct sup_entry, elem);
      if (f->shared_mem == true)
	return f->page_no;
    }
    NOT_REACHED();
  }

  t->shared_mem = true;

  uint8_t *upage = get_user_page();
  
  entry->page_no = upage;
  entry->kpool_no = shared_mem;
  entry->writable = true;
  entry->stack_page = false;
  entry->shared_mem = true;
  

  list_push_back (&t->sup_list, &entry->elem);
  
  pagedir_set_page(t->pagedir, upage, shared_mem, true);
  return upage;
}

void shared_memory_close_sys(){
  struct thread *cur = thread_current();
  struct list_elem *e;
  if (cur->shared_mem == true){
    for (e = list_begin (&cur->sup_list); e != list_end (&cur->sup_list); ){
      struct sup_entry *f = list_entry (e, struct sup_entry, elem);
      if (f->shared_mem == true){
	cur->shared_mem = false;
	pagedir_clear_page (cur->pagedir, f->page_no);
	e = list_remove(e);
	free(f);
	return;
      }
      else
	e = list_next (e);
    }
    return;
  }
  return;
}

static uint8_t *get_user_page(){
  struct thread *t = thread_current();
  uint8_t *upage = 0x00001000;
  uint8_t *offset = 0x00001000;
  while(is_user_vaddr(upage)){
    bool flag = false;
    struct list_elem *e;
    for (e = list_begin (&t->sup_list); e != list_end (&t->sup_list); e = list_next (e)){
      struct sup_entry *f = list_entry (e, struct sup_entry, elem);
      if (f->page_no == upage){
	flag = true;
	break;
      }
    }
    if (flag){
      upage = (uint8_t *)((uint32_t)upage + (uint32_t)offset);
      continue;
    }
    else
      return upage;
  }
}
