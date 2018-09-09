/*
 * bochs.h
 *
 *  Created on: 09 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

// --- Enable BOCHS debuging support ---
#ifndef BOCHS_DEBUG
#define BOCHS_DEBUG
#endif
//-----

#ifdef BOCHS_DEBUG

#ifndef INCLUDE_BOCHS_DEBUG

#define BOCHS_PRINT_CHAR(c) setPortByte(0xe9, c)
#define BOCHS_SET_STD_OUT() System::setStdOut(BochsOutputStream::instance())
#define BOCHS_BREAK() setPortWord(0x8A00,0x8A00); setPortWord(0x8A00,0x08AE0);

#include <std/streams.h>
#include <sys/ioports.h>

class BochsOutputStream : public Std::OutputStream
{
  public:
    BochsOutputStream() {}
    virtual void write(int ch) { BOCHS_PRINT_CHAR(ch); }
    static BochsOutputStream *instance()
    {
        static BochsOutputStream bos;
        return &bos;
    };
};

#endif //INCLUDE_BOCHS_DEBUG

#endif //BOCHS_DEBUG
