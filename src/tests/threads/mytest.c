#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"

void test_mytest(){
  printf("Hello World!\n");
  ASSERT (1==2);
  halt();
  //hello();
}
