.intel_syntax noprefix
    
    .text
    .globl _start
    .globl argv_str
    .globl argv_
    .globl main
_start:
    mov rsp, 0xfff0
    xor rdi, rdi
    inc rdi
    lea rsi, argv_str
    lea rax, argv_
    push rsi
    push rdi
    call main

argv_str:
    .ascii "kernel"
    .byte 0

argv_:
    .long argv_str
    .long 0

