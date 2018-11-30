#ifndef _CON32_H_
#define _CON32_H_

#include "boottypes.h"

typedef struct _con32 {
    unsigned short *base;
    unsigned char attr;
    unsigned short r, c;
    unsigned short rows, cols;
} con32;

void init_con(con32 *c);
void write_con(con32 *c, char ch);
void write_con_str(con32 *c, const char *str);
void write_con_int(con32 *c, uint64_t i);
void write_con_ptr(con32 *c, const void *p);

#endif//_CON32_H_
