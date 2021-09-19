#ifndef MEGAOS_GDT_HPP
#define MEGAOS_GDT_HPP


#include <stdint.h>
#include <mkl/array.hpp>
#include <definitions.hpp>



ASM_DEFINED void asm_lgdt(uintptr_t desc_ptr);


class GDT
{
    struct gdt_entry
    {
        uint16_t limit;
        uint16_t base1;
        uint8_t base2;
        uint8_t access;
        uint8_t flags;
        uint8_t base3;
    } PACKED;

    struct gdt_register
    {
        uint16_t size;
        uint64_t base_addr;
    } PACKED;

    enum gdt_entry_vec : uint8_t
    {
        NULL_ENT = 0,

        _16BIT_CS = 1,
        _16BIT_DS = 2,

        _32BIT_CS = 3,
        _32BIT_DS = 4,

        _64BIT_CS = 5,
        _64BIT_DS = 6,
    };

    enum gdt_access_bits : uint8_t
    {
        /* present bit, must be set for all valid selectors */
        SEG_PRESENT = 1 << 7,

#define SEG_DPL(x) (((x) & 0b11) << 5)
        /* descriptor privilege levels */
        SEG_DPL0 = SEG_DPL(0),
        SEG_DPL1 = SEG_DPL(1),
        SEG_DPL2 = SEG_DPL(2),
        SEG_DPL3 = SEG_DPL(3),

#define SEG_TYPE(x) ((x) << 4)
        /* descriptor type, should be set for code or data segment */
        SEG_TYPE_DATA    = SEG_TYPE(1),
        SEG_TYPE_CODE    = SEG_TYPE(1),
        SEG_TYPE_SYS    = SEG_TYPE(0),

        /* set if the segment can be executed */
        SEG_EXEC    = 1 << 3,

        /* for code selectors only: code can be executed from an equal or lower privilege level */
        SEG_CONF    = 1 << 2,

        /* for data selectors only: 0 the segment grows up, 1 the segments grows down */
        SEG_DIR     = 1 << 2,

        /* for code selectors only: whether read access is allowed for segment */
        SEG_READ    = 1 << 1,

        /* for data selectors only: whether write access is allowed for segment */
        SEG_WRITE   = 1 << 1,
    };

    static constexpr uint8_t ENTRY_COUNT = 7;
    static mkl::array<gdt_entry, ENTRY_COUNT> table;
    static bool installed;

public:
    static constexpr uint16_t size() { return entry_count() * sizeof(gdt_entry); }
    static constexpr uint8_t entry_count() { return table.size(); };
    static constexpr uint8_t get_selector(gdt_entry_vec vec) { return vec * sizeof(gdt_entry); }
    static constexpr uint8_t kernel_cs() { return get_selector(_64BIT_CS); }

    static void install();

private:
    static void make_entry(gdt_entry_vec entry_index, uint8_t access, uint8_t flags, uint16_t limit = 0);
};



#endif // MEGAOS_GDT_HPP