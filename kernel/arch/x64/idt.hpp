#ifndef MEGAOS_IDT_HPP
#define MEGAOS_IDT_HPP


#include <stdint.h>
#include <utils.hpp>
#include <mkl/array.hpp>



class IDT
{
    enum type_attribute : uint8_t
    {
        PRESENT = 1 << 7,

#define DPL(x) (((x) & 0b11) << 5)
        /* descriptor privilege levels */
        DPL0 = DPL(0),
        DPL1 = DPL(1),
        DPL2 = DPL(2),
        DPL3 = DPL(3),
    };

    enum gate_type : uint8_t
    {
        // The Intel SDM specifies that legacy 32-bit
        // interrupt or trap gate types (intr32, trap32)
        // are redefined in IA-32e mode as 64-bit types

        task32 = 0b0101,
        intr16 = 0b0110,
        intr32 = 0b1110,
        trap16 = 0b0111,
        trap32 = 0b1111
    };

    struct idt_entry
    {
        uint16_t offset_1;
        uint16_t selector;
        uint8_t  ist;
        uint8_t flags;
        uint16_t offset_2;
        uint32_t offset_3;
        uint32_t zero;
    } PACKED;

    struct idt_register
    {
        uint16_t size;
        uint64_t base_addr;
    } PACKED;


    static constexpr uint16_t ENTRY_COUNT = 0x100;
    static mkl::array<idt_entry, ENTRY_COUNT> table;
    static bool installed;

public:
    static constexpr uint16_t size() { return entry_count() * sizeof(idt_entry); }
    static constexpr uint16_t entry_count() { return table.size(); }
    static void make_entry(uint16_t entry_vec, uintptr_t isr, uint8_t flags);
    static void install();
};


#endif // MEGAOS_IDT_HPP