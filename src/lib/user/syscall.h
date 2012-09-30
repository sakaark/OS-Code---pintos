#ifndef __LIB_USER_SYSCALL_H
#define __LIB_USER_SYSCALL_H

#include <stdbool.h>
#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include "userprog/syscall.h"

/***for message queues****/
typedef long long int mqd_t;
/*************************/

/* Process identifier. */
typedef int pid_t;
#define PID_ERROR ((pid_t) -1)

/* Map region identifier. */
typedef int mapid_t;
#define MAP_FAILED ((mapid_t) -1)

/* Maximum characters in a filename written by readdir(). */
#define READDIR_MAX_LEN 14

/* Typical return values from main() and arguments to exit(). */
#define EXIT_SUCCESS 0          /* Successful execution. */
#define EXIT_FAILURE 1          /* Unsuccessful execution. */

/***** Message queues implementation ****/
mqd_t mq_open(const char *name, int oflag);
int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, 
		unsigned msg_prio);
size_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, 
		      unsigned *msg_prio);
int mq_close(mqd_t mqdes); 
int mq_unlink(const char *name);

/****************************************/

/****** Pthreads implementation *********/
int
pthread_create (pthread_t *thread, 
		const pthread_attr_t *attr, 
		void (*start_routine) (void *), void *arg);

void pthread_exit (void *value_ptr);

int pthread_join(pthread_t thread, void **retval);
/****************************************/

/* Projects 2 and later. */
void halt (void) NO_RETURN;
void exit (int status) NO_RETURN;
pid_t exec (const char *file);
int wait (pid_t);
bool create (const char *file, unsigned initial_size);
bool remove (const char *file);
int open (const char *file);
int filesize (int fd);
int read (int fd, void *buffer, unsigned length);
int write (int fd, const void *buffer, unsigned length);
void seek (int fd, unsigned position);
unsigned tell (int fd);
void close (int fd);

/* Project 3 and optionally project 4. */
mapid_t mmap (int fd, void *addr);
void munmap (mapid_t);

/* Project 4 only. */
bool chdir (const char *dir);
bool mkdir (const char *dir);
bool readdir (int fd, char name[READDIR_MAX_LEN + 1]);
bool isdir (int fd);
int inumber (int fd);

#endif /* lib/user/syscall.h */
