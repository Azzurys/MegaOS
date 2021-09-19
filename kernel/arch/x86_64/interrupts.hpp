#ifndef MEGAOS_INTERRUPTS_HPP
#define MEGAOS_INTERRUPTS_HPP


#include <stdint.h>
#include <logging.hpp>
#include <mkl/array.hpp>
#include <definitions.hpp>
#include <arch/x86_64/cpu.hpp>


namespace interrupts
{
    struct interrupt_frame
    {
        cpu::registers GPRs;
        uint64_t err_code;
        uint64_t int_no;
        uint64_t rip;
        uint64_t cs;
        uint64_t rflags;
        uint64_t rsp;
        uint64_t ss;
    } PACKED;

    /* see page 2996 of the Intel SDM */
    constexpr uint8_t EXCEPTION_COUNT = 32;

    constexpr mkl::array<const char* const, EXCEPTION_COUNT> exception_desc = {
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
        /* We explicitly separate exception interrupts from IRQs */
        ASM_DEFINED uintptr_t exceptions_handlers[];
        ASM_DEFINED uintptr_t interrupts_handlers[];

        extern "C" void cpu_exception_handler(interrupt_frame* frame);
        extern "C" void irq_interrupt_handler(interrupt_frame* frame);

        /*!
         * @brief install the actual handlers
         */
        void install();
    }

    namespace pic
    {
        enum ports : uint16_t
        {
            /* Master / Slave controll ports */
            MPIC_COMMAND = 0x20,
            SPIC_COMMAND = 0xA0,

            /* Master / Slave data ports */
            MPIC_DATA = 0x21,
            SPIC_DATA = 0xA1
        };

        enum commands : uint8_t
        {
            INIT = 0x10,    // Initialization command
            ICW4 = 0x01,    // Give additional info about the environment
            EOI  = 0x20     // Issued at the end of an IRQ interrupt routine
        };

        /*!
         * @brief disable the PIC, useful if we use the processor LAPIC or IOAPIC,
         *        as we must disable the PIC first
         */
        inline void disable();

        void set_irq_mask(uint8_t irq);
        void clear_irq_mask(uint8_t irq);

        /*!
         * @brief Remap the interrupts. The PIC uses interrupts 0 - 15 for hardware
         * interrupts by default, which conflicts with the CPU interrupts.
         * Therefore the PIC interrupts must be remapped to another interval.
         *
         * @param mpic_offset offset for remapped master PIC's vectors
         * @param spic_offset offset for remapped slave  PIC's vectors
         */
        void remap(uint8_t mpic_offset, uint8_t spic_offset);

        inline void send_eoi(uint8_t irq_no);
    }
}


#endif //MEGAOS_INTERRUPTS_HPP
