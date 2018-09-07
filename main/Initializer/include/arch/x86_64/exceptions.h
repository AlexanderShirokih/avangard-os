/*
 * exceptions.h
 *
 *  Created on: 6 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#ifndef INCLUDE_EXCEPTIONS_H_
#define INCLUDE_EXCEPTIONS_H_

#include <arch/x86_64/interrupts.h>
#include <sys/system.h>
#include <std/format.h>

#define kpanic(msg) kpanicAt(msg, __FILE__, __LINE__);
#define REGDUMPB(out, rn, r) Std::printf(out, "%s:  0x%X\n", rn, r);
#define REGDUMP2(out, rn, r, r2n, r2) Std::printf(out, (String)"%s:  0x%X  |  %s:  0x%X\n", rn, r, (String)r2n, r2);
#define NEWLINE(out) out->write('\n')

void dump_regstate(RegistersState *reg_state);

extern "C"
{
    void kpanicAt(String msg, String file, const unsigned int line);
    void kexception(RegistersState *reg_state, String exeptionMsg);
}

#endif