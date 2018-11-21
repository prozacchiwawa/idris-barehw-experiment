#include <sys/time.h>
#include <sys/errno.h>

int nanosleep(const struct timespec *req, struct timespec *rem) {
    *rem = *req;
    return -1;
}
