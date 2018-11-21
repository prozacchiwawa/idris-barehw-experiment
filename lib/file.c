#include <sys/types.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <stdio.h>

int open(const char *name, int access, ...) {
    errno = ENOENT;
    return -1;
}

int write(int fd, const char *data, size_t len) {
    errno = EBUSY;
    return -1;
}

int close(int fd) {
    return 0;
}

int isatty(int fd) {
    return 0;
}

int lseek(int fd, off_t offset, int whence) {
    return 0;
}

int read(int fd, char *data, size_t len) {
    return 0;
}
