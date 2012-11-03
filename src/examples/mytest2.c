#include <stdio.h>
#include <syscall.h>
#include <syscall-nr.h>
#include <stdlib.h>

int
main (int argc, char **argv)
{
  if (argc != 2){
    printf("Invalid no. of arguments!!\n");
    return EXIT_SUCCESS;
  }

  int num = atoi(argv[1]);

  int i, pid;

  pid = fork();

  if(pid == 0)
    exec("compute_f 10");
  else{
    pid = fork();
    if (pid == 0)
      exec("compute_f 11");
    else{
      pid = fork();
      if (pid == 0)
	exec("compute_f 12");
    }
  }
  
  return EXIT_SUCCESS;
}
