#ifndef _LONGMODE_H_
#define _LONGMODE_H_

typedef struct _mod_64 {
    uint32_t pentry;
    uint32_t start;
    uint32_t length;
    uint32_t mstart;
    uint64_t pml4;
    uint64_t alloc_bitmap;
    char entry[10];
} mod_64;

extern "C" void __fastcall go64(mod_64 *mod64);

#endif//_LONGMODE_H_
