#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#define SHM_ERROR -1

void *shared_memory_open_sys(int size);

#endif
