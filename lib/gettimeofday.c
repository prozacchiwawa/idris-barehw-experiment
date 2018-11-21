#include <sys/types.h>
#include <sys/errno.h>

int gettimeofday(struct timeval *tv, const struct timezone *tz) {
    errno = EINVAL;
    return -1;
}
