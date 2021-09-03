#include <devices/ps2keyboard.hpp>
#include <arch/x64/cpu.hpp>
#include <logging.hpp>


namespace ps2::keyboard
{
    uint8_t controller::key_up = 0;

    // Fortunately, we will not need to send any command, this
    // allows us to make a very straight forward ps2 keyboard implementation
    // using the interrupt requests.
    void controller::irq_handler([[maybe_unused]] interrupts::interrupt_frame* frame)
    {
        constexpr uint16_t data_port = 0x60;
        const uint8_t code = cpu::io::inb(data_port);

        switch (code)
        {
            /* handle special cases */
            case 0x2A: key_up |= 0b01; return;  // left shift pressed
            case 0xAA: key_up &= 0b10; return;  // left shift released
            case 0x3A: key_up ^= 0b10; return;  // caps lock pressed

            case 0x0E: log::print("\b \b");   return;  // backspace
            case 0x0F: log::putchar('\t'); return;  // tab
            case 0x1C: log::putchar('\n'); return;  // newline

            default: break;
        }

        if (code < 58)
        {
            const auto& kmap = key_up ? kmap_upper : kmap_lower;
            log::putchar(kmap[code]);
        }
    }
}