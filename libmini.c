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

int sigaction(int sig, const struct sigaction *act, struct sigaction *oldact){
    if (act == NULL || oldact == 0 || sig <= 0 || sig >= NSIG || sig == SIGSTOP || sig == SIGKILL) {
        errno = EINVAL;
        return -1;
    }

    long ret = sys_rt_sigaction(sig, act, oldact, sizeof(sigset_t));
    WRAPPER_RETval(int);
}

int sigaddset(sigset_t *set, int sig){
    if (set == NULL || sig <= 0 || sig >= NSIG) {
        errno = EINVAL;
        return -1;
    }

    sigset_t addsig = ( 1 << (sig - 1) );
    *set = *set & addsig;
    return 0;
}

int sigemptyset(sigset_t *set){
    if (set == NULL){
        errno = EINVAL;
        return -1;
    }

    memset(set, 0, sizeof(sigset_t));
    return 0;
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset){
    if(set == NULL || oldset == NULL) {
        errno = EINVAL;
        return -1;
    }

    sys_rt_sigprocmask(how, set, oldset, sizeof(sigset_t));
    return 0;
}