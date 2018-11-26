#include "con32.h"

void memmove_con(unsigned short *target, unsigned short *src, int elts) {
    while (elts--) {
        *target++ = *src++;
    }
}

void init_con(con32 *c) {
    c->base = (void*)0xb8000;
    c->attr = 0x1b;
    c->r = c->c = 0;
    c->rows = 80;
    c->cols = 25;
}

void write_con(con32 *c, char ch) {
    unsigned short *bound = c->base + (c->rows * c->cols);
    if (ch == 13) {
        c->c = 0;
    } else if (ch == 10) {
        c->r++;
        while (c->r >= c->rows) {
            memmove_con(
                c->base + c->rows,
                c->base,
                (c->rows - 1) * c->cols
                );
            c->r--;
        }
    } else if (ch == 8) {
        while (c->c % 8) {
            c->c++;
            if (c->c == c->cols) {
                write_con(c, '\r');
                write_con(c, '\n');
            }
        }
    } else {
        c->base[c->r * c->cols + c->c] = (c->attr << 8) | ch;
        c->c++;
        if (c->c == c->cols) {
            write_con(c, '\r');
            write_con(c, '\n');
        }
    }
}

void write_con_str(con32 *c, const char *str) {
    while (*str) {
        write_con(c, *str);
        str++;
    }
}

const char *digits = "0123456789abcdef";
void write_con_int(con32 *c, long i) {
    char writebuf[100];
    int writeoff = 99;
    writebuf[writeoff] = 0;
    int once = 0;
    while (i || !once) {
        writebuf[--writeoff] = digits[i%16];
        i >>= 4;
        once = 1;
    }
    write_con_str(c, "0x");
    write_con_str(c, writebuf + writeoff);
}

void write_con_ptr(con32 *c, const void *p) {
    write_con_str(c, "$");
    write_con_int(c, (long)p);
}
