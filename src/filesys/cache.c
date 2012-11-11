#include "filesys/cache.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include <stdio.h>
#include "threads/vaddr.h"
#include "lib/kernel/bitmap.h"
#include "threads/palloc.h"
#include "filesys/filesys.h"
#include "lib/string.h"
#include "threads/thread.h"

//cache size
#define CACHE_SIZE 64

//sectors per page
#define SECPP 8

void evict_cache();
void add_readahead(block_sector_t next);

struct cache_block {
  block_sector_t bid;
  void *kpage;
  bool dirty;
  bool accessed;
  int reader;
  int writer;
};

struct cache_block *cache[CACHE_SIZE];
struct bitmap *cache_table;

struct lock cache_glock;

void init_cache(){
  int i,j;
  for(i = 0; i < (CACHE_SIZE / SECPP); i++){
    void *kpage = palloc_get_page(PAL_ZERO);
    for(j = 0; j < SECPP; j++){
      struct cache_block *c = (struct cache_block *)malloc(sizeof(struct cache_block));
      c->reader = 0;
      c->writer = 0;
      c->bid = -1;
      c->kpage = kpage + j * BLOCK_SECTOR_SIZE;
      c->dirty = false;
      c->accessed = false;
      cache[i*SECPP + j] = c;
    }
  }
  lock_init(&cache_glock);
  cache_table = bitmap_create(CACHE_SIZE);
}

size_t add_cache(block_sector_t bid){
  lock_acquire(&cache_glock);
  size_t find = bitmap_scan(cache_table, 0, 1, false);

  if(find == BITMAP_ERROR){
    evict_cache();
    find = bitmap_scan(cache_table, 0, 1, false);
  }

  ASSERT(find != BITMAP_ERROR);

  block_read(fs_device, bid, cache[find]->kpage);
  cache[find]->dirty = false;
  cache[find]->accessed = false;
  cache[find]->bid = bid;

  bitmap_set(cache_table, find, true);
  lock_release(&cache_glock);

  return find;
}

void writeback_cache(int index){
  if(bitmap_test(cache_table, index) == false)
    return;
  cache[index]->reader++;
  if(cache[index]->dirty == true)
    block_write(fs_device, cache[index]->bid, cache[index]->kpage);
  cache[index]->reader--;
}

int find_sector(block_sector_t bid, int rw){
  lock_acquire(&cache_glock);

  int i;
  for(i = 0; i < CACHE_SIZE; i++){
    if(cache[i]->bid == bid){
      if(!rw)
	cache[i]->reader++;
      else if(rw == 1)
	cache[i]->writer++;
      lock_release(&cache_glock);
      return i;
    }
  }
  lock_release(&cache_glock);
  return -1;
}

void read_cache(block_sector_t bid, void *buffer, int offset, int rsize){
  ASSERT(offset < BLOCK_SECTOR_SIZE);
  int get = find_sector(bid, 0);

  if(get != -1){
    get = add_cache(bid);
    cache[get]->reader++;
  }

  memcpy(buffer, cache[get]->kpage + offset, rsize);
  cache[get]->accessed = true;
  cache[get]->reader--;
}

void write_cache(block_sector_t bid, void *buffer, int offset, int wsize){
  ASSERT(offset < BLOCK_SECTOR_SIZE);

  int get = find_sector(bid, 1);
  
  if(get == -1){
    get = add_cache(bid);
    cache[get]->writer++;
  }

  memcpy(cache[get]->kpage + offset, buffer, wsize);
  cache[get]->accessed = true;
  cache[get]->dirty = true;

  cache[get]->writer--;
}

//second chance algorithm
void evict_cache(){
  int i, ind = -1;
  while(ind == -1){
    for(i = 0; i < CACHE_SIZE; i++){
      if(cache[i]->writer == 0 && cache[i]->reader == 0){
	if(cache[i]->accessed == false){
	  ind = i;
	  writeback_cache(ind);
	  bitmap_set(cache_table, ind, false);
	  return;
	}
	else
	  cache[i]->accessed = false;
      }
    }
  }
}

void save_cachetable(){
  lock_acquire(&cache_glock);
  int i;
  for(i = 0; i < CACHE_SIZE; i++){
    writeback_cache(i);
  }
  lock_release(&cache_glock);
}
