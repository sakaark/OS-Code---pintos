#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"
#include "lib/debug.h"

void test_mytest(){
  printf("Hello World!\n");
  //halt();
  //debug_backtrace_all();
  halt();
  printf("Caught!!\n");
  //hello();
}
