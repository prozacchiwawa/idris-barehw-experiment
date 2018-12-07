#include "boot.h"
#include "sse.h"
#include "con32.h"
#include "multiboot.h"
#include "elf.h"
#include "longmode.h"
#include "memfns.h"

#define MEGABYTE (1024 * 1024)
#define APIC 0xfee00000
#define PAGE_SIZE 4096
#define KERNEL_ADDR (8 * MEGABYTE)

uint32_t min(uint32_t a, uint32_t b) {
    return a < b ? a : b;
}

uint32_t max(uint32_t a, uint32_t b) {
    return a > b ? a : b;
}

extern uint32_t _start;
extern uint32_t _end;

con32 c;
volatile int continue_64 = 1;

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

void set_alloc_page(uint64_t *bitmap_dir, uint64_t page) {
    int dir_idx = (int)(page / (PAGE_SIZE * 8));
    int dir_bit = (int)(page % (PAGE_SIZE * 8));
    int dir_byte = dir_bit / 8;
    dir_bit = dir_bit % 8;
    char *dir_page = (char *)bitmap_dir[dir_idx];
    dir_page[dir_byte] |= 1 << dir_bit;
}

uint64_t alloc_page(uint64_t *bitmap_dir) {
    int dir_idx = 0;
    uint64_t allocated = 0;

    while (bitmap_dir[dir_idx+1]) { dir_idx++; }

    while (!allocated) {
        uint8_t *dir_page_start = (uint8_t *)bitmap_dir[dir_idx];
        uint8_t *dir_page = dir_page_start + PAGE_SIZE - 1;

        while (dir_page >= dir_page_start && *dir_page == 0xff) { dir_page--; }
        if (dir_page < dir_page_start) {
            dir_idx--;
            continue;
        }
        
        int dir_byte = dir_page - dir_page_start;
        int dir_bit = 0;
        
        while (*dir_page & (1 << dir_bit)) { dir_bit++; }
        allocated = dir_idx * (PAGE_SIZE * 8) + (dir_byte * 8) + dir_bit;
        *dir_page |= (1 << dir_bit);
    }

    uint64_t result = allocated * PAGE_SIZE;
    memset((void*)result, 0, PAGE_SIZE);
    return result;
}

#define RWXKERNEL 3
#define PAGE_ADDR(x) ((x) & ~0xfff)
const uint64_t PML4_SHIFT = 12 + 9 + 9 + 9;
const uint64_t PML3_SHIFT = 12 + 9 + 9;
const uint64_t PML2_SHIFT = 12 + 9;
const uint64_t PML4_DIV = 1ll << PML4_SHIFT;
const uint64_t PML3_DIV = 1ll << PML3_SHIFT;
const uint64_t PML2_DIV = 1ll << PML2_SHIFT;

uint64_t map_page(uint64_t pml4, uint64_t *bitmap_dir, uint64_t addr) {
    uint64_t *pml4_ptr = (uint64_t*)pml4;
    int pml4_entry = (addr / PML4_DIV) % 512;
    if (!pml4_ptr[pml4_entry]) {
        pml4_ptr[pml4_entry] = alloc_page(bitmap_dir) | RWXKERNEL;
    }
    uint64_t *pml3_ptr = (uint64_t *)PAGE_ADDR(pml4_ptr[pml4_entry]);
    int pml3_entry = (addr / PML3_DIV) % 512;
    if (!pml3_ptr[pml3_entry]) {
        pml3_ptr[pml3_entry] = alloc_page(bitmap_dir) | RWXKERNEL;
    }
    uint64_t *pml2_ptr = (uint64_t *)PAGE_ADDR(pml3_ptr[pml3_entry]);
    int pml2_entry = (addr / PML2_DIV) % 512;
    if (!pml2_ptr[pml2_entry]) {
        pml2_ptr[pml2_entry] = alloc_page(bitmap_dir) | RWXKERNEL;
    }
    uint64_t *pml1_ptr = (uint64_t *)PAGE_ADDR(pml2_ptr[pml2_entry]);
    int pml1_entry = (addr / PAGE_SIZE) % 512;
    if (!pml1_ptr[pml1_entry]) {
        pml1_ptr[pml1_entry] = addr | RWXKERNEL;
    }
    return PAGE_ADDR(pml1_ptr[pml1_entry]);
}

