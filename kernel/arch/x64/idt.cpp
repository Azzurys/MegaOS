#include <logging.hpp>
#include <arch/x64/idt.hpp>
#include <arch/x64/gdt.hpp>
#include <arch/x64/interrupts.hpp>


IDT::idt_entry IDT::table[ENTRY_COUNT] {};
bool IDT::installed = false;


void IDT::make_entry(uint16_t entry_vec, uintptr_t isr, uint8_t flags)
{
    if (entry_vec >= ENTRY_COUNT)
        return;

    idt_entry& gate = table[entry_vec];
    gate.flags = flags;
    gate.selector = GDT::kernel_cs();
    gate.ist = 0;
    gate.zero = 0;

    gate.offset_1 = isr & 0xFFFF;
    gate.offset_2 = (isr >> 16) & 0xFFFF;
    gate.offset_3 = (isr >> 32) & 0xFFFFFFFF;
}

void IDT::install()
{
    if (installed)
        return;

    // Interrupts are cleared for the moment
    // because the IDT is not loaded yet

    constexpr auto type_attr = static_cast<uint8_t>(PRESENT | DPL0 | gate_type::intr32);

    // The 32 first interrupts (0-31) are architecture-defined
    for (uint16_t vector = 0; vector < interrupts::ARCH_DEFINED_COUNT; ++vector)
        make_entry(vector, interrupts::isr::handlers[vector], type_attr);

    idt_register idt_ptr {
        .size = size() - 1,
        .base_addr = (uint64_t)&table
    };

    __asm__ volatile("lidt %0" : : "m"(idt_ptr));

    installed = true;

    log::printf("IDT successfully installed: %#lX %#lX\n",
                idt_ptr.base_addr,
                idt_ptr.size
    );
}
