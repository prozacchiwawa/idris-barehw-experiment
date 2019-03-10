#include <sys/types.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <stdio.h>

uint16_t *screen_start = (uint16_t *)0xb8000;
uint16_t *screen = (uint16_t *)0xb8000;
uint16_t *screen_end = ((uint16_t *)0xb8000) + (80 * 25);

int open(const char *name, int access, ...) {
    errno = ENOENT;
    return -1;
}

void scroll() {
    memcpy(screen_start + 80, screen_start, (screen_end - screen_start - 80) * sizeof(uint16_t));
    screen -= 80;
}

void outchar(const char ch) {
    if (ch == 10) {
        int col = (screen - screen_start) % 80;
        screen += 80 - col;
    } else {
        *screen++ = 0x1a00 | (0xff & ch);
    }
    if (screen >= screen_end) {
        scroll();
    }
}

int write(int fd, const char *data, size_t len) {
    if (fd == 1 || fd == 2) {
        const char *end = data + len;
        while (data < end) {
            outchar(*data++);
        }
    }
    return len;
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
