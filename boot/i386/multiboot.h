#ifndef _MULTIBOOT_H_
#define _MULTIBOOT_H_

#include "boottypes.h"

#define MEM_FLAG 1
#define BOOTDEV_FLAG 2
#define CMDLINE_FLAG 4
#define MODS_FLAG 8
#define SYMS1_FLAG 16
#define SYMS2_FLAG 32
#define MMAP_FLAG 64
#define DRIVES_FLAG 128
#define CONFIG_FLAG 256
#define BOOTLDR_FLAG 512
#define APM_FLAG 1024
#define VBE_FLAG 2048
#define FRAMEBUF_FLAG 4096

typedef struct _mb_mod {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t mod_args;
    uint32_t mod_reserved;
} mb_mod;

#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5

typedef struct _mb_mmap {
    uint32_t mmap_size;
    uint64_t mmap_addr;
    uint64_t mmap_len;
    uint32_t mmap_type;
} __attribute__((packed)) mb_mmap;

typedef struct _mb_root {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
    uint32_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
    const char color_info[1];
} mb_root;

const mb_mmap *getFirstMmapEntry(const mb_root *root);
const mb_mmap *getNextMmapEntry(const mb_root *root, const mb_mmap *mptr);

#endif//_MULTIBOOT_H_
