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
go64: // Fastcall, struct is at eax
    push eax
    mov esi, [eax+4]    // ESI = start of module
    
    mov eax, cr0                                   // Set the A-register to control register 0.
    and eax, 01111111111111111111111111111111b     // Clear the PG-bit, which is bit 31.
    mov cr0, eax                                   // Set control register 0 to the A-register.

    // Clear tables
    mov edi, 0x1000    // Set the destination index to 0x1000.
    mov cr3, edi       // Set control register 3 to the destination index.
    xor eax, eax       // Nullify the A-register.
    mov ecx, 4096      // Set the C-register to 4096.
    rep stosd          // Clear the memory.
    mov edi, cr3       // Set the destination index to control register 3.

    // Address the first table
    mov DWORD ptr [edi], 0x2003      // Set the uint32_t at the destination index to 0x2003.
    add edi, 0x1000              // Add 0x1000 to the destination index.
    mov DWORD ptr [edi], 0x3003      // Set the uint32_t at the destination index to 0x3003.
    add edi, 0x1000              // Add 0x1000 to the destination index.
    mov DWORD ptr [edi], 0x4003      // Set the uint32_t at the destination index to 0x4003.
    add edi, 0x1000              // Add 0x1000 to the destination index.

    // Set the elements of the first table
    mov DWORD ptr [edi], 0x2003      // Set the uint32_t at the destination index to 0x2003.
    add edi, 0x1000              // Add 0x1000 to the destination index.
    mov DWORD ptr [edi], 0x3003      // Set the uint32_t at the destination index to 0x3003.
    add edi, 0x1000              // Add 0x1000 to the destination index.
    mov DWORD ptr [edi], 0x4003      // Set the uint32_t at the destination index to 0x4003.
    add edi, 0x1000              // Add 0x1000 to the destination index.

    mov ebx, 0x00000003          // Set the B-register to 0x00000003.
    mov ecx, 512                 // Set the C-register to 512.
 
.SetEntry:
    mov dword ptr [edi], ebx         // Set the uint32_t at the destination index to the B-register.
    add ebx, 0x1000              // Add 0x1000 to the B-register.
    add edi, 8                   // Add eight to the destination index.
    loop .SetEntry               // Set the next entry.

    // Set long mode
    mov eax, cr4                 // Set the A-register to control register 4.
    or eax, 1 << 5               // Set the PAE-bit, which is the 6th bit (bit 5).
    mov cr4, eax                 // Set control register 4 to the A-register.

    // Turn on paging
    mov eax, cr0                 // Set the A-register to control register 0.
    or eax, 1 << 31              // Set the PG-bit, which is the 32nd bit (bit 31).
    mov cr0, eax                 // Set control register 0 to the A-register.

    pop eax
    mov eax, dword ptr [eax+12]
    
    //
    lgdt [.Pointer]              // Load the 64-bit global descriptor table.
    ljmp [eax]                   // Set the code segment and enter 64-bit long mode.

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
    .byte 10010010b                 // Access (read/write).
    .byte 00000000b                 // Granularity.
    .byte 0                         // Base (high).
.Pointer:
    .word 23                        // Limit.
    .long GDT64                     // Base.
