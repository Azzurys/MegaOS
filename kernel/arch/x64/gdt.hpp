#ifndef MEGAOS_GDT_HPP
#define MEGAOS_GDT_HPP


#include <stdint.h>


struct gdt_entry
{
    uint16_t limit;
    uint16_t base1;
    uint8_t base2;
    uint8_t access;
    uint8_t flags;
    uint8_t base3;
} __attribute__((packed));


struct gdt_register
{
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));


extern "C" void asm_lgdt(uintptr_t desc_ptr);


class GDT
{
    static constexpr uint8_t ENTRY_COUNT = 11;

    /* segment descriptor offsets in the GDT */
    static constexpr uint8_t KERN_CS = 7;
    static constexpr uint8_t KERN_DS = 8;
    static constexpr uint8_t USER_CS = 9;
    static constexpr uint8_t USER_DS = 10;

    // Executable bit:
    // if set, the segment can be executed   -> code selector
    // if not, the segment can't be executed -> data selector
    static constexpr uint8_t EXEC_BIT = 1 << 3;

    static gdt_entry table[ENTRY_COUNT];

public:
    static uint8_t entry_count() { return ENTRY_COUNT; };

    static void install();

private:
    static void make_entry(uint8_t entry_index, uint8_t access, uint8_t flags, uint32_t limit = 0);
};


#endif // MEGAOS_GDT_HPP