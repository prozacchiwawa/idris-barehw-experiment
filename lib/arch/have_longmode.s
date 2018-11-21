.intel_syntax noprefix

    .text
    .globl has_longmode
has_longmode:
    // Check if CPUID sets longmode
    mov rax, 1 << 31
    inc rax
    cpuid

    test rdx, 1 << 29
    jz NoLongmode

    xor rax, rax
    inc rax
    ret

NoLongmode:
    xor rax, rax
    ret
