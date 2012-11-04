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
      /*char *shm = shared_memory_open(32);
      printf("shared_memory = %p\n", shm);
      strlcpy(shm, "hi", 3);*/

      if (pid == 0)
	exec("compute_f 12");
      else{
	char *shm = shared_memory_open(32);
	printf("shared_memory = %p\n", shm);
	strlcpy(shm, "hi", 3);
	printf("putter = %s\n",shm);
	
	shared_memory_close();
      }
    }
  }
  
  return EXIT_SUCCESS;
}
