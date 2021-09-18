#include <serial.hpp>
#include <arch/x86_64/cpu.hpp>


namespace serial
{
    bool test()
    {
        using namespace cpu;

        io::outb(regs::MODM_CR, 0x1E);  // set loop back mode
        io::outb(regs::DATA,    0xAE);  // send dummy test byte

        /* check if the serial echoes the byte correctly */
        if (io::inb(regs::DATA) != 0xAE)
            return false;

        /* if so, reset to normal operation mode */
        io::outb(regs::MODM_CR, 0x0F);

        return true;
    }

    bool init()
    {
        using namespace cpu;

        io::outb(regs::INTE,    0x00);   // disable all interrupts
        io::outb(regs::LINE_CR, 0x80);   // set msb of the line control register, which is DLAB
        io::outb(regs::DIVL,    0x03);   // DLAB is now set, set low
        io::outb(regs::DIVH,    0x00);   // and high byte of the divisor
        io::outb(regs::LINE_CR, 0x03);   // 8 bits, no parity, 1 stop bit
        io::outb(regs::INTI,    0xC7);   // enable FIFO
        io::outb(regs::MODM_CR, 0x0B);   // IRQS enabled

        return test();
    }

    void send(char c)
    {
        /* check if the transmitter is not doing anything */
        while (!(cpu::io::inb(regs::LINE_SR) & TEMT_MASK));

        cpu::io::outb(regs::DATA, c);
    }
}
