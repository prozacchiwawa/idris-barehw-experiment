OUTPUT_FORMAT("elf64-x86-64");
ENTRY(_start);
phys = 0x1000;
SECTIONS
{
  .text phys : AT(phys) {
_TEXT_START_ = .;
    code = .;
    *(.text)
    *(.rodata)
_TEXT_END_ = .;
    . = ALIGN(4096);
  }

  .data : AT(phys + (data - code)) {
    data = .;
_DATA_START_ = .;
    *(.data)
_DATA_END_ = .;
    . = ALIGN(4096);
  }

  .bss : AT(phys + (bss - code)) {
_BSS_START_ = .;
    bss = .;
    *(.bss)
_BSS_END_ = .;
    . = ALIGN(4096);
  }
}
