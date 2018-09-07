#ifndef _FORMAT_H
#define _FORMAT_H 1

#include <std/types.h>
#include <std/streams.h>

#define kprint(string, ...) *System::getStdOut()<<__VA_OPT__(<<) __VA_ARGS__
#define kprintf(format, ...) Std::printf(System::getStdOut(), format, __VA_ARGS__)
#define kprintln(str) *System::getStdOut()<<str<<'\n'
#define kerror(str) *System::getStdOut()<<str<<'\n'; while(1);
#define EOF (-1)

namespace Std {

    void printf(OutputStream *os, String __restrict str, ...);

}
#endif
