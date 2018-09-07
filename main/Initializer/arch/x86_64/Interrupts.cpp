/*
 * Interrupts.c
 *
 *  Created on: 06 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#include <arch/x86_64/interrupts.h>
#include <arch/x86_64/ioports.h>
#include <arch/x86_64/exceptions.h>
#include <std/types.h>
#include <std/memory.h>

#define NUM_IDT_ENTRIES 256
#define SYSTEM_CS 0x08
#define IDT_FLAG 0x8E

//IDT 64-bit entries

struct IdtDescriptor
{
    UShort base_low;
    UShort sel;
    UByte zero;
    UByte flags;
    UShort base_middle;
    UInt base_high;
    UInt zero2;
} __attribute__((packed));

struct IdtPointer
{
    UShort limit;
    Address base;
} __attribute__((packed));

static IntHandler interruptHandlers[NUM_IDT_ENTRIES];
struct IdtDescriptor idt[NUM_IDT_ENTRIES];
struct IdtPointer idtp;

extern "C"
{

    extern void defaultIRQ();
    extern void isr0();
    extern void isr1();
    extern void isr2();
    extern void isr3();
    extern void isr4();
    extern void isr5();
    extern void isr6();
    extern void isr7();
    extern void isr8();
    extern void isr9();
    extern void isr10();
    extern void isr11();
    extern void isr12();
    extern void isr13();
    extern void isr14();
    extern void isr15();
    extern void isr16();
    extern void isr17();
    extern void isr18();
    extern void isr19();
    extern void isr20();
    extern void isr21();
    extern void isr22();
    extern void isr23();
    extern void isr24();
    extern void isr25();
    extern void isr26();
    extern void isr27();
    extern void isr28();
    extern void isr29();
    extern void isr30();
    extern void isr31();

    extern void irq0();
    extern void irq1();
    extern void irq2();
    extern void irq3();
    extern void irq4();
    extern void irq5();
    extern void irq6();
    extern void irq7();
    extern void irq8();
    extern void irq9();
    extern void irq10();
    extern void irq11();
    extern void irq12();
    extern void irq13();
    extern void irq14();
    extern void irq15();

    extern void idtLoad();
}

static void remapIRQTable()
{
    setPortByte(0x20, 0x11);
    setPortByte(0xA0, 0x11);
    setPortByte(0x21, 0x20);
    setPortByte(0xA1, 0x28);
    setPortByte(0x21, 0x04);
    setPortByte(0xA1, 0x02);
    setPortByte(0x21, 0x01);
    setPortByte(0xA1, 0x01);
    setPortByte(0x21, 0x0);
    setPortByte(0xA1, 0x0);
}

void idtSetGate(UByte num, Address base, UShort sel, UByte flags)
{
    idt[num].base_low = (base & 0xFFFF);
    idt[num].base_middle = (base >> 16) & 0xFFFF;
    idt[num].base_high = (base >> 32);
    idt[num].sel = sel;
    idt[num].zero = 0x0;
    idt[num].flags = flags;
    idt[num].zero2 = 0x0;
}

/* Installs the IDT */
void setupInterrupts()
{
    /* Sets the special IDT pointer up*/
    idtp.limit = (sizeof(struct IdtDescriptor) * NUM_IDT_ENTRIES) - 1;
    idtp.base = (Address)&idt;

    /* Clear out the entire IDT, initializing it to zeros */
    setMemory(&idt, 0, sizeof(struct IdtDescriptor) * NUM_IDT_ENTRIES);

    // Nullify all the interrupt handlers.
    setMemory(&interruptHandlers, 0, sizeof(IntHandler) * 256);

    remapIRQTable();

    /* Add any new ISRs to the IDT here using idtSetGate */
    idtSetGate(0, (Address)isr0, SYSTEM_CS, IDT_FLAG);
    idtSetGate(1, (Address)isr1, SYSTEM_CS, IDT_FLAG);
    idtSetGate(2, (Address)isr2, SYSTEM_CS, IDT_FLAG);
    idtSetGate(3, (Address)isr3, SYSTEM_CS, IDT_FLAG);
    idtSetGate(4, (Address)isr4, SYSTEM_CS, IDT_FLAG);
    idtSetGate(5, (Address)isr5, SYSTEM_CS, IDT_FLAG);
    idtSetGate(6, (Address)isr6, SYSTEM_CS, IDT_FLAG);
    idtSetGate(7, (Address)isr7, SYSTEM_CS, IDT_FLAG);
    idtSetGate(8, (Address)isr8, SYSTEM_CS, IDT_FLAG);
    idtSetGate(9, (Address)isr9, SYSTEM_CS, IDT_FLAG);
    idtSetGate(10, (Address)isr10, SYSTEM_CS, IDT_FLAG);
    idtSetGate(11, (Address)isr11, SYSTEM_CS, IDT_FLAG);
    idtSetGate(12, (Address)isr12, SYSTEM_CS, IDT_FLAG);
    idtSetGate(13, (Address)isr13, SYSTEM_CS, IDT_FLAG);
    idtSetGate(14, (Address)isr14, SYSTEM_CS, IDT_FLAG);
    idtSetGate(15, (Address)isr15, SYSTEM_CS, IDT_FLAG);
    idtSetGate(16, (Address)isr16, SYSTEM_CS, IDT_FLAG);
    idtSetGate(17, (Address)isr17, SYSTEM_CS, IDT_FLAG);
    idtSetGate(18, (Address)isr18, SYSTEM_CS, IDT_FLAG);
    idtSetGate(19, (Address)isr19, SYSTEM_CS, IDT_FLAG);
    idtSetGate(20, (Address)isr20, SYSTEM_CS, IDT_FLAG);
    idtSetGate(21, (Address)isr21, SYSTEM_CS, IDT_FLAG);
    idtSetGate(22, (Address)isr22, SYSTEM_CS, IDT_FLAG);
    idtSetGate(23, (Address)isr23, SYSTEM_CS, IDT_FLAG);
    idtSetGate(24, (Address)isr24, SYSTEM_CS, IDT_FLAG);
    idtSetGate(25, (Address)isr25, SYSTEM_CS, IDT_FLAG);
    idtSetGate(26, (Address)isr26, SYSTEM_CS, IDT_FLAG);
    idtSetGate(27, (Address)isr27, SYSTEM_CS, IDT_FLAG);
    idtSetGate(28, (Address)isr28, SYSTEM_CS, IDT_FLAG);
    idtSetGate(29, (Address)isr29, SYSTEM_CS, IDT_FLAG);
    idtSetGate(30, (Address)isr30, SYSTEM_CS, IDT_FLAG);
    idtSetGate(31, (Address)isr31, SYSTEM_CS, IDT_FLAG);
    idtSetGate(32, (Address)irq0, SYSTEM_CS, IDT_FLAG);
    idtSetGate(33, (Address)irq1, SYSTEM_CS, IDT_FLAG);
    idtSetGate(34, (Address)irq2, SYSTEM_CS, IDT_FLAG);
    idtSetGate(35, (Address)irq3, SYSTEM_CS, IDT_FLAG);
    idtSetGate(36, (Address)irq4, SYSTEM_CS, IDT_FLAG);
    idtSetGate(37, (Address)irq5, SYSTEM_CS, IDT_FLAG);
    idtSetGate(38, (Address)irq6, SYSTEM_CS, IDT_FLAG);
    idtSetGate(39, (Address)irq7, SYSTEM_CS, IDT_FLAG);
    idtSetGate(40, (Address)irq8, SYSTEM_CS, IDT_FLAG);
    idtSetGate(41, (Address)irq9, SYSTEM_CS, IDT_FLAG);
    idtSetGate(42, (Address)irq10, SYSTEM_CS, IDT_FLAG);
    idtSetGate(43, (Address)irq11, SYSTEM_CS, IDT_FLAG);
    idtSetGate(44, (Address)irq12, SYSTEM_CS, IDT_FLAG);
    idtSetGate(45, (Address)irq13, SYSTEM_CS, IDT_FLAG);
    idtSetGate(46, (Address)irq14, SYSTEM_CS, IDT_FLAG);
    idtSetGate(47, (Address)irq15, SYSTEM_CS, IDT_FLAG);
    for (int i = 48; i < NUM_IDT_ENTRIES; i++)
    {
        idtSetGate(i, (Address)defaultIRQ, SYSTEM_CS, IDT_FLAG);
    }
    idtLoad();
}

