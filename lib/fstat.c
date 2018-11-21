#include <sys/stat.h>
#include <sys/errno.h>

int fstat(int fd, struct stat *buf) {
    errno = ENOENT;
    return -1;
}
