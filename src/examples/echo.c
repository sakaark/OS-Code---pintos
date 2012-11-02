#include <stdio.h>
#include <syscall.h>
#include <syscall-nr.h>

#define syscall0(NUMBER)                                        \
        ({                                                      \
          int retval;                                           \
          asm volatile                                          \
            ("pushl %[number]; int $0x30; addl $4, %%esp"       \
               : "=a" (retval)                                  \
               : [number] "i" (NUMBER)                          \
               : "memory");                                     \
          retval;                                               \
        })


int
main (int argc, char **argv)
{
  int i;

  printf("Hello Hard Disk!!\n");

  for (i = 0; i < argc; i++)
    printf ("%s ", argv[i]);

  //fork();
  i = (pid_t) syscall0 (SYS_FORK);

  printf("Phew! tough job! pid=%d\n", i);
  //while(1);

  return EXIT_SUCCESS;
}
