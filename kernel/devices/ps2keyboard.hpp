#ifndef MEGAOS_PS2KEYBOARD_HPP
#define MEGAOS_PS2KEYBOARD_HPP


#include <mkl/array.hpp>
#include <arch/x86_64/interrupts.hpp>


namespace ps2::keyboard
{
    class controller
    {
    public:
        static void irq_handler([[maybe_unused]] interrupts::interrupt_frame* frame);

    private:
        static uint8_t key_up;

        static constexpr mkl::array<char, 58> kmap_lower {
            '\0', '\0', '&', '\0', '"',  '\'', '(', '-',  '\0', '_', '\0', '\0',
            ')', '=', '\b', '\t', 'a',  'z', 'e', 'r',  't', 'y', 'u', 'i',
            'o', 'p', '^', '$', '\0',  '\0', 'q', 's',  'd', 'f', 'g', 'h',
            'j', 'k', 'l', 'm', '\0', '`', '\0', '*', 'w', 'x', 'c', 'v',
            'b', 'n', ',', ';', ':',  '!', '\0', '\0', '\0', ' '
        };

        static constexpr mkl::array<char, 58> kmap_upper {
            '\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
            '\0', '+', '\b', '\t', 'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I',
            'O', 'P', '\0', '}', '\0', '\0', 'Q', 'S', 'D', 'F', 'G', 'H',
            'J', 'K', 'L', 'M', '%', '~', '\0', '\\', 'W', 'X', 'C', 'V',
            'B', 'N', '?', '.', '/',  '\0', '\0', '\0', '\0', ' '
        };
    };

}


#endif //MEGAOS_PS2KEYBOARD_HPP
