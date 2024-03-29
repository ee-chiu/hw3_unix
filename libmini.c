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

void exit(int error_code){
    sys_exit(error_code);
}

int sigaction(int sig, struct sigaction *act, struct sigaction *oldact){
    act->sa_flags |= SA_RESTORER;
    act->sa_restorer = sys_rt_sigreturn;
    long ret = sys_rt_sigaction(sig, act, oldact, sizeof(sigset_t));
    WRAPPER_RETval(int);
}

int sigismember(const sigset_t *set, int sig){
    sigset_t checksig = ( 1 << (sig - 1) );
    return *set & checksig;
}

int sigaddset(sigset_t *set, int sig){
    sigset_t addsig = ( 1 << (sig - 1) );
    *set = *set | addsig;
    return 0;
}

int sigdelset (sigset_t *set, int sig){
    sigset_t delsig = ( 1 << (sig-1) );
    *set = *set & (~delsig);
    return 0;
}

int sigemptyset(sigset_t *set){
    *set = 0;
    return 0;
}

int sigfillset(sigset_t *set){
    sigset_t addsig = 1;
    for(int i = 1; i <= 31; i++){
        *set |= addsig;
        addsig = addsig << 1;
    }
    return 0;
}

int sigpending(sigset_t *set){
    long ret = sys_rt_sigpending(set, sizeof(sigset_t));
    WRAPPER_RETval(int);
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset){
    long ret = sys_rt_sigprocmask(how, set, oldset, sizeof(sigset_t));
    WRAPPER_RETval(int);
}

sighandler_t signal(int sig, sighandler_t handler){
    struct sigaction act, oldact;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    if (sigaction(sig, &act, &oldact) < 0) return SIG_ERR;
    return oldact.sa_handler;      
}

int getsigmask(sigset_t *set){
    long ret = sigprocmask(0, NULL, set);
    WRAPPER_RETval(int);
}

int setsigmask(sigset_t *set){
    long ret = sigprocmask(SIG_SETMASK, set, NULL);
    WRAPPER_RETval(int);
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