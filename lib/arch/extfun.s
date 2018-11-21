.intel_syntax noprefix

    .text
    .globl has_extfun
has_extfun:
    // Check if CPUID[RAX=0x80000000] gives at least 0x80000001
    mov rax, 1 << 31
    cpuid
    
    mov rbx, 1 << 31
    inc rbx
    
    cmp rax, rbx
    jb NoExtfun

    xor rax, rax
    inc rax
    ret
    
NoExtfun:
    xor rax, rax
    ret
