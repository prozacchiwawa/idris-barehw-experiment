#include "boot.h"
#include "con32.h"
#include "multiboot.h"

con32 c;

const char *flags[] = {
    "mem",
    "bootdev",
    "cmdline",
    "mods",
    "syms1",
    "syms2",
    "mmap",
    "drives",
    "config",
    "bootldr",
    "apm",
    "vbe",
    "framebuf"
};

void halt(const char *res) {
    write_con_str(&c, res);
    write_con_str(&c, "\r\n");
    while (1) {}
}

int main(int argc, char **argv) {
    init_con(&c);
    write_con_str(&c, "init ");
    write_con_int(&c, argc);
    write_con_str(&c, " argv ");
    write_con_ptr(&c, argv);
    write_con_str(&c, "\r\n");

    mb_root *bootinfo = (mb_root*)argc;
    write_con_str(&c, "flags: ");
    for (int i = 0; i < 12; i++) {
        if (i) { write_con_str(&c, " "); }
        write_con_str(&c, flags[i]);
    }
    write_con_str(&c, "\r\n");
    if (bootinfo->flags & BOOTLDR_FLAG) {
        write_con_str(&c, "booter ");
        write_con_str(&c, (const char *)bootinfo->boot_loader_name);
        write_con_str(&c, "\r\n");
    }
    if (bootinfo->flags & CMDLINE_FLAG) {
        write_con_str(&c, "cmdline ");
        write_con_str(&c, (const char *)bootinfo->cmdline);
        write_con_str(&c, "\r\n");
    }
    
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

    if (bootinfo->flags & MODS_FLAG) {
        mb_mod *mods = (mb_mod*)bootinfo->mods_addr;
        for (int i = 0; i < bootinfo->mods_count; i++) {
            write_con_str(&c, "mod ");
            write_con_ptr(&c, mods[i].mod_start);
            write_con_str(&c, "-");
            write_con_ptr(&c, mods[i].mod_end);
            write_con_str(&c, " -- ");
            write_con_str(&c, (const char *)mods[i].mod_args);
            write_con_str(&c, "\r\n");
        }
    }
    
    halt("go64");
    return 0;
}
