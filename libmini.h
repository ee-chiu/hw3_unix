#ifndef __LIBMINI_H__
#define __LIBMINI_H__

typedef long long size_t;
typedef long long ssize_t;

extern long errno;

#define NULL ((void*) 0)

/* system calls */
long sys_write(int fd, const void *buf, size_t count);
long sys_alarm(unsigned int sec);
long sys_pause(void);

/* wrappers */
ssize_t write(int fd, const void *buf, size_t count);
size_t strlen(const char *s);
unsigned int alarm(unsigned int sec);
int pause(void);

#endif