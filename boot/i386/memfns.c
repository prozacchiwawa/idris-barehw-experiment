#include "memfns.h"

void *memset(void *target, int fill, size_t len) {
    char *res = target;
    char *tgt = (char *)target;
    while (len > 0) {
        *tgt++ = fill;
        len--;
    }
    return target;
}

void *memcpy(void *target, void *source, size_t len) {
    char *tgt = target;
    char *src = source;
    while (len > 0) {
        *tgt++ = *src++;
        len--;
    }
    return target;
}
