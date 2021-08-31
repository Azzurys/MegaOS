#ifndef MEGAOS_CPU_HPP
#define MEGAOS_CPU_HPP


#include <stdint.h>
#include <utils.hpp>


namespace cpu
{
    struct registers
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
    } PACKED;

    namespace io
    {

    }
}


#endif //MEGAOS_CPU_HPP