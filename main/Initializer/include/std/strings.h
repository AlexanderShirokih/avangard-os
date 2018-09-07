//
// Created by Alexander Shirokih on 13.06.18.
//
#ifndef AVANGARDOS_STRINGS_H
#define AVANGARDOS_STRINGS_H

#include <std/types.h>
#include <stddef.h>

namespace Std {

    size_t strlen(const char* str);
    char* strcpy(char* dest, String src);

    void int_to_dstr(long n, char buff[]);
    void int_to_hstr(unsigned long n, char buff[]);
    void int_to_bstr(unsigned long n, char buff[]);
//
    int str_to_int(String s);
//
    int strcmp(String s1, String s2);
//
    char* strcat(char* dest, String src);
    char* strchr(String str, int c);
}

#endif