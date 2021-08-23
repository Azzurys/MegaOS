#include <stdint.h>
#include <stddef.h>

#include <stivale2.h>
#include <arch/x64/gdt.hpp>


constexpr auto KERNEL_STACK_SIZE = 8192u;
static uint8_t kstack[KERNEL_STACK_SIZE];


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
    .stack = (uintptr_t)kstack + KERNEL_STACK_SIZE,
    .flags = 0b110,
    .tags = (uintptr_t)&framebuffer_header_tag,
};


[[noreturn]] void halt()
{
    for (;;)
        __asm__("hlt");
}

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

extern "C"
void _start(stivale2_struct* boot_info)
{
    const auto term_tag = st2_get_tag<stivale2_struct_tag_terminal>(boot_info, STIVALE2_STRUCT_TAG_TERMINAL_ID);

    if (term_tag == nullptr)
        halt();

    using TermWriteFunc = void(*)(const char*, size_t);
    void *term_write_ptr = (void*)term_tag->term_write;
    auto term_write = reinterpret_cast<TermWriteFunc>(term_write_ptr);

    term_write("Mega OS\n", 9);

    term_write("Installing GDT...\n", 19);
    GDT::install();
    term_write("GDT successfully installed !\n", 30);

    halt();
}



