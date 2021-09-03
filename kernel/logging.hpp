#ifndef MEGAOS_LOGGING_HPP
#define MEGAOS_LOGGING_HPP


#include <stdarg.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


    /*
     * printf family implementation taken from https://github.com/mpaland/printf
     * removed checks for FLAGS_UPPERCASE in hex conversion to not make the 'x'
     * in '0x' uppercase when using the hash flag.
     */


namespace log
{
    namespace
    {
        using TTYWriteFunction = void(*)(const char*, size_t);

        void init(TTYWriteFunction log_function);
    }

/**
 * Output a character to a custom device like UART, used by the printf() function
 * This function is declared here only. You have to write your custom implementation somewhere
 * \param character Character to output
 */
    void putchar(char character);


    int puts(const char* string);
    int print(const char* string);

/**
 * Tiny printf implementation
 * You have to implement _putchar if you use printf()
 * To avoid conflicts with the regular printf() API it is overridden by macro defines
 * and internal underscore-appended functions like printf() are used
 * \param format A string that specifies the format of the output
 * \return The number of characters that are written into the elements, not counting the terminating null character
 */

    int printf(const char *format, ...);


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
 * printf with output function
 * You may use this as dynamic alternative to printf() with its fixed _putchar() output
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
