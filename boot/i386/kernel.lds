OUTPUT_FORMAT("elf64-x86-64");
ENTRY(_start);
SECTIONS
{
  . = 0x100000;
  .text : {
_TEXT_START_ = .;
    code = .;
    *(.text) ;
    *(.rodata) ;
_TEXT_END_ = .;
    . = ALIGN(4096);
  }

  .data : {
    data = .;
_DATA_START_ = .;
    *(.data)
_DATA_END_ = .;
    . = ALIGN(4096);
  }

  .bss : {
_BSS_START_ = .;
    bss = .;
    *(.bss)
_BSS_END_ = .;
    . = ALIGN(4096);
  }

  /DISCARD/ : { *(.note.*) }
}