uint64_t map_alloc_page(uint64_t pml4, uint64_t *bitmap_dir, uint64_t addr) {
    uint64_t *pml4_ptr = (uint64_t*)pml4;
    int pml4_entry = (addr / PML4_DIV) % 512;
    if (!pml4_ptr[pml4_entry]) {
        pml4_ptr[pml4_entry] = alloc_page(bitmap_dir) | RWXKERNEL;
    }
    uint64_t *pml3_ptr = (uint64_t *)PAGE_ADDR(pml4_ptr[pml4_entry]);
    int pml3_entry = (addr / PML3_DIV) % 512;
    if (!pml3_ptr[pml3_entry]) {
        pml3_ptr[pml3_entry] = alloc_page(bitmap_dir) | RWXKERNEL;
    }
    uint64_t *pml2_ptr = (uint64_t *)PAGE_ADDR(pml3_ptr[pml3_entry]);
    int pml2_entry = (addr / PML2_DIV) % 512;
    if (!pml2_ptr[pml2_entry]) {
        pml2_ptr[pml2_entry] = alloc_page(bitmap_dir) | RWXKERNEL;
    }
    uint64_t *pml1_ptr = (uint64_t *)PAGE_ADDR(pml2_ptr[pml2_entry]);
    int pml1_entry = (addr / PAGE_SIZE) % 512;
    if (!pml1_ptr[pml1_entry]) {
        pml1_ptr[pml1_entry] = alloc_page(bitmap_dir) | RWXKERNEL;
    }
    return PAGE_ADDR(pml1_ptr[pml1_entry]);
}

uint64_t find_bitmap_dir(const mb_mmap *mmap_entries, int mmap_current) {
    uint64_t max_page = 0x80000000 - PAGE_SIZE;
    uint64_t bitmap_dir_page = 0;
    
    for (int i = mmap_current - 1; i >= 0; i--) {
        uint64_t max_addr = mmap_entries[i].mmap_addr + mmap_entries[i].mmap_len;
        
        if (max_page < max_addr) {
            // Crawl the space, allocating
            uint64_t used_pages = 1;
            uint64_t use_page = min(max_page, max_addr - PAGE_SIZE);
            
            write_con(&c, "use_page ", use_page, "\r\n");
            bitmap_dir_page = use_page;
            break;
        }
    }
    return bitmap_dir_page;
}

// Find the mb_mmap with the maximum address below the given address
mb_mmap *find_space_under(mb_mmap *mmap_entries, int mmap_current, uint64_t last) {
    for (int i = mmap_current - 1; i >= 0; i--) {
        if (mmap_entries[i].mmap_addr <= last) {
            return mmap_entries + i;
        }
    }
    return nullptr;
}

uint64_t adjust_down_mmap(mb_mmap *entry, uint64_t use_page) {
    if (entry->mmap_addr + entry->mmap_len <= use_page) {
        return entry->mmap_addr + entry->mmap_len - PAGE_SIZE;
    } else {
        return use_page;
    }
}

uint64_t *setup_bitmap_dir(mb_mmap *mmap_entries, int mmap_current, uint64_t bitmap_dir_page, uint64_t bitmap_space) {
    uint64_t *bitmap_dir = (uint64_t *)bitmap_dir_page;
    uint64_t *bitmap_dir_fill = bitmap_dir;
    uint64_t used_pages = 0;
    uint64_t use_page = bitmap_dir_page - PAGE_SIZE;

    while (used_pages < bitmap_space + 1) {
        use_page -= PAGE_SIZE;
        mb_mmap *next_entry = find_space_under(mmap_entries, mmap_current, use_page);
        use_page = adjust_down_mmap(next_entry, use_page);
        if (!use_page) {
            halt("Not enough space for alloc bitmap");
        }
        *bitmap_dir_fill++ = use_page;
        used_pages++;
    }
    
    // We have all the pages we need, mark them in the bitmap
    set_alloc_page(bitmap_dir, bitmap_dir_page);
    for (int j = 0; j < used_pages-1; j++) {
        set_alloc_page(bitmap_dir, bitmap_dir[j]);
    }
    
    return (uint64_t *)bitmap_dir;
}


