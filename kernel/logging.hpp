#ifndef MEGAOS_LOGGING_HPP
#define MEGAOS_LOGGING_HPP


#include <stdarg.h>
#include <stddef.h>
#include <stivale2.hpp>


#ifdef __cplusplus
extern "C" {
#endif


    /*
     * printf family implementation taken from https://github.com/mpaland/printf
     * removed checks for FLAGS_UPPERCASE in hex conversion to not make the 'x'
     * in '0x' uppercase when using the hash flag.
     */

#define RED "\033[31m"

namespace log
{
    /* initialize logging and serial communication */
    bool init(st2::st2_struct* boot_info);

    /* print to kernel's terminal */
    void kputc(char c);
    int  kputs(const char* string);
    int  kprint(const char* string);
    int  kprintf(const char *format, ...);

    /* print to host's terminal via serial communication */
    void dputc(char c);
    int  dputs(const char* string);
    int  dprint(const char* string);
    int  dprintf(const char *format, ...);

/**
 * Tiny sprintf implementation
 * Due to security reasons (buffer overflow) YOU SHOULD CONSIDER USING (V)SNPRINTF INSTEAD!
 * \param buffer A pointer to the buffer where to store the formatted string. MUST be big enough to store the output!
 * \param format A string that specifies the format of the output
 * \return The number of characters that are WRITTEN into the buffer, not counting the terminating null character
 */

    int sprintf(char *buffer, const char *format, ...);


/**
 * Tiny snprintf/vsnprintf implementation
 * \param buffer A pointer to the buffer where to store the formatted string
 * \param count The maximum number of characters to store in the buffer, including a terminating null character
 * \param format A string that specifies the format of the output
 * \param va A value identifying a variable arguments list
 * \return The number of characters that COULD have been written into the buffer, not counting the terminating
 *         null character. A value equal or larger than count indicates truncation. Only when the returned value
 *         is non-negative and less than count, the string has been completely written.
 */
    int snprintf(char *buffer, size_t count, const char *format, ...);

    int vsnprintf(char *buffer, size_t count, const char *format, va_list va);


/**
 * Tiny vprintf implementation
 * \param format A string that specifies the format of the output
 * \param va A value identifying a variable arguments list
 * \return The number of characters that are WRITTEN into the buffer, not counting the terminating null character
 */

    int vprintf(const char *format, va_list va);


/**
 * kprintf with output function
 * You may use this as dynamic alternative to kprintf() with its fixed _putchar() output
 * \param out An output function which takes one character and an argument pointer
 * \param arg An argument pointer for user data passed to output function
 * \param format A string that specifies the format of the output
 * \return The number of characters that are sent to the output function, not counting the terminating null character
 */
    int fctprintf(void (*out)(char character, void *arg), void *arg, const char *format, ...);
}

#ifdef __cplusplus
}
#endif


#endif //MEGAOS_LOGGING_HPP
