#include <arch/x64/interrupts.hpp>


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
        extern "C" __attribute__((interrupt))
        void generic_handler(interrupt_frame* regs)
        {
            log::printf("Received interrupt: %d\n", regs->int_no);
            log::printf("Error Code: %d\n", regs->err_code);

            if (regs->int_no < ARCH_DEFINED_COUNT)
                log::printf("Description: %s\n", arch_defined_descriptions[regs->int_no]);

            __asm__ volatile("cli; hlt");
        }
    }
}
