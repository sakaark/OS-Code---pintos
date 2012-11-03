#include <stdio.h>
#include <syscall.h>
#include <syscall-nr.h>

int
main (int argc, char **argv)
{
  int i, pid;

  printf("Hello Hard Disk!! ");
  printf("I love USERPROG!!\n");

  int t = 4;
  //fork();
  pid = fork();

  for (i = 0; i < argc; i++)
    printf ("%s ", argv[i]);

  printf("\nPhew! tough job! pid=%d\n", pid);

  return EXIT_SUCCESS;
}
