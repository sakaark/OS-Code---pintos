#include <stdio.h>
#include <syscall.h>
#include <syscall-nr.h>

int
main (int argc, char **argv)
{
  printf("f(%s)=%s%s\n", argv[1],argv[1],argv[1]);
  return EXIT_SUCCESS;
}
