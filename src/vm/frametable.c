#include <stdio.h>
#include "frametable.h"

void frametable_init(){
  list_init(&frame_list);
  lock_init(&frame_lock);
}

