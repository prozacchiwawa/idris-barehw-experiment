#include <stdio.h>
#include <sys/errno.h>

FILE *popen(const char *cmd, const char *type) {
    errno = ENOENT;
    return NULL;
}
