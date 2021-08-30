#include <logging.hpp>
#include <arch/x64/gdt.hpp>


GDT::gdt_entry GDT::table[ENTRY_COUNT] {};
bool GDT::installed = false;

void GDT::make_entry(gdt_entry_vec entry_index, uint8_t access, uint8_t flags, uint16_t limit)
{
    if (entry_index >= ENTRY_COUNT)
        return;

    auto& entry = table[entry_index];

    entry.base1 = 0;
    entry.base2 = 0;
    entry.base3 = 0;

    // limit parameter is only on 16-bits since
    // the 4 remaining bits are stored in flags
    entry.limit = limit;
    entry.access = access;
    entry.flags = flags;
}

void GDT::install()
{
    if (installed)
        return;

    log::puts("Installing GDT...\n");

    constexpr auto cs_access_bits = SEG_PRESENT | SEG_DPL0 | SEG_TYPE_CODE | SEG_EXEC | SEG_READ;
    constexpr auto ds_access_bits = SEG_PRESENT | SEG_DPL0 | SEG_TYPE_DATA | SEG_WRITE;

    /* define the mandatory null entry */
    make_entry(NULL_ENT, 0, 0);

    /* define the entries conforming the stivale2 spec */
    make_entry(_16BIT_CS, cs_access_bits, 0b00000000, 0xFFFF);
    make_entry(_16BIT_DS, ds_access_bits, 0b00000000, 0xFFFF);

    make_entry(_32BIT_CS, cs_access_bits, 0b11001111, 0xFFFF);
    make_entry(_32BIT_DS, ds_access_bits, 0b11001111, 0xFFFF);

    make_entry(_64BIT_CS, cs_access_bits, 0x20);
    make_entry(_64BIT_DS, ds_access_bits, 0x0);

    /* tells the CPU where the GDT is */
    gdt_register desc_ptr {
        .size = size() - 1,
        .base_addr = (uint64_t)&table
    };

    asm_lgdt((uintptr_t)&desc_ptr);

    log::puts("GDT successfully installed !\n");

    installed = true;
}



