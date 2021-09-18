#include <logging.hpp>
#include <arch/x86_64/gdt.hpp>


mkl::array<GDT::gdt_entry, GDT::ENTRY_COUNT> GDT::table {};
bool GDT::installed = false;

void GDT::make_entry(gdt_entry_vec entry_index, uint8_t access, uint8_t flags, uint16_t limit)
{
    if (entry_index >= table.size())
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
    gdt_register gdt_ptr {
        .size = size() - 1,
        .base_addr = (uint64_t)&table
    };

    asm_lgdt((uintptr_t)&gdt_ptr);

    log::dprintf("GDT successfully installed: %#lX %#lX\n",
                 gdt_ptr.base_addr,
                 gdt_ptr.size
    );

    installed = true;
}



