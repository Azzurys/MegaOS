#include "gdt.hpp"


void GDT::make_entry(uint8_t entry_index, uint8_t access, uint8_t flags, uint32_t limit)
{
    if (entry_index >= ENTRY_COUNT)
        return;

    auto& entry = table[entry_index];

    entry.base1 = 0;
    entry.base2 = 0;
    entry.base3 = 0;

    entry.limit = limit;
    entry.access = access;
    entry.flags = flags;
}

void GDT::install()
{

    // See https://wiki.osdev.org/Global_Descriptor_Table for precisions on bitfields
    // Only the privilege bits change depending to segment
    // Kernel has ring 0 (highest) while User has ring 3 (lowest)
    const uint8_t kern_access_bits = 0b10010010;
    const uint8_t user_access_bits = 0b11110010;

    /* define the mandatory null entry */
    make_entry(0, 0, 0);

    /* define the entry conforming the stivale2 spec */
    make_entry(1, 0b10011010, 0, 0xFFFF);
    make_entry(2, 0b10010010, 0, 0xFFFF);
    make_entry(3, 0b10011010, 0b11001111);
    make_entry(4, 0b10010010, 0b11001111);
    make_entry(5, 0b10011010, 0b00100000);
    make_entry(6, 0b10010010, 0);

    /* Set the exec bit as the entries describe code segments */
    make_entry(KERN_CS, kern_access_bits | EXEC_BIT, 0x20);
    make_entry(USER_CS, user_access_bits | EXEC_BIT, 0x20);

    make_entry(KERN_DS, kern_access_bits, 0x20);
    make_entry(USER_DS, user_access_bits, 0x20);

    /* tells the CPU where the GDT is */
    gdt_register desc_ptr {
        .size = ENTRY_COUNT * sizeof(gdt_entry) - 1,
        .offset = (uint64_t)&table
    };

    asm_lgdt((uintptr_t)&desc_ptr);
}



