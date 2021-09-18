#include <stdint.h>

#include <stivale2.hpp>

#include <logging.hpp>

#include <arch/x86_64/cpu.hpp>
#include <arch/x86_64/gdt.hpp>
#include <arch/x86_64/idt.hpp>
#include <arch/x86_64/interrupts.hpp>

#include <mkl/array.hpp>


static mkl::array<uint8_t, 8192> kstack;


static st2::header_tag_terminal
terminal_header_tag {
    .tag = {
            .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
            .next = 0          // end of linked list
    },
    .flags = 0
};


static st2::header_tag_framebuffer
framebuffer_header_tag {
    .tag = {
            .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
            .next = (uintptr_t)&terminal_header_tag
    },

    /* let the bootloader pick for us */
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};


__attribute__((section(".stivale2hdr"), used))
static st2::header st2_header {
    .entry_point = 0,
    .stack = (uintptr_t)kstack.data() + kstack.size(),
    .flags = 0b110,
    .tags = (uintptr_t)&framebuffer_header_tag,
};


extern "C"
[[noreturn]] void _start(st2::st2_struct* boot_info)
{
    if (!boot_info)
        cpu::halt();

    if (!log::init(boot_info))
        cpu::halt();

    log::kprintf("Mega OS - %s %s\n\n", boot_info->bootloader_brand, boot_info->bootloader_version);

    GDT::install();
    IDT::install();


    // remap master PIC and slave PIC to avoid conflict between IRQ and CPU exceptions.
    // remap MPIC to start at vector 32 (right after CPU exceptions)
    // remap SPIC to start at vector 40 (as there is 8 inputs by PIC)
    constexpr auto mpic_offset = interrupts::EXCEPTION_COUNT;
    constexpr auto spic_offset = interrupts::EXCEPTION_COUNT + 8;

    /* remap master PIC and slave PIC */
    interrupts::pic::remap(mpic_offset, spic_offset);

    /* the stivale2 boot protocol sets all the irq mask, which make the PIC ignore irqs */
    interrupts::pic::clear_irq_mask(1);
    interrupts::enable();
    log::dputs("Interrupts enabled");

    while (true);
}



