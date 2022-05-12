#ifndef __LIBMINI_H__
#define __LIBMINI_H__

typedef long long size_t;
typedef long long ssize_t;

extern long errno;

#define NULL ((void*) 0)

/* system calls */
long sys_write(int fd, const void *buf, size_t count);

/* wrappers */
ssize_t write(int fd, const void *buf, size_t count);

size_t strlen(const char *s);

#endif