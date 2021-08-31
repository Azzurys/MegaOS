#ifndef MEGAOS_PANIC_HPP
#define MEGAOS_PANIC_HPP


#include <arch/x64/cpu.hpp>
#include <arch/x64/interrupts.hpp>


[[noreturn]] void kpanic(const char* cause);
[[noreturn]] void kpanic(const char* cause, cpu::registers* r);
[[noreturn]] void kpanic(const char* cause, interrupts::interrupt_frame* f);

#endif //MEGAOS_PANIC_HPP
