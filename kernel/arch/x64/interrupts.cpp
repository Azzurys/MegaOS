#include <devices/ps2keyboard.hpp>
#include <arch/x64/interrupts.hpp>
#include <arch/x64/cpu.hpp>
#include <panic.hpp>


namespace interrupts
{
    bool enabled()
    {
        uint16_t eflags_low;

        // push 16 lower bits of RFLAGS register onto the stack
        // pop value from the stack into output variable
        __asm__ volatile(
            "pushf\n\t"
            "pop %0"
            : "=g"(eflags_low)
        );

        /* check if IF is set */
        return eflags_low & 0x200;
    }

    void enable()
    {
        __asm__ volatile("sti");
    }

    void disable()
    {
        __asm__ volatile("cli");
    }

    namespace isr
    {
        enum class class_type
        {
            fault,       // These can be corrected and the program may continue as if nothing happened
            trap,        // Reported immediately after the execution of the trapping instruction
            abort,       // unrecoverable error.

            /* Defined in the intel SDM table p. 2996 Table 6-1 */
            interrupt,   // only occur when NMI interrupt or for user-defined interrupts
            reserved     // serves as a place holder for reserved ints
        };

        static class_type int_type_lut[EXCEPTION_COUNT] = {
                class_type::fault,
                class_type::fault,
                class_type::interrupt,
                class_type::trap,
                class_type::trap,
                class_type::fault,
                class_type::fault,
                class_type::fault,
                class_type::abort,
                class_type::fault,
                class_type::fault,
                class_type::fault,
                class_type::fault,
                class_type::fault,
                class_type::fault,
                class_type::reserved,
                class_type::fault,
                class_type::fault,
                class_type::abort,
                class_type::fault,
                class_type::fault,
                class_type::fault,
                class_type::reserved,
                class_type::reserved,
                class_type::reserved,
                class_type::reserved,
                class_type::reserved,
                class_type::reserved,
                class_type::reserved,
                class_type::reserved,
                class_type::reserved,
                class_type::reserved,
        };

        inline class_type get_class_type(uint64_t int_no)
        {
            return int_type_lut[int_no];
        }

        /*!
         * @brief checks if the interrupt was generated
         *        internally by the CPU.
         */
        inline bool is_cpu_exception(uint64_t int_no)
        {
            return int_no < EXCEPTION_COUNT;
        }

        /*!
         * @brief checks if the interrupt was generated
         *        externally by some hardware.
         */
        inline bool is_irq_interrupt(uint64_t int_no)
        {
            return int_no >= EXCEPTION_COUNT && int_no < 256;
        }

        using handler_function = void (*)(interrupt_frame*);
        mkl::array<handler_function, 255> handlers;

        void install()
        {
            for (handler_function& handler : handlers)
                handler = nullptr;

            handlers[33] = ps2::keyboard::controller::irq_handler;

            log::puts("ISR successfully installed");
        }

        extern "C"
        void cpu_exception_handler(interrupt_frame* frame)
        {
            /* should never happen except if you explicitly call it yourself */
            if (!frame)
                kpanic("Invalid Pointer to GPRs in CPU exception handler");

            /* should never happen except if you explicitly call it yourself */
            if (!is_cpu_exception(frame->int_no))
                kpanic("Not a CPU exception", frame);

            // Check if the interrupt is of type abort, if so, kernel panic then halt.
            if (get_class_type(frame->int_no) == class_type::abort)
                kpanic("CPU interrupt of class abort.", frame);

            // Check if the interrupt has an handler
            if (handlers[frame->int_no])
                handlers[frame->int_no](frame);
            else
                kpanic("CPU interrupt with no handler", frame);
        }

        extern "C"
        void irq_interrupt_handler(interrupt_frame* frame)
        {
            /* should never happen except if you explicitly call it yourself */
            if (!frame)
                kpanic("Invalid Pointer to GPRs in IRQ interrupt handler");

            /* should never happen except if you explicitly call it yourself */
            if (!is_irq_interrupt(frame->int_no))
                kpanic("Not an IRQ interrupt", frame);

            /* Call the IRQ handler */
            if (handlers[frame->int_no])
                handlers[frame->int_no](frame);
            else
                log::printf("IRQ %d received but has no handler\n", frame->int_no);

            pic::send_eoi(frame->int_no);
        }
    }

    namespace pic
    {
        inline void disable()
        {
            cpu::io::outb(ports::MPIC_DATA, 0xFF);
            cpu::io::outb(ports::SPIC_DATA, 0xFF);
        }

        void set_irq_mask(uint8_t irq)
        {
            uint16_t data_port = ports::MPIC_DATA;

            if (irq >= 8)
            {
                data_port = ports::SPIC_DATA;
                irq -= 8;
            }

            uint8_t bitset = cpu::io::inb(data_port) | (1 << irq);
            cpu::io::outb(data_port, bitset);
        }

        void clear_irq_mask(uint8_t irq)
        {
            uint16_t data_port = ports::MPIC_DATA;

            if (irq >= 8)
            {
                data_port = ports::SPIC_DATA;
                irq -= 8;
            }

            uint8_t bitset = cpu::io::inb(data_port) & ~(1 << irq);
            cpu::io::outb(data_port, bitset);
        }

        void remap_pic(ports command_port, ports data_port, uint8_t pic_offset)
        {
            // save interrupt mask register because
            // we are going to write in the data port
            const auto imr = cpu::io::inb(data_port);

            const uint8_t icw3 = (command_port == ports::MPIC_COMMAND) ? 0x4 : 0x2;

            /* Start initialization elements of the PIC, after this command, the PIC will expect 3 bytes */
            cpu::io::outb(command_port, INIT | ICW4); cpu::io::wait();

            cpu::io::outb(data_port, pic_offset); cpu::io::wait();  // Vector offset byte
            cpu::io::outb(data_port, icw3);       cpu::io::wait();  // Slave / Master wiring byte
            cpu::io::outb(data_port, 0x1);  cpu::io::wait();  // Environment info byte, 8086 mode
            cpu::io::outb(data_port, imr); // restore imr
        }

        void remap(uint8_t mpic_offset, uint8_t spic_offset)
        {
            remap_pic(ports::MPIC_COMMAND, ports::MPIC_DATA, mpic_offset);
            remap_pic(ports::SPIC_COMMAND, ports::SPIC_DATA, spic_offset);

            log::printf("PIC remapped: PIC1 -> %d | PIC2 -> %d\n", mpic_offset, spic_offset);
        }

        inline void send_eoi(uint8_t irq_no)
        {
            // If the IRQ came from the Master PIC, we
            // can just send the command to the Master PIC,
            // but if it came from the Slave PIC, we must
            // send to both chips.

            if (irq_no >= 8)
                cpu::io::outb(ports::SPIC_COMMAND, commands::EOI);

            cpu::io::outb(ports::MPIC_COMMAND, commands::EOI);
        }
    }
}
