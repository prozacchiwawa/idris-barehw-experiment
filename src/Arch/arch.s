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

    .globl poke16
poke16:
    mov word ptr [rdi], si
    ret

    .globl peek64
peek64:
    mov rax, qword ptr [rdi]
    ret

    .globl poke64
poke64:
    mov qword ptr [rdi], rsi
    ret
    
    .globl invlpg
invlpg:
    invlpg byte ptr [rdi]
    ret
