#include <arch/x86_64/cpu.hpp>
#include <panic.hpp>
#include <logging.hpp>


[[noreturn]] void kpanic(const char* cause)
{
    log::kputs(RED "KERNEL PANIC OCCURED, CHECK LOGS ON HOST MACHINE !");
    log::dprintf(RED "KERNEL PANIC: %s\n", cause);
    cpu::halt();
}

[[noreturn]] void kpanic(const char* cause, cpu::registers* r)
{
    if (r)
    {
        const char* format = "General Purpose Registers:\n"
            "\t- rax = %#lX\n"
            "\t- rbx = %#lX\n"
            "\t- rcx = %#lX\n"
            "\t- rdx = %#lX\n"
            "\t- rbp = %#lX\n"
            "\t- rdi = %#lX\n"
            "\t- rsi = %#lX\n"
            "\t- r8  = %#lX\n"
            "\t- r9  = %#lX\n"
            "\t- r10 = %#lX\n"
            "\t- r11 = %#lX\n"
            "\t- r12 = %#lX\n"
            "\t- r13 = %#lX\n"
            "\t- r14 = %#lX\n"
            "\t- r15 = %#lX\n\n";

        log::dprintf(format,
                     r->rax, r->rbx, r->rcx, r->rdx,
                     r->rbp, r->rdi, r->rsi, r->r8, r->r9,
                     r->r10, r->r11, r->r12, r->r13, r->r14, r->r15);
    }

    kpanic(cause);
}

[[noreturn]] void kpanic(const char* cause, interrupts::interrupt_frame* f)
{
    cpu::registers* stack_frame = nullptr;

    if (f)
    {
        stack_frame = &f->GPRs;

        const char* format = "Interrupt Stack Frame:\n"
                             "\t- err_code = %d\n"
                             "\t- int_no   = %d\n"
                             "\t- rip      = %#lX\n"
                             "\t- cs       = %#lX\n"
                             "\t- rflags   = %#lX\n"
                             "\t- rsp      = %#lX\n"
                             "\t- ss       = %#lX\n\n";

        log::dprintf(format, f->err_code, f->int_no, f->rip,
                     f->cs, f->rflags, f->rsp, f->ss);

        if (f->int_no < interrupts::EXCEPTION_COUNT)
            log::dprintf("Interrupt description: %s\n",
                         interrupts::exception_desc[f->int_no]);
    }

    kpanic(cause, stack_frame);
}
