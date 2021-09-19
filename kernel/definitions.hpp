#ifndef MEGAOS_DEFINITIONS_HPP
#define MEGAOS_DEFINITIONS_HPP

#define PACKED __attribute__((packed))
#define ASM_DEFINED extern "C"

#ifdef MEGA_RELEASE
#define assert(expr)
#else
#include <logging.hpp>
#include <arch/x86_64/cpu.hpp>

[[noreturn]] static void assertion_failed(const char* expr_string, const char* file, int line)
{
    log::dprintf(RED "Assertion failed: %s at %s, line %d", expr_string, file, line);
    cpu::halt();
}

#define assert(expr) ((expr) || (assertion_failed(#expr, __FILE__, __LINE__), 0))
#endif


#endif //MEGAOS_DEFINITIONS_HPP
