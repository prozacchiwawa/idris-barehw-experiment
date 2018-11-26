.intel_syntax noprefix

    .text
    .globl _start
    .globl main
_start:
    push ebx
    push 0
    jmp main

    .align 8
    .globl _multiboot
_multiboot:
    .long 0x1BADB002
    .long 1
    .long 3830599677
