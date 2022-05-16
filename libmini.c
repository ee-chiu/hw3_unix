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

int sigaction(int sig, struct sigaction *act, struct sigaction *oldact){
    if (sig <= 0 || sig >= NSIG || sig == SIGSTOP || sig == SIGKILL) {
        errno = EINVAL;
        return -1;
    }

    act->sa_flags |= SA_RESTORER;
    act->sa_restorer = sys_rt_sigreturn;
    long ret = sys_rt_sigaction(sig, act, oldact, sizeof(sigset_t));
    WRAPPER_RETval(int);
}

int sigismember(const sigset_t *set, int sig){
    if (set == NULL || sig <= 0 || sig >= NSIG) {
        errno = EINVAL;
        return 0;
    }

    sigset_t checksig = ( 1 << (sig - 1) );
    return *set & checksig;
}

int sigaddset(sigset_t *set, int sig){
    if (set == NULL || sig <= 0 || sig >= NSIG) {
        errno = EINVAL;
        return -1;
    }

    sigset_t addsig = ( 1 << (sig - 1) );
    *set = *set | addsig;
    return 0;
}

int sigemptyset(sigset_t *set){
    if (set == NULL){
        errno = EINVAL;
        return -1;
    }

    *set = 0;
    return 0;
}

int sigpending(sigset_t *set){
    if(set == NULL){
        errno = EINVAL;
        return -1;
    }

    long ret = sys_rt_sigpending(set, sizeof(sigset_t));
    WRAPPER_RETval(int);
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset){
    if(set == NULL) {
        errno = EINVAL;
        return -1;
    }

    if(oldset != NULL)  *oldset = *set;

    long ret = sys_rt_sigprocmask(how, set, oldset, sizeof(sigset_t));
    WRAPPER_RETval(int);
}

sighandler_t signal(int sig, sighandler_t handler){
    if(sig <= 0 || sig >= NSIG) {
        errno = EINVAL;
        return SIG_ERR; 
    }
    struct sigaction act, oldact;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, sig);
    act.sa_flags = 0;
    act.sa_restorer = NULL;
    if (sig == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    }
    else {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    } 
    if (sigaction(sig, &act, &oldact) < 0) return SIG_ERR;
    return oldact.sa_handler;      
}

static const char *errmsg[] = {
	"Success",
	"Operation not permitted",
	"No such file or directory",
	"No such process",
	"Interrupted system call",
	"I/O error",
	"No such device or address",
	"Argument list too long",
	"Exec format error",
	"Bad file number",
	"No child processes",
	"Try again",
	"Out of memory",
	"Permission denied",
	"Bad address",
	"Block device required",
	"Device or resource busy",
	"File exists",
	"Cross-device link",
	"No such device",
	"Not a directory",
	"Is a directory",
	"Invalid argument",
	"File table overflow",
	"Too many open files",
	"Not a typewriter",
	"Text file busy",
	"File too large",
	"No space left on device",
	"Illegal seek",
	"Read-only file system",
	"Too many links",
	"Broken pipe",
	"Math argument out of domain of func",
	"Math result not representable"
};

void perror(const char *prefix){
	long backup = errno;
	if(prefix) {
		write(2, prefix, strlen(prefix));
		write(2, ": ", 2);
	}
	write(2, errmsg[backup], strlen(errmsg[backup]));
	write(2, "\n", 1);
    return;
}