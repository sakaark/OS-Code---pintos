/* mkdir.c

   Creates a directory. */

#include <stdio.h>
#include <syscall.h>

int
main (int argc, char *argv[]) 
{
  chdir("dir");
  mkdir("dir_ch");
  mkdir("dir_ch2");
  chdir("..");

  int fd = open("dir");
  printf("fd = %d\n", fd);

  char name[20];

  while (readdir(fd, name))
    printf("%s\n", name);
  
  return EXIT_SUCCESS;
}
