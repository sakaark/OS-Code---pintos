/* chdir.c

   Creates a directory. */

#include <stdio.h>
#include <syscall.h>

int
main (int argc, char *argv[]) 
{
  if (argc != 2) 
    {
      printf ("usage: %s DIRECTORY\n", argv[0]);
      return EXIT_FAILURE;
    }
  if (chdir(argv[1]) == -1) 
    {
      printf ("%s: chdir failed\n", argv[1]);
      return EXIT_FAILURE;
    }
  else
    printf("directory changed\n");
  
  return EXIT_SUCCESS;
}
