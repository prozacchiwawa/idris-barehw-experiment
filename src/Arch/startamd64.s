.intel_syntax noprefix
    
    .text
    .globl _start
    .globl argv_str
    .globl argv_
    .globl main
_start:
    mov rsp, 0xfff0
    mov qword ptr [argv_ + 8], rdi
    xor rdi, rdi
    inc rdi
    inc rdi
    lea rsi, argv_
    call main

    .globl getCommandLineArgPtr
getCommandLineArgPtr:
    mov rax, rdi
    mov rdx, 8
    mul rdx
    lea rsi, qword ptr [argv_]
    add rsi, rax
    mov rax, qword ptr [rsi]
    ret
    
argv_str:
    .ascii "kernel"
    .byte 0

argv_:
    .long argv_str
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
