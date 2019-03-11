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

    .globl waitEvent
waitEvent:
    sti
    hlt
    ret
