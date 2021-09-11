#include <stdint.h>

#include <stivale2.hpp>
#include <logging.hpp>

#include <arch/x86_64/cpu.hpp>
#include <arch/x86_64/gdt.hpp>
#include <arch/x86_64/idt.hpp>
#include <arch/x86_64/interrupts.hpp>

#include <mkl/array.hpp>


static mkl::array<uint8_t, 8192> kstack;


static stivale2_header_tag_terminal
terminal_header_tag {
    .tag = {
            .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
            .next = 0          // end of linked list
    },
    .flags = 0
};


static stivale2_header_tag_framebuffer
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
static stivale2_header st2_header {
    .entry_point = 0,
    .stack = (uintptr_t)kstack.data() + kstack.size(),
    .flags = 0b110,
    .tags = (uintptr_t)&framebuffer_header_tag,
};


template<typename TagType>
TagType* st2_get_tag(stivale2_struct* st2_struct, uint64_t tag_id)
{
    auto curr_tag = reinterpret_cast<stivale2_tag*>(st2_struct->tags);

    for (;;)
    {
        if (curr_tag == nullptr)
            return nullptr;

        if (curr_tag->identifier == tag_id)
            return reinterpret_cast<TagType*>(curr_tag);

        curr_tag = reinterpret_cast<stivale2_tag*>(curr_tag->next);
    }
}

log::TTYWriteFunction get_write_function(stivale2_struct* boot_info)
{
    const auto term_tag = st2_get_tag<stivale2_struct_tag_terminal>(boot_info, STIVALE2_STRUCT_TAG_TERMINAL_ID);

    if (term_tag == nullptr)
        cpu::halt();

    return reinterpret_cast<log::TTYWriteFunction>((void*)term_tag->term_write);
}

extern "C"
[[noreturn]] void _start(stivale2_struct* boot_info)
{
    const auto func = get_write_function(boot_info);
    log::init(func);

    log::printf("Mega OS - %s %s\n\n", boot_info->bootloader_brand, boot_info->bootloader_version);

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
    log::puts("Interrupts enabled");

    while (true);
}



