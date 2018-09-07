/*
 * interrupts.h
 *
 *  Created on: 6 янв. 2018 г.
 *      Author: Alexander Shirokih
 */

#ifndef INCLUDE_INTERRUPTS_H_
#define INCLUDE_INTERRUPTS_H_

#include <std/types.h>

#define INT_ON asm("sti");
#define INT_OFF asm("cli");
#define HALT asm("hlt");

typedef struct {
    ULong rax, rbx, rcx, rdx, rsi, rdi, rbp;
    ULong r8, r9, r10, r11, r12, r13, r14, r15;
    ULong int_no, err_code;
    ULong rip;
    ULong cs;
    ULong rflags;
    ULong rsp;
    ULong ss;
}__attribute__((packed)) RegistersState;

typedef void (*IntHandler)(void);

void idtSetGate(UByte num, Address base, UShort sel, UByte flags);

void setupInterrupts();

void setInterruptHandler(UByte num, IntHandler handler);

#endif