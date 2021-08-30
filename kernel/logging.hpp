#ifndef MEGAOS_LOGGING_HPP
#define MEGAOS_LOGGING_HPP


#include <stdarg.h>
#include <stddef.h>


namespace log
{
    using TTYWriteFunction = void(*)(const char*, size_t);

    void init(TTYWriteFunction log_function);
    int puts(const char* data);
    int putc(char c);
    int printf(const char* format, ...);
}



#endif //MEGAOS_LOGGING_HPP
