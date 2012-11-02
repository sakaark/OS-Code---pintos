#include <stdio.h>
#include <syscall.h>
#include <syscall-nr.h>

int
main (int argc, char **argv)
{
  int i;

  printf("Hello Hard Disk!! ");
  printf("I love USERPROG!!\n");

  for (i = 0; i < argc; i++)
    printf ("%s ", argv[i]);

  int t = 4;
  //fork();
  i = fork();

  printf("Phew! tough job! pid=%d\n", i);
  while(1);



  return EXIT_SUCCESS;
}
