
#include <limits.h>
#include <std/strings.h>
#include <std/format.h>
#include <stddef.h>
#include <stdarg.h>

using namespace Std;

void Std::printf(OutputStream *os, String __restrict format, ...) {
    va_list parameters;
    va_start(parameters, format);

    char buffer[20];

    while (*format != '\0') {

        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;

            os->write(format, amount);
            format += amount;
            continue;
        }

        const String format_begun_at = format++;
        if (*format == 'c') {
            format++;
            int ic = va_arg(parameters, int /* char promotes to int */);
            os->write(ic);
        } else if (*format == 's') {
            format++;
            const String str = va_arg(parameters, char*);
            size_t len = strlen(str);
            os->write(str, len);
        } else if (*format == 'b') {
            format++;
            int integer = va_arg(parameters, int);
            char bbuffer[65];
            int_to_bstr(integer, bbuffer);
            size_t len = strlen(bbuffer);
            os->write(bbuffer, len);
        } else if (*format == 'd' || *format == 'u' || *format == 'i') {
            format++;
            int integer = va_arg(parameters, int);
            int_to_dstr(integer, buffer);
            size_t len = strlen(buffer);
            os->write(buffer, len);
        } else if (*format == 'x' || *format == 'X') {
            //%X = hex format with 16-digit
            unsigned integer = va_arg(parameters, unsigned);
            int_to_hstr(integer, buffer);
            size_t len = strlen(buffer);
            if (*format == 'X')
                for (size_t i = 0; i < 2 * 8 - len; i++)
                    os->write('0');
            os->write(buffer, len);
            format++;
        } else {
            format = format_begun_at;
            size_t len = strlen(format);
            os->write(format, len);
            format += len;
        }
    }
    va_end(parameters);
}