//
// Created by Alexander Shirokih on 13.06.18.
//
#include <std/types.h>
#include <std/strings.h>
#include <stddef.h>

using namespace Std;

size_t Std::strlen(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

static void int_to_str(long num, int base, char buff[]) {
    int i = 12;
    int j = 0;

    if (num < 0) {
        num = -num;
        buff[0] = '-';
        ++j;
    }

    do {
        buff[i] = "0123456789ABCDEF"[num % base];
        i--;
        num = num / base;
    } while (num > 0);

    while (++i < 13) {
        buff[j++] = buff[i];
    }

    buff[j] = 0;
}

void Std::int_to_dstr(long n, char buff[]) {
    int_to_str(n, 10, buff);
}

void Std::int_to_hstr(unsigned long num, char buff[]) {
    int_to_str(num, 16, buff);
}

void Std::int_to_bstr(unsigned long num, char buff[]) {
    int_to_str(num, 2, buff);
}

int Std::str_to_int(String s) {
    while (*s && (*s == '\n' || *s == '\t' || *s == ' '))
        ++s;

    int n = 0;
    while (*s >= '0' && *s <= '9')
        n = n * 10 + (*s++ - '0');

    return n;
}

int Std::strcmp(String s1, String s2) {
    while (*s1 && (*s1 == *s2))
        s1++, s2++;
    return *(const unsigned char *) s1 - *(const unsigned char *) s2;
}

char* Std::strcat(char *dest, String src) {
    strcpy(dest + strlen(dest), src);
    return dest;
}

char* Std::strchr(String str, int c) {
    for (; *str; str++)
        if (*str == c)
            return (char *) ++str;
    return (char*) 0;
}

char* Std::strcpy(char* dest, String src) {
    char* result = dest;
    while ((*dest++ = *src++));
    return result;
}