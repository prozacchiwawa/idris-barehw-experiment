#include <sys/types.h>
#include <sys/errno.h>

typedef struct _DIR DIR;

DIR *opendir(const char *name) {
    return NULL;
}

struct dirent *readdir(DIR *dir) {
    return NULL;
}

int closedir(DIR *dir) {
    return 0;
}

int mkdir(const char *name) {
    errno = ENOENT;
    return -1;
}

int chdir(const char *name) {
    errno = ENOENT;
    return -1;
}

char *getcwd(char *buffer, size_t len) {
    errno = ENOENT;
    return "/";
}
