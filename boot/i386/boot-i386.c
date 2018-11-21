#include "boot.h"

const int ERRTEXTATTR = 0x1b00;

void halt(const char *res) {
    const char *ptr = res;
    unsigned short *outptr = ((unsigned short*)0xb8000);
    while (*ptr) {
        *outptr++ = *ptr | ERRTEXTATTR;
        ptr++;
    }
    while (1) {}
}

int main(int argc, char **argv) {
    int has_cpuid_ = has_cpuid(0);
    if (!has_cpuid_) {
        halt("No CPUID");
    }
    int has_extfun_ = has_extfun(0);
    if (!has_extfun_) {
        halt("No EXTFUN");
    }
    int has_longmode_ = has_longmode(0);
    if (!has_longmode_) {
        halt("No LONGMODE");
    }

    halt("go64");
    return 0;
}
