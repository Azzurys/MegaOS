OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)


ENTRY(_start)

PHDRS
{
    null    PT_NULL    FLAGS(0) ;                   /* Null segment */
    text    PT_LOAD    FLAGS((1 << 0) | (1 << 2)) ; /* Execute + Read */
    rodata  PT_LOAD    FLAGS((1 << 2)) ;            /* Read only */
    data    PT_LOAD    FLAGS((1 << 1) | (1 << 2)) ; /* Write + Read */
    dynamic PT_DYNAMIC FLAGS((1 << 1) | (1 << 2)) ; /* Dynamic segment needed for PIE */
}

SECTIONS
{
    /* We are building a higher half kernel */
    . = 0xffffffff80200000;

    .text : {
        *(.text*)
    } :text

    /* Move to the next memory page for .rodata */
    . += 0x1000;

    /* We place the .stivale2hdr section containing the header in its own section, */
    /* and we use the KEEP directive on it to make sure it doesn't get discarded. */
    .stivale2hdr : {
        KEEP(*(.stivale2hdr))
    } :rodata

    .rodata : {
        *(.rodata*)
    } :rodata

    /* Move to the next memory page for .data */
    . += 0x1000;

    .data : {
        *(.data*)
    } :data

    /* Dynamic section needed for PIE */
    .dynamic : {
        *(.dynamic)
    } :data :dynamic

    .bss : {
        *(COMMON)
        *(.bss*)
    } :data
}
