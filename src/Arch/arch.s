.intel_syntax noprefix

    .text
    .globl getCR3
getCR3:
    mov     rax,cr3
    ret

    .globl setCR3
setCR3: 
    mov     cr3,rdi
    ret

    .globl lidt
lidt:
    lidt    byte ptr [rdi]
    ret

    .globl lgdt
lgdt:
    lgdt    byte ptr [rdi]
    ret

    .globl waitEvent
waitEvent:
    sti
    hlt
    ret

    .globl halt
halt:
    cli
    hlt
    ret

    .globl peek8
peek8:
    xor rax, rax
    mov al, byte ptr [rdi]
    ret
