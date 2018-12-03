#ifndef _BOOT_H_
#define _BOOT_H_

#include "boottypes.h"

extern "C" {
    int has_cpuid(int foo);
    int has_extfun(int foo);
    int has_longmode(int foo);

    int main(int argc, char **argv);
    
    extern uint32_t _start;
    extern uint32_t _end;
}

#endif//_BOOT_H_
