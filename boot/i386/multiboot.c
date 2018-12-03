#include "multiboot.h"

const mb_mmap *getFirstMmapEntry(const mb_root *root) {
    if (root->flags & MMAP_FLAG) {
        return (mb_mmap *)root->mmap_addr;
    } else {
        return nullptr;
    }
}

const mb_mmap *getNextMmapEntry(const mb_root *root, const mb_mmap *ptr) {
    const char *cptr = (const char *)ptr;
    const uint32_t *skip = (const uint32_t *)cptr;
    if (!*skip) {
        return nullptr;
    }
    const char *nextptr = cptr + *skip;
    uint32_t checkval = (uint32_t)nextptr;
    uint32_t limitval = root->mmap_addr + root->mmap_length;
    if (checkval >= limitval) {
        return nullptr;
    }
    return (mb_mmap*)(nextptr + sizeof(uint32_t));
}
