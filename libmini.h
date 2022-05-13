#ifndef __LIBMINI_H__
#define __LIBMINI_H__

typedef long long size_t;
typedef long long ssize_t;
typedef int pid_t;
typedef int uid_t;
typedef long clock_t; /* from https://elixir.bootlin.com/linux/v4.16.8/source/include/linux/types.h#L75 */
typedef unsigned long sigset_t; /* from /usr/include/x86_64-linux-gnu/asm/signal.h */
//typedef char* sighandler_t; /* from https://elixir.bootlin.com/linux/v4.16.8/source/arch/parisc/include/uapi/asm/signal.h#L94 line 94 */
typedef void (*sighandler_t)(int); /* from man signal */

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

/* from /usr/include/asm-generic/siginfo.h */
typedef union sigval {
    int sival_int;
    void *sival_ptrl;
} sigval_t;

/* from man sigaction */
typedef struct {
    int      si_signo;     /* Signal number */
    int      si_errno;     /* An errno value */
    int      si_code;      /* Signal code */
    int      si_trapno;    /* Trap number that caused
                                hardware-generated signal
                                (unused on most architectures) */
    pid_t    si_pid;       /* Sending process ID */
    uid_t    si_uid;       /* Real user ID of sending process */
    int      si_status;    /* Exit value or signal */
    clock_t  si_utime;     /* User time consumed */
    clock_t  si_stime;     /* System time consumed */
    union sigval si_value; /* Signal value */
    int      si_int;       /* POSIX.1b signal */
    void    *si_ptr;       /* POSIX.1b signal */
    int      si_overrun;   /* Timer overrun count;
                                POSIX.1b timers */
    int      si_timerid;   /* Timer ID; POSIX.1b timers */
    void    *si_addr;      /* Memory location which caused fault */
    long     si_band;      /* Band event (was int in
                                glibc 2.3.2 and earlier) */
    int      si_fd;        /* File descriptor */
    short    si_addr_lsb;  /* Least significant bit of address
                                (since Linux 2.6.32) */
    void    *si_lower;     /* Lower bound when address violation
                                occurred (since Linux 3.19) */
    void    *si_upper;     /* Upper bound when address violation
                                occurred (since Linux 3.19) */
    int      si_pkey;      /* Protection key on PTE that caused
                                fault (since Linux 4.6) */
    void    *si_call_addr; /* Address of system call instruction
                                (since Linux 3.5) */
    int      si_syscall;   /* Number of attempted system call
                                (since Linux 3.5) */
    unsigned int si_arch;  /* Architecture of attempted system call
                                (since Linux 3.5) */
} siginfo_t;

/* slide 10-signals p.41 */
struct sigaction {
    void    (*sa_handler)(int);
    void    (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t    sa_mask;
    int     sa_flags;
    void    (*sa_restorer)(void);
};

/* system calls */
long sys_write(int fd, const void *buf, size_t count);
long sys_alarm(unsigned int sec);
long sys_pause(void);
long sys_nanosleep(struct timespec *rqtp, struct timespec *rmtp);
long sys_rt_sigaction(int sig, const struct sigaction *act, struct sigaction *oldact, size_t sigsetsize);
long sys_rt_sigpending(sigset_t *set, size_t sigsetsize);
long sys_rt_sigprocmask(int how, const sigset_t *set, sigset_t *oldset, size_t sigsetsize);

/* wrappers */
ssize_t write(int fd, const void *buf, size_t count);
size_t strlen(const char *s);
unsigned int alarm(unsigned int sec);
int pause(void);
unsigned int sleep(unsigned int sec);
int sigaction(int sig, const struct sigaction *act, struct sigaction *oldact);
int sigismember(const sigset_t *set, int sig);
int sigaddset(sigset_t *set, int sig);
int sigemptyset(sigset_t *set);
int sigpending(sigset_t *set);
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
sighandler_t signal(int sig, sighandler_t handler);

void perror(const char *prefix);

#endif