/*
 * Exceptions.cpp
 *
 *  Created on: 06 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#include <arch/x86_64/interrupts.h>
#include <arch/x86_64/exceptions.h>
#include <std/types.h>
#include <sys/system.h>

void kpanicAt(const char* msg, const char* file, const unsigned int line)
{
    INT_OFF

    ULong rax, rbx, rcx, rdx, rdi, rsi, rbp, rsp, r8, r9, r10, r11, r12, r13,
        r14, r15, cr0, cr2, cr3; //rip, rflags

    Std::OutputStream* stdOut = System::getStdOut();

    asm("mov %%rax, %0;\
		 	mov %%rbx, %1;\
		 	mov %%rcx, %2;\
			mov %%rdx, %3;\
		 	mov %%rdi, %4;\
		 	mov %%rsi, %5;\
		 	mov %%rbp, %6;\
		 	mov %%rsp, %7;\
		 	mov %%r8, %8;\
		 	mov %%r9, %9;\
		 	mov %%r10, %10;\
			mov %%r11, %11;\
			mov %%r12, %12;\
			mov %%r13, %13;\
			mov %%r14, %14;\
			mov %%r15, %15;\
			mov %%cr0, %16;\
			mov %%cr2, %17;\
			mov %%cr3, %18;\
		 	"
        : "=g"(rax),
          "=g"(rbx),
          "=g"(rcx),
          "=g"(rdx),
          "=g"(rdi),
          "=g"(rsi),
          "=g"(rbp),
          "=g"(rsp),
          "=g"(r8),
          "=g"(r9),
          "=g"(r10),
          "=g"(r11),
          "=g"(r12),
          "=g"(r13),
          "=g"(r14),
          "=g"(r15),
          "=g"(cr0),
          "=g"(cr2),
          "=g"(cr3));
    printf(stdOut, "--> KERNEL PANIC at file: %s, in line: %d <--\n", file, line);
    printf(stdOut, "--> MSG=%s <--\n", msg);
    printf(stdOut, "--> General register state: <--\n");
    REGDUMP2(stdOut, "RAX", rax, "RBX", rbx);
    REGDUMP2(stdOut, "RCX", rcx, "RDX", rdx);
    REGDUMP2(stdOut, "RSI", rsi, "RSP", rsp);
    REGDUMP2(stdOut, "RDI", rdi, "RBP", rbp);
    NEWLINE(stdOut);
    REGDUMP2(stdOut, "R8 ", r8, "R9 ", r9);
    REGDUMP2(stdOut, "R10", r10, "R11", r11);
    REGDUMP2(stdOut, "R12", r13, "R13", r13);
    REGDUMP2(stdOut, "R14", r14, "R15", r15);
    NEWLINE(stdOut);
    REGDUMPB(stdOut, "CR0", cr0);
    REGDUMPB(stdOut, "CR2", cr2);
    REGDUMPB(stdOut, "CR3", cr3);

    HALT
}

extern "C" void kexception(RegistersState *regs, String ex_msg)
{
    Std::OutputStream* stdOut = System::getStdOut();

    printf(stdOut, "--> FATAL EXCEPTION IN KERNEL <--\n");
    printf(stdOut, "--> MSG: %s <--\nGeneral registers state:\n", ex_msg);
    dump_regstate(regs);
    HALT
}

void dump_regstate(RegistersState *regs)
{


    Std::OutputStream* stdOut = System::getStdOut();

    REGDUMP2(stdOut, "RAX", regs->rax, "RBX", regs->rbx);
    REGDUMP2(stdOut, "RCX", regs->rcx, "RDX", regs->rdx);
    NEWLINE(stdOut);
    REGDUMP2(stdOut, "RSI", regs->rsi, "RSP", regs->rsp);
    REGDUMP2(stdOut, "RDI", regs->rdi, "RBP", regs->rbp);
    NEWLINE(stdOut);
    REGDUMP2(stdOut, "R8 ", regs->r8, "R9 ", regs->r9);
    REGDUMP2(stdOut, "R10", regs->r10, "R11", regs->r11);
    REGDUMP2(stdOut, "R12", regs->r13, "R13", regs->r13);
    REGDUMP2(stdOut, "R14", regs->r14, "R15", regs->r15);
    NEWLINE(stdOut);
    REGDUMP2(stdOut, "CS ", regs->cs, "SS ", regs->ss);
    NEWLINE(stdOut);
    REGDUMPB(stdOut, "RIP", regs->rip);
    NEWLINE(stdOut);
    REGDUMPB(stdOut, "RFLAGS", regs->rflags);
    REGDUMPB(stdOut, "INT_NO", regs->int_no);
    REGDUMPB(stdOut, "ERR_NO", regs->err_code);
}