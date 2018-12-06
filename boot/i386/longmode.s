.intel_syntax noprefix

    .text
    .globl has_longmode
has_longmode:
    // Check if CPUID sets longmode
    mov eax, 1 << 31
    inc eax
    cpuid

    test edx, 1 << 29
    jz NoLongmode

    xor eax, eax
    inc eax
    ret

NoLongmode:
    xor eax, eax
    ret

    .globl go64
go64: // Fastcall, struct is at ecx    
    mov eax, cr0                                   // Set the A-register to control register 0.
    and eax, 01111111111111111111111111111111b     // Clear the PG-bit, which is bit 31.
    mov cr0, eax                                   // Set control register 0 to the A-register.

    // Preload the jump address
    lea ebx, dword ptr [ecx+32]
    
    // Set long mode
    mov eax, cr4                 // Set the A-register to control register 4.
    or eax, 1 << 5               // Set the PAE-bit, which is the 6th bit (bit 5).
    mov cr4, eax                 // Set control register 4 to the A-register.

    // Reload cr3
    mov eax, [ecx + 16]
    mov cr3, eax

    mov ecx, 0xC0000080          // Set the C-register to 0xC0000080, which is the EFER MSR.
    rdmsr                        // Read from the model-specific register.
    or eax, 1 << 8               // Set the LM-bit which is the 9th bit (bit 8).
    wrmsr                        // Write to the model-specific register.

    // Turn on paging
    mov eax, cr0                 // Set the A-register to control register 0.
    or eax, 1 << 31              // Set the PG-bit, which is the 32nd bit (bit 31).
    mov cr0, eax                 // Set control register 0 to the A-register.

    // Load up the GDT
    lgdt [.Pointer]              // Load the 64-bit global descriptor table.
    ljmp [ebx]                   // Set the code segment and enter 64-bit long mode.

GDT64:                           // Global Descriptor Table (64-bit).
.Null:
    .word 0xFFFF                    // Limit (low).
    .word 0                         // Base (low).
    .byte 0                         // Base (middle)
    .byte 0                         // Access.
    .byte 1                         // Granularity.
    .byte 0                         // Base (high).
.Code:
    .word 0                         // Limit (low).
    .word 0                         // Base (low).
    .byte 0                         // Base (middle)
    .byte 10011010b                 // Access (exec/read).
    .byte 10101111b                 // Granularity, 64 bits flag, limit19:16.
    .byte 0                         // Base (high).
.Data:
    .word 0                         // Limit (low).
    .word 0                         // Base (low).
    .byte 0                         // Base (middle)
    .byte 10011010b                 // Access (read/write).
    .byte 00000000b                 // Granularity.
    .byte 0                         // Base (high).
.Pointer:
    .word 23                        // Limit.
    .long GDT64                     // Base.
