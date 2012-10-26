#include <stdio.h>
#include <syscall.h>

int
main (int argc, char **argv)
{
  int i;

  printf("Hello Hard Disk!!\n");

  /*for (i = 0; i < argc; i++)
    printf ("%s ", argv[i]);*/
  fork();

  return EXIT_SUCCESS;
}