String exception_messages[] = {"Division By Zero", "Debug",
                               "Non Maskable Interrupt", "Breakpoint", "Into Detected Overflow",
                               "Out of Bounds", "Invalid Opcode", "No Coprocessor", "Double Fault!",
                               "Coprocessor Segment Overrun", "Bad TSS", "Segment Not Present",
                               "Stack Fault", "General Protection Fault!", "Page Fault",
                               "Unknown Interrupt", "Coprocessor Fault", "Alignment Check Exception",
                               "Machine Check Exception", "SIMD fp Exception",
                               "Virtualization Exception", "Reserved", "Reserved", "Reserved",
                               "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
                               "Reserved"};

extern "C"
{

    void isrHandler(RegistersState *state)
    {
        const ULong int_no = state->int_no;
        if (int_no < 32)
        {
            kexception(state, exception_messages[int_no]);
        }
        else
        {
            if (interruptHandlers[int_no])
            {
                IntHandler handler = interruptHandlers[int_no];
                handler();
            }
            else
                kpanic("Unhandled interrupt!");
        }
    }

    void irqHandler(RegistersState *state)
    {
        // Send an EOI (end of interrupt) signal to the PICs.
        // If this interrupt involved the slave.
        if (state->int_no >= 40)
        {
            // Send reset signal to slave.
            setPortByte(0xA0, 0x20);
        }
        // Send reset signal to master. (As well as slave, if necessary).
        setPortByte(0x20, 0x20);

        if (interruptHandlers[state->int_no])
        {
            IntHandler handler = interruptHandlers[state->int_no];
            handler();
        }
    }

    void defHandler()
    {
        kpanic("Interrupt on unsettled gate!");
    }
}

void setInterruptHandler(UByte num, IntHandler handler)
{
    interruptHandlers[num] = handler;
}