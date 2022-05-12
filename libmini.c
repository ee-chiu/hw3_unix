#include "libmini.h"

long errno;

#define WRAPPER_RETval(type)    errno = 0; if(ret < 0) { errno = -ret; return -1; } return((type) ret);

ssize_t write(int fd, const void *buf, size_t count){
    long ret = sys_write(fd, buf, count);
    WRAPPER_RETval(ssize_t);
}

size_t strlen(const char *s){
    size_t count = 0;
    while(*s++) count++;
    return count;
}

unsigned int alarm(unsigned int sec){
    long ret = sys_alarm(sec);
    WRAPPER_RETval(unsigned);
}

int pause(void){
    long ret = sys_pause();
    WRAPPER_RETval(int);
}

unsigned int sleep(unsigned int sec){
    long ret;
    struct timespec req, rem;
    req.tv_sec = sec;
    req.tv_nsec = 0;
    ret = sys_nanosleep(&req, &rem);
    if(ret >= 0) return ret;
    if(ret == -EINTR) {
        return rem.tv_sec;
    } 
    return 0;
}