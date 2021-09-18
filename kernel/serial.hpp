#ifndef MEGAOS_SERIAL_HPP
#define MEGAOS_SERIAL_HPP


#include <stdint.h>


namespace serial
{
    /* 6th bit set if the transmitter is not doing anything */
    constexpr uint8_t TEMT_MASK = 1 << 5;

    enum ports : uint16_t
    {
        COM1 = 0x3F8,
    };

    // data registers mapped to the ports
    enum regs : uint16_t
    {
        DATA    = ports::COM1 + 0,    // data register, read/write to transmit buffer
        INTE    = ports::COM1 + 1,    // interrupt enable register
        DIVL    = ports::COM1 + 0,    // DLAB 1: low  byte of divisor value when setting the baud rate
        DIVH    = ports::COM1 + 1,    // DLAB 1: high byte of divisor value when setting the baud rate
        INTI    = ports::COM1 + 2,    // interrupt identification and FIFO control registers
        LINE_CR = ports::COM1 + 3,    // line control register, msb is DLAB
        MODM_CR = ports::COM1 + 4,    // modem control register
        LINE_SR = ports::COM1 + 5,    // line status register
        MODM_SR = ports::COM1 + 6,    // modem status register
        SCRATCH = ports::COM1 + 7,    // scratch register
    };

    bool init();
    void send(char c);
}


#endif //MEGAOS_SERIAL_HPP
