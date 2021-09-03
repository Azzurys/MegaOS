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
        inline uint8_t inb(uint16_t port)
        {
            uint8_t value;
            __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
            return value;
        }
        
        inline uint16_t inw(uint16_t port)
        {
            uint16_t value;
            __asm__ volatile ("inw %1, %0" : "=a"(value) : "Nd"(port));
            return value;
        }
        
        inline uint32_t inl(uint16_t port)
        {
            uint32_t value;
            __asm__ volatile ("inl %1, %0" : "=a"(value) : "Nd"(port));
            return value;
        }
        
        inline void outb(uint16_t port, uint8_t value)
        {
            __asm__ volatile ("outb %0, %1" ::"a"(value), "Nd"(port));
        }
        
        inline void outw(uint16_t port, uint16_t value)
        {
            __asm__ volatile ("outw %0, %1" ::"a"(value), "Nd"(port));
        }
        
        inline void outl(uint16_t port, uint32_t value)
        {
            __asm__ volatile ("outl %0, %1" ::"a"(value), "Nd"(port));
        }

        /*!
         * @brief IO operation on arbitrary unused port.
         *        This act like it waits a very small amount of time (1-4 microseconds).
         *        Mainly used for PIC remapping to let the chip process inputs.
         */
        inline void wait()
        {
            outb(0x80, 0);
        }
    }
}


#endif //MEGAOS_CPU_HPP
