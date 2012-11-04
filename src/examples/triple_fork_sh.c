#include <stdio.h>
#include <syscall.h>
#include <syscall-nr.h>
#include <stdlib.h>

int
main (int argc, char **argv)
{
  int i, pid;

  pid = fork();

  if(pid == 0)
    exec("compute_sh 10");
  else{
    pid = fork();
    if (pid == 0)
      exec("compute_sh 11");
    else{
      pid = fork();
      if (pid == 0)
	exec("compute_sh 12");
      else{
	char *shm = shared_memory_open(32);
	strlcpy(shm, "hi", 3);
	printf("Put in shared memory = %s\n", shm);
	shared_memory_close();
      }
    }
  }
  
  return EXIT_SUCCESS;
}
