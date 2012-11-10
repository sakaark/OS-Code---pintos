#include <stdio.h>
#include <syscall.h>
#include <syscall-nr.h>

int
main (int argc, char **argv)
{
  char *shm = shared_memory_open(40);
  printf("f(%s)=%s%s shm=%p\n", argv[1],argv[1],argv[1], shm);
  printf("shared = %c%c%c\n", shm[0],shm[1],shm[2]);
  shared_memory_close();
  return EXIT_SUCCESS;
}
