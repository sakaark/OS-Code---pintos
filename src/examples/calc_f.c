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

  int i;

  for(i=0; i<=num; i++){
    int pid = fork();
    //printf("pid=%d\n", pid);
    if (pid != 0){ 
      char a[10];
      snprintf(a, 10, "%d", i);
      char b[40] = "compute_f ";
      strlcat(b, a, 40);
      exec(b);
    }
  }
  return EXIT_SUCCESS;
}
