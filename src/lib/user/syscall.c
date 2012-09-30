//#include <syscall.h>
#include "lib/user/syscall.h"
#include "../syscall-nr.h"


/* Invokes syscall NUMBER, passing no arguments, and returns the
   return value as an `int'. */
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

/* Invokes syscall NUMBER, passing argument ARG0, and returns the
   return value as an `int'. */
#define syscall1(NUMBER, ARG0)                                           \
        ({                                                               \
          int retval;                                                    \
          asm volatile                                                   \
            ("pushl %[arg0]; pushl %[number]; int $0x30; addl $8, %%esp" \
               : "=a" (retval)                                           \
               : [number] "i" (NUMBER),                                  \
                 [arg0] "g" (ARG0)                                       \
               : "memory");                                              \
          retval;                                                        \
        })

/* Invokes syscall NUMBER, passing arguments ARG0 and ARG1, and
   returns the return value as an `int'. */
#define syscall2(NUMBER, ARG0, ARG1)                            \
        ({                                                      \
          int retval;                                           \
          asm volatile                                          \
            ("pushl %[arg1]; pushl %[arg0]; "                   \
             "pushl %[number]; int $0x30; addl $12, %%esp"      \
               : "=a" (retval)                                  \
               : [number] "i" (NUMBER),                         \
                 [arg0] "g" (ARG0),                             \
                 [arg1] "g" (ARG1)                              \
               : "memory");                                     \
          retval;                                               \
        })

/* Invokes syscall NUMBER, passing arguments ARG0, ARG1, and
   ARG2, and returns the return value as an `int'. */
#define syscall3(NUMBER, ARG0, ARG1, ARG2)                      \
        ({                                                      \
          int retval;                                           \
          asm volatile                                          \
            ("pushl %[arg2]; pushl %[arg1]; pushl %[arg0]; "    \
             "pushl %[number]; int $0x30; addl $16, %%esp"      \
               : "=a" (retval)                                  \
               : [number] "i" (NUMBER),                         \
                 [arg0] "g" (ARG0),                             \
                 [arg1] "g" (ARG1),                             \
                 [arg2] "g" (ARG2)                              \
               : "memory");                                     \
          retval;                                               \
        })

#define syscall4(NUMBER, ARG0, ARG1, ARG2, ARG3)			\
        ({                                                      \
          int retval;                                           \
          asm volatile                                          \
            ("pushl %[arg3]; pushl %[arg2]; pushl %[arg1]; pushl %[arg0]; "    \
             "pushl %[number]; int $0x30; addl $20, %%esp"      \
               : "=a" (retval)                                  \
               : [number] "i" (NUMBER),                         \
                 [arg0] "g" (ARG0),                             \
                 [arg1] "g" (ARG1),                             \
		 [arg2] "g" (ARG2), [arg3] "g" (ARG3)		\
               : "memory");                                     \
          retval;                                               \
        })

/********** Message Queue Implementation ************/
mqd_t mq_open(const char *name, int oflag)
{
  return syscall2 (SYS_MQ_OPEN, name, oflag);
}

int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, 
	    unsigned msg_prio)
{
  return syscall4 (SYS_MQ_SEND, mqdes, msg_ptr, msg_len, msg_prio);
}

size_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, 
		  unsigned *msg_prio)
{
  return syscall4 (SYS_MQ_RECEIVE, mqdes, msg_ptr, msg_len, msg_prio);
}

int mq_close(mqd_t mqdes)
{
  return syscall1 (SYS_MQ_CLOSE, mqdes);
}

int mq_unlink(const char *name)
{
  return syscall1 (SYS_MQ_UNLINK, name);
}
/****************************************************/

/********** Pthread implementation ******************/
int
pthread_create (pthread_t *thread, 
		const pthread_attr_t *attr, 
		void (*start_routine) (void *), void *ar)
{
  return syscall4 (SYS_PTHREADS_CREATE, thread, attr, start_routine, ar);
}

void
pthread_exit (void *value_ptr)
{
  syscall1(SYS_PTHREADS_EXIT, value_ptr);
}

int
pthread_join (pthread_t thread, void **retval)
{
  return syscall2(SYS_PTHREADS_JOIN, thread, retval);
}
/****************************************************/

void
halt (void) 
{
  syscall0 (SYS_HALT);
  NOT_REACHED ();
}

void
exit (int status)
{
  syscall1 (SYS_EXIT, status);
  NOT_REACHED ();
}

pid_t
exec (const char *file)
{
  return (pid_t) syscall1 (SYS_EXEC, file);
}

int
wait (pid_t pid)
{
  return syscall1 (SYS_WAIT, pid);
}

bool
create (const char *file, unsigned initial_size)
{
  return syscall2 (SYS_CREATE, file, initial_size);
}

bool
remove (const char *file)
{
  return syscall1 (SYS_REMOVE, file);
}

int
open (const char *file)
{
  return syscall1 (SYS_OPEN, file);
}

int
filesize (int fd) 
{
  return syscall1 (SYS_FILESIZE, fd);
}

int
read (int fd, void *buffer, unsigned size)
{
  return syscall3 (SYS_READ, fd, buffer, size);
}

int
write (int fd, const void *buffer, unsigned size)
{
  return syscall3 (SYS_WRITE, fd, buffer, size);
}

void
seek (int fd, unsigned position) 
{
  syscall2 (SYS_SEEK, fd, position);
}

unsigned
tell (int fd) 
{
  return syscall1 (SYS_TELL, fd);
}

void
close (int fd)
{
  syscall1 (SYS_CLOSE, fd);
}

mapid_t
mmap (int fd, void *addr)
{
  return syscall2 (SYS_MMAP, fd, addr);
}

void
munmap (mapid_t mapid)
{
  syscall1 (SYS_MUNMAP, mapid);
}

bool
chdir (const char *dir)
{
  return syscall1 (SYS_CHDIR, dir);
}

bool
mkdir (const char *dir)
{
  return syscall1 (SYS_MKDIR, dir);
}

bool
readdir (int fd, char name[READDIR_MAX_LEN + 1]) 
{
  return syscall2 (SYS_READDIR, fd, name);
}

bool
isdir (int fd) 
{
  return syscall1 (SYS_ISDIR, fd);
}

int
inumber (int fd) 
{
  return syscall1 (SYS_INUMBER, fd);
}