int main(int argc, char **argv) {
    enable_sse();
    
    int mmap_current = 0;
    mb_mmap mmap_entries[MMAP_MAX] = { };

    init_con(&c);
    write_con(&c, "init ", argc, " argv ", argv, "\r\n");

    mb_root *bootinfo = (mb_root*)argc;
    write_con(&c, "flags: ");
    for (int i = 0; i < 12; i++) {
        if (i) { write_con(&c, " "); }
        write_con(&c, flags[i]);
    }
    write_con(&c, "\r\n");
    
    if (bootinfo->flags & BOOTLDR_FLAG) {
        write_con
            (&c, "booter ", (const char *)bootinfo->boot_loader_name, "\r\n");
    }
    if (bootinfo->flags & CMDLINE_FLAG) {
        write_con(&c, "cmdline ", (const char *)bootinfo->cmdline, "\r\n");
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
        const mb_mmap *mmap = getFirstMmapEntry(bootinfo);
        while (mmap && mmap_current < MMAP_MAX) {
            write_con
                (&c,
                 "mmap @", mmap,
                 "(", mmap->mmap_type,
                 ",", mmap->mmap_addr, ",", mmap->mmap_len, ")\r\n");
            total_pages += mmap->mmap_len >> 12;
            if (mmap->mmap_type && mmap->mmap_addr && mmap->mmap_len) {
                mmap_entries[mmap_current++] = *mmap;
            }
            mmap = getNextMmapEntry(bootinfo, mmap);
        }
    }

    write_con(&c, "memory pages: ", total_pages, "\r\n");

    write_con(&c, "go64 ", (void*)go64, "\r\n");

    write_con(&c, "&continue_64 ", &continue_64, "\r\n");

    mod_64 mod64 = { };

    // We have all the ranges we need, start taking over...
    uint32_t start = (uint32_t)&_start;
    uint32_t end = (uint32_t)&_end;
    
    // Setup a page bitmap.
    // Need to represent entries:
    uint64_t max_address =
        mmap_entries[mmap_current - 1].mmap_addr +
        mmap_entries[mmap_current - 1].mmap_len;
    uint64_t page_entries = max_address / PAGE_SIZE;
    uint64_t bitmap_space = page_entries / (4096 * 8);
    
    // We need bitmap_space + 1 for a directory.
    // Bitmap space in the last pages before the apic.

    uint64_t bitmap_dir_page = find_bitmap_dir(mmap_entries, mmap_current);
    if (!bitmap_dir_page) {
        halt("failed to find a page for the bitmap dir");
    }

    write_con(&c, "bitmap_dir_page ", bitmap_dir_page, "\r\n");
    
    uint64_t *bitmap_dir =
        setup_bitmap_dir(
            mmap_entries, mmap_current, bitmap_dir_page, bitmap_space
            );
    if (!bitmap_dir) {
        halt("failed to setup bitmap directory");
    }

    write_con(&c, "bitmap_dir ", bitmap_dir, "\r\n");
    for (int i = 0; i < bitmap_space; i++) {
        write_con(&c, "bitmap ", i, " = ", bitmap_dir[i], "\r\n");
    }

    // Mark the first megabyte reserved
    for (uint32_t addr = 0; addr < MEGABYTE; addr += PAGE_SIZE) {
        set_alloc_page(bitmap_dir, addr);
    }
    
    // Mark the kernel space in the bitmap
    for (uint32_t kernel_addr = mod64.start;
         kernel_addr < mod64.start + mod64.length;
         kernel_addr += PAGE_SIZE) {
        set_alloc_page(bitmap_dir, kernel_addr);
    }
    
    // Mark the boot space
    for (uint32_t boot_addr = PAGE_ADDR(start);
         boot_addr < end;
         boot_addr += PAGE_SIZE) {
        set_alloc_page(bitmap_dir, boot_addr);
    }
    
    // We have a page dir root
    uint64_t pml4 = alloc_page(bitmap_dir);

    // Map the first Megabyte
    for (uint32_t addr = 0; addr < MEGABYTE; addr += PAGE_SIZE) {
        map_page(pml4, bitmap_dir, addr);
    }
        
    // Map the old boot program
    for (uint32_t boot_addr = PAGE_ADDR(start);
         boot_addr < end;
         boot_addr += PAGE_SIZE) {
        map_page(pml4, bitmap_dir, boot_addr);
    }

    if (!(bootinfo->flags & MODS_FLAG)) {
        halt("No mods");
    }

    mb_mod *mods = (mb_mod*)bootinfo->mods_addr;
    for (int i = 0; i < bootinfo->mods_count; i++) {
        write_con
            (&c,
             "mod ",
             mods[i].mod_start, "-", mods[i].mod_end,
             " -- ",
             (const char *)mods[i].mod_args,
             "\r\n");
        
        Elf64_Ehdr *ehdr = (Elf64_Ehdr*)mods[i].mod_start;
        
        write_con(&c, "ehdr @", ehdr, "\r\n");
        
        if (ehdr->e_type != ET_EXEC) {
            halt("Kernel not executable");
        }
        
        uint64_t entryPointVaddr = ehdr->e_entry;
        mod64.pentry = entryPointVaddr + KERNEL_ADDR;
        mod64.entry = mod64.pentry;
        mod64.entryseg = 8;
        
        write_con(&c, "#phdr ", ehdr->e_phnum, "\r\n");

        for (int j = 0; j < ehdr->e_phnum; j++) {
            uint32_t phdrAddr = mods[i].mod_start + ehdr->e_phoff + ehdr->e_phentsize * j;
            Elf64_Phdr *phdr = (Elf64_Phdr*)phdrAddr;
            write_con(&c, "phdr @", phdr, "\r\n");
            write_con
                (&c, "phdr ", phdr->p_type,
                 " vaddr ", phdr->p_vaddr, " memsz ", phdr->p_memsz, "\r\n");
            if (phdr->p_type == PT_LOAD) {
                uint64_t loadAddr = phdr->p_vaddr + KERNEL_ADDR;
                uint64_t modCopy = mods[i].mod_start + phdr->p_offset;
                uint64_t modEnd = modCopy + phdr->p_filesz;

                // Copy load segments into place.
                while (modCopy < modEnd) {
                    write_con(&c, "modCopy ", modCopy, " modEnd ", modEnd, "\r\n");
                    uint64_t copyToPage = map_alloc_page(pml4, bitmap_dir, loadAddr);
                    write_con(&c, "alloc ", copyToPage, " for ", loadAddr, "\r\n");
                    uint64_t copyBytes = min(modEnd - modCopy, PAGE_SIZE);
                    memcpy((void*)copyToPage, (void*)modCopy, copyBytes);
                    loadAddr += PAGE_SIZE;
                    modCopy += PAGE_SIZE;
                }
            }
        }
    }

    // We'll put 64k below kernel space for the stack
    mod64.stlen = 0x10000;
    mod64.stack = KERNEL_ADDR - mod64.stlen;

    for (uint64_t i = mod64.stack; i < mod64.stack + mod64.stlen; i += PAGE_SIZE) {
        map_alloc_page(pml4, bitmap_dir, i);
    }

    mod64.alloc_bitmap = bitmap_dir_page;
    mod64.pml4 = pml4;
    if (!mod64.pml4) {
        halt("no 64 bit pagedir");
    }

    int ct = 0;
    while (!continue_64) {
        ct++;
    }
    
    go64(&mod64);
    halt("Didn't boot");
    return 0;
}
