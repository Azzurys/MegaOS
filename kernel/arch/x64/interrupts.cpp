#include <arch/x64/interrupts.hpp>
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

    namespace exception
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

        static class_type int_type_lut[ARCH_DEFINED_COUNT] = {
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

        class_type get_class_type(int int_no)
        {
            return int_type_lut[int_no];
        }

        /*!
         * @brief checks if the interrupt was generated
         *        internally by the CPU.
         */
        bool is_cpu_exception(uint64_t int_no)
        {
            return int_no < ARCH_DEFINED_COUNT;
        }
    }

    namespace isr
    {
        extern "C"
        void cpu_exception_handler(interrupt_frame* frame)
        {
            /* should never happen except if you explicitly call it yourself */
            if (!frame)
                kpanic("Invalid Pointer to GPRs in CPU exception handler");

            /* should never happen except if you explicitly call it yourself */
            if (!exception::is_cpu_exception(frame->int_no))
                kpanic("Not a CPU exception", frame);

            const int int_no = static_cast<int>(frame->int_no);

            // Check if the interrupt is an abort type, if so, kernel panic then halt.
            if (exception::get_class_type(int_no) == exception::class_type::abort)
                kpanic("CPU interrupt of class abort.", frame);
            else
                // For the others, we can't do much about them at this stage of the OS, so we must halt too
                // When our OS grows, we will still need to deal with interrupts like page faults.
                kpanic("CPU interrupt", frame);
        }
    }
}
