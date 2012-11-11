#ifndef FILESYS_CACHE_H
#define FILESYS_CACHE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "devices/block.h"
#include "threads/synch.h"
#include <stdio.h>

#define debug 0

void read_cache (block_sector_t bid, void *buffer, int offset, int rsize);
void init_cache (void);

void write_cache (block_sector_t bid, void *buffer, int offset, int wsize);
void save_cachetable (void);

#endif
