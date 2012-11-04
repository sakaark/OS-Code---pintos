#include <stdio.h>
#include <syscall.h>
#include <syscall-nr.h>

int
main (int argc, char **argv)
{
  char *shm = shared_memory_open(40);
  printf("shared = %s\n", shm);
  shared_memory_close();
  printf("f(%s)=%s%s\n", argv[1],argv[1],argv[1]);
  return EXIT_SUCCESS;
}
