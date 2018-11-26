#include "boot.h"
#include "con32.h"

con32 c;

void halt(const char *res) {
    write_con_str(&c, res);
    while (1) {}
}

int main(int argc, char **argv) {
    init_con(&c);
    write_con_str(&c, "init ");
    write_con_int(&c, argc);
    write_con_str(&c, " argv ");
    write_con_ptr(&c, argv);
    
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
