.intel_syntax noprefix

    .text
    .globl has_extfun
has_extfun:
    // Check if CPUID[RAX=0x80000000] gives at least 0x80000001
    mov eax, 1 << 31
    cpuid
    
    mov ebx, 1 << 31
    inc ebx
    
    cmp eax, ebx
    jb NoExtfun

    xor eax, eax
    inc eax
    ret
    
NoExtfun:
    xor eax, eax
    ret
