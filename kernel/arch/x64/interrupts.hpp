#ifndef MEGAOS_INTERRUPTS_HPP
#define MEGAOS_INTERRUPTS_HPP


#include <stdint.h>
#include <utils.hpp>
#include <logging.hpp>


namespace interrupts
{
    struct interrupt_frame
    {
        uint64_t r15;
        uint64_t r14;
        uint64_t r13;
        uint64_t r12;
        uint64_t r11;
        uint64_t r10;
        uint64_t r9;
        uint64_t r8;
        uint64_t rsi;
        uint64_t rdi;
        uint64_t rbp;
        uint64_t rdx;
        uint64_t rcx;
        uint64_t rbx;
        uint64_t rax;
        uint64_t err_code;
        uint64_t int_no;
        uint64_t rip;
        uint64_t cs;
        uint64_t rflags;
        uint64_t rsp;
        uint64_t ss;
    } PACKED;

    /* see page 2996 of the Intel SDM */
    constexpr uint8_t ARCH_DEFINED_COUNT = 32;

    const char* const arch_defined_descriptions[ARCH_DEFINED_COUNT] = {
            "Divide Error",
            "Debug Exception",
            "NMI Interrupt",
            "Breakpoint",
            "Overflow",
            "BOUND Range Exceeded",
            "Invalid Opcode",
            "Device Not Available",
            "Double Fault",
            "Coprocessor Segment Overrun",
            "Invalid TSS",
            "Segment Not Present",
            "Stack-Segment-Fault",
            "General Protection",
            "Page Fault",
            "Intel Reserved (15)",
            "x87 FPU Floating-Point Error",
            "Alignment Check",
            "Machine Check",
            "SIMD Floating-Point Exception",
            "Virtualization Exception",
            "Control Protection Exception",
            "Intel Reserved (22)",
            "Intel Reserved (23)",
            "Intel Reserved (24)",
            "Intel Reserved (25)",
            "Intel Reserved (26)",
            "Intel Reserved (27)",
            "Intel Reserved (28)",
            "Intel Reserved (29)",
            "Intel Reserved (30)",
            "Intel Reserved (31)",
    };

    bool enabled();
    void enable();
    void disable();

    namespace isr
    {
        /* points to an array of handlers defined in ASM */
        ASM_DEFINED void* handlers[];

        extern "C" __attribute__((interrupt))
        void generic_handler(interrupt_frame* regs);
    }
}


#endif //MEGAOS_INTERRUPTS_HPP
