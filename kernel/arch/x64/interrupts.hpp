#ifndef MEGAOS_INTERRUPTS_HPP
#define MEGAOS_INTERRUPTS_HPP


#include <stdint.h>
#include <utils.hpp>
#include <logging.hpp>
#include <arch/x64/cpu.hpp>


namespace interrupts
{
    struct interrupt_frame
    {
        cpu::registers GPRs;
        uint64_t int_no;
        uint64_t err_code;
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
        ASM_DEFINED uintptr_t handlers[];

        extern "C"
        void cpu_exception_handler(interrupt_frame* frame);
    }
}


#endif //MEGAOS_INTERRUPTS_HPP
