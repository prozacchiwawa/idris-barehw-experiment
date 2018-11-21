.intel_syntax noprefix

    .text
    .globl has_cpuid
has_cpuid:
    // Check if CPUID is supported by attempting to flip the ID bit (bit 21) in
    // the FLAGS register. If we can flip it, CPUID is available.
 
    // Copy FLAGS in to EAX via stack
    pushf
    pop eax
 
    // Copy to ECX as well for comparing later on
    mov ecx, eax
 
    // Flip the ID bit
    xor eax, 1 << 21
 
    // Copy EAX to FLAGS via the stack
    push eax
    popf
 
    // Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
    pushf
    pop eax
 
    // Restore FLAGS from the old version stored in ECX (i.e. flipping the ID bit
    // back if it was ever flipped).
    push ecx
    popf
 
    // Compare EAX and ECX. If they are equal then that means the bit wasn't
    // flipped, and CPUID isn't supported.
    xor eax, ecx
    ret
