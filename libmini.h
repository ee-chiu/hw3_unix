#ifndef __LIBMINI_H__
#define __LIBMINI_H__

typedef long long size_t;
typedef long long ssize_t;
typedef unsigned long sigset_t; /* from /usr/include/x86_64-linux-gnu/asm/signal.h */
typedef char* sighandler_t; /* from https://elixir.bootlin.com/linux/v4.16.8/source/arch/parisc/include/uapi/asm/signal.h#L94 line 94 */

extern long errno;

#define NULL    ((void*) 0)

/* from /usr/include/asm-generic/errno-base.h */
#define	EPERM		 1	/* Operation not permitted */
#define	ENOENT		 2	/* No such file or directory */
#define	ESRCH		 3	/* No such process */
#define	EINTR		 4	/* Interrupted system call */
#define	EIO		 5	/* I/O error */
#define	ENXIO		 6	/* No such device or address */
#define	E2BIG		 7	/* Argument list too long */
#define	ENOEXEC		 8	/* Exec format error */
#define	EBADF		 9	/* Bad file number */
#define	ECHILD		10	/* No child processes */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EACCES		13	/* Permission denied */
#define	EFAULT		14	/* Bad address */
#define	ENOTBLK		15	/* Block device required */
#define	EBUSY		16	/* Device or resource busy */
#define	EEXIST		17	/* File exists */
#define	EXDEV		18	/* Cross-device link */
#define	ENODEV		19	/* No such device */
#define	ENOTDIR		20	/* Not a directory */
#define	EISDIR		21	/* Is a directory */
#define	EINVAL		22	/* Invalid argument */
#define	ENFILE		23	/* File table overflow */
#define	EMFILE		24	/* Too many open files */
#define	ENOTTY		25	/* Not a typewriter */
#define	ETXTBSY		26	/* Text file busy */
#define	EFBIG		27	/* File too large */
#define	ENOSPC		28	/* No space left on device */
#define	ESPIPE		29	/* Illegal seek */
#define	EROFS		30	/* Read-only file system */
#define	EMLINK		31	/* Too many links */
#define	EPIPE		32	/* Broken pipe */
#define	EDOM		33	/* Math argument out of domain of func */
#define	ERANGE		34	/* Math result not representable */

/* from /usr/include/x86_64-linux-gnu/asm/signal.h */
#define NSIG    32

#define SIGHUP     1
#define SIGINT     2
#define SIGQUIT    3    
#define SIGILL     4
#define SIGTRAP    5 
#define SIGABRT    6 
#define SIGIOT     6 
#define SIGBUS     7
#define SIGFPE     8
#define SIGKILL    9
#define SIGUSR1    10
#define SIGSEGV    11
#define SIGUSR2    12
#define SIGPIPE    13
#define SIGALRM    14
#define SIGTERM    15
#define SIGSTKFLT  16
#define SIGCHLD    17
#define SIGCONT    18
#define SIGSTOP    19
#define SIGTSTP    20
#define SIGTTIN    21
#define SIGTTOU    22
#define SIGURG     23
#define SIGXCPU    24
#define SIGXFSZ    25
#define SIGVTALRM  26
#define SIGPROF    27
#define SIGWINCH   28
#define SIGIO      29
#define SIGPWR     30
#define SIGSYS     31

/* from https://elixir.bootlin.com/linux/v4.16.8/source/arch/parisc/include/uapi/asm/signal.h#L79 */
#define SIG_BLOCK   0
#define SIG_UNBLOCK 1
#define SIG_SETMASK 2
#define SIG_DFL     ((sighandler_t)0)
#define SIG_IGN     ((sighandler_t)1)
#define SIG_ERR     ((sighandler_t)-1)

/* from libmini.h in sample code */
struct timespec {
    long tv_sec;    /* seconds */
    long tv_nsec;   /* nanoseconds */
};

/* slide 10-signals p.41 */
struct sigaction {
    void    (*sa_handler)(int);
    void    (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t    sa_mask;
    int     sa_flags;
    void    (*sa_restorer)(void);
}

/* system calls */
long sys_write(int fd, const void *buf, size_t count);
long sys_alarm(unsigned int sec);
long sys_pause(void);
long sys_nanosleep(struct timespec *rqtp, struct timespec *rmtp);

/* wrappers */
ssize_t write(int fd, const void *buf, size_t count);
size_t strlen(const char *s);
unsigned int alarm(unsigned int sec);
int pause(void);
unsigned int sleep(unsigned int sec);
int sigaddset(sigset_t *set, int sig);
int sigemptyset(sigset_t *set);
#endif