#include <sys/types.h>
#include <sys/errno.h>

void _exit(int code) {
    while(1) { }
}

int kill(int pid, int signal) {
    errno = ESRCH;
    return -1;
}

int getpid() {
    return 1;
}
