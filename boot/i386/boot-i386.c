#include "boot.h"
#include "con32.h"
#include "multiboot.h"
#include "elf.h"
#include "longmode.h"

con32 c;
volatile int continue_64 = 0;

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

#define MMAP_MAX 128

int main(int argc, char **argv) {
    enable_sse();
    
    int mmap_current = 0;
    mb_mmap mmap_entries[MMAP_MAX] = { };

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

    int total_pages = 0;
    if (bootinfo->flags & MEM_FLAG) {
        // copy out hopefull enough entries to use.
        mb_mmap *mmap = getFirstMmapEntry(bootinfo);
        while (mmap && mmap_current < MMAP_MAX) {
            write_con_str(&c, "mmap @");
            write_con_ptr(&c, mmap);
            write_con_str(&c, "(");
            write_con_int(&c, mmap->mmap_type);
            write_con_str(&c, ",");
            write_con_int(&c, mmap->mmap_addr);
            write_con_str(&c, ",");
            write_con_int(&c, mmap->mmap_len);
            write_con_str(&c, ")\r\n");
            total_pages += mmap->mmap_len >> 12;
            mmap_entries[mmap_current++] = *mmap;
            mmap = getNextMmapEntry(bootinfo, mmap);
        }
    }

    write_con_str(&c, "memory pages: ");
    write_con_int(&c, total_pages);
    write_con_str(&c, "\r\n");

    write_con_str(&c, "go64 ");
    write_con_ptr(&c, go64);
    write_con_str(&c, "\r\n");

    write_con_str(&c, "&continue_64 ");
    write_con_ptr(&c, &continue_64);
    write_con_str(&c, "\r\n");

    mod_64 mod64 = { };
    mod64.entry[0] = 8;

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
            Elf64_Ehdr *ehdr = (Elf64_Ehdr*)mods[i].mod_start;

            write_con_str(&c, "ehdr @");
            write_con_ptr(&c, ehdr);
            write_con_str(&c, "\r\n");

            if (ehdr->e_type != ET_EXEC) {
                halt("Kernel not executable");
            }
            
            uint64_t entryPointVaddr = ehdr->e_entry;
            write_con_str(&c, "#phdr ");
            write_con_int(&c, ehdr->e_phnum);
            write_con_str(&c, "\r\n");

            for (int j = 0; j < ehdr->e_phnum; j++) {
                uint32_t phdrAddr = mods[i].mod_start + ehdr->e_phoff + ehdr->e_phentsize * j;
                write_con_str(&c, "phdr @");
                Elf64_Phdr *phdr = (Elf64_Phdr*)phdrAddr;
                write_con_ptr(&c, phdr);
                write_con_str(&c, "\r\n");
                if (0) {
                    write_con_str(&c, "phdr ");
                    write_con_int(&c, phdr->p_type);
                    write_con_str(&c, " vaddr ");
                    write_con_int(&c, phdr->p_vaddr);
                    write_con_str(&c, " memsz ");
                    write_con_int(&c, phdr->p_memsz);
                    write_con_str(&c, "\r\n");
                    if (phdr->p_vaddr <= entryPointVaddr &&
                        phdr->p_vaddr + phdr->p_memsz > entryPointVaddr &&
                        phdr->p_type == PT_LOAD) {
                        mod64.pentry = 
                            mods[i].mod_start +
                            phdr->p_paddr + (entryPointVaddr - phdr->p_vaddr);
                        mod64.start = mods[i].mod_start;
                        mod64.length = mods[i].mod_end - mods[i].mod_start;
                        break;
                    }
                }
            }
        }
    }

    if (!mod64.start || !mod64.length) {
        halt("Could not find kernel module");
    }

    halt("no");
            
    int ct = 0;
    while (!continue_64) {
        ct++;
    }

    // Find a boot slab area
    for (int i = 0; i < mmap_current; i++) {
        if (mmap_entries[i].mmap_len >= mod64.length) {
            mod64.mstart = mmap_entries[i].mmap_addr;
            mod64.pentry = mod64.pentry - mod64.start + mod64.mstart;
            write_con_str(&c, "use memory at ");
            write_con_int(&c, mmap_entries[i].mmap_addr);
            write_con_str(&c, " start ");
            write_con_int(&c, mod64.pentry);
            write_con_str(&c, "\r\n");
            memcpy(&mod64.entry[2], &mod64.pentry, sizeof(mod64.pentry));
            memcpy((void*)mmap_entries[i].mmap_addr, mod64.start, mod64.length);
        }
    }
    
    if (!mod64.start) {
        halt("Could not find entry point in any PT_LOAD program header");
    }

    go64(&mod64);
    write_con_str(&c, "Didn't boot\r\n");
    return 0;
}
