#ifndef _MEMFNS_H_
#define _MEMFNS_H_

#include "boottypes.h"

extern "C" {
void *memset(void *target, int fill, size_t len);
void *memcpy(void *taget, void *src, size_t len);
}

#endif//_MEMFNS_H_
