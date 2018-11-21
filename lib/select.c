#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>

int select(int maxfd, fd_set *reads, fd_set *writes, fd_set *errs, struct timeval *timeouts) {
    return -1;
}
