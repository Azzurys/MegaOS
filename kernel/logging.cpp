#include <stdint.h>
#include <logging.hpp>


namespace log
{
    namespace
    {
        TTYWriteFunction write = nullptr;

        size_t strlen(const char* data)
        {
            size_t s = 0;
            for (; data && *data; ++data)
                ++s;

            return s;
        }

        void swap(char* a, char* b)
        {
            char temp = *a;
            *a = *b;
            *b = temp;
        }

        void reverse(char* str, size_t size)
        {
            size_t beg = 0;
            size_t end = size - 1;

            for (; beg < end; ++beg, --end)
                swap(&str[beg], &str[end]);
        }

        void to_upper(char* buffer)
        {
            for (size_t i = 0; buffer && buffer[i]; ++i)
            {
                if (buffer[i] >= '0' && buffer[i] <= '9')
                    continue;

                // clear 5th bit
                buffer[i] &= ~(1 << 5);
            }
        }

        template<typename IntegerType>
        char* itoa(IntegerType value, char* str, int base)
        {
            if (value == 0)
            {
                str[0] = '0';
                str[1] = '\0';

                return str;
            }

            /* standard function ignore sign if base is not 10 */
            bool negative = (value < 0 && base == 10);

            if (negative)
                value = -value;

            size_t i = 0;
            for (; value != 0; ++i, value /= base)
            {
                int remainder = value % base;

                if (remainder > 9)
                    str[i] = (remainder - 10) + 'a';
                else
                    str[i] = remainder + '0';
            }

            if (negative)
                str[i++] = '-';

            str[i] = '\0';

            reverse(str, i);

            return str;
        }
    }

    void init(TTYWriteFunction log_function)
    {
        if (!write)
            write = log_function;
    }

    int puts(const char* data)
    {
        if (!write)
            return -1;

        const size_t size = strlen(data);

        if (size)
            write(data, size);

        return static_cast<int>(size);
    }

    int putc(char c)
    {
        if (!write)
            return -1;

        write(&c, 1);
        return 1;
    }

    int printf_impl(const char* format, va_list args)
    {
        // small and not the cleanest printf implementation

        if (!write)  return -1;
        if (!format) return 0;

        int printed = 0;

        // we have to define these variables here
        // else the switch can't jump
        char char_temp;
        char* str_temp;
        int32_t int32_temp;
        uint64_t uint64_temp;

        // make place to convert number to string
        // 21 is enough to hold 64-bit integers
        char buffer[21];
        
        while (char ch = *format++)
        {
            if (ch == '%')
            {
                switch (ch = *format++)
                {
                    case '%':
                        printed += putc(ch);
                        break;

                    case 'c':
                        char_temp = va_arg(args, int);
                        printed += putc(char_temp);
                        break;

                    case 's':
                        str_temp = va_arg(args, char*);
                        printed += puts(str_temp);
                        break;

                    case 'd':
                        int32_temp = va_arg(args, int32_t);
                        itoa(int32_temp, buffer, 10);
                        printed += puts(buffer);
                        break;

                    case 'x':
                        int32_temp = va_arg(args, int32_t);
                        itoa(int32_temp, buffer, 16);
                        printed += puts(buffer);
                        break;

                    case 'X':
                        int32_temp = va_arg(args, int32_t);
                        itoa(int32_temp, buffer, 16);
                        to_upper(buffer);
                        printed += puts(buffer);
                        break;

                    case 'l':
                        if (*format++ == 'l' && (*format == 'x' || *format == 'X'))
                        {
                            uint64_temp = va_arg(args, uint64_t);
                            itoa(uint64_temp, buffer, 16);

                            if (*format++ == 'X')
                                to_upper(buffer);

                            printed += puts(buffer);
                        }
                        break;
                }
            } else
                printed += putc(ch);
        }

        return printed;
    }

    int printf(const char* format, ...)
    {
        va_list args;
        va_start(args, format);

        int printed = printf_impl(format, args);

        va_end(args);

        return printed;
    }
}


