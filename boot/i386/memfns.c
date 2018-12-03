#include "memfns.h"

void *memset(void *target, int fill, size_t len) {
    char *res = (char *)target;
    char *tgt = (char *)target;
    while (len > 0) {
        *tgt++ = fill;
        len--;
    }
    return target;
}

void *memcpy(void *target, void *source, size_t len) {
    char *tgt = (char *)target;
    char *src = (char *)source;
    while (len > 0) {
        *tgt++ = *src++;
        len--;
    }
    return target;
}
