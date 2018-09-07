/*
 * IOports.c
 *
 *  Created on: 06 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#include <arch/x86_64/ioports.h>

//ENTRY(inb)  unsigned inb(U16_t port); - from MINIX
//	push	%ebp
//	movl	%esp, %ebp
//	movl	8(%ebp), %edx	/* port */
//	xorl	%eax, %eax
//	inb	%dx	/* read 1 byte */
//	pop	%ebp
//

UByte getPortByte(UShort port)
{
	UByte ret;
	__asm__ __volatile__("inb %1, %0"
						 : "=a"(ret)
						 : "dN"(port));
	return ret;
}

//ENTRY(outb)- void outb(U16_t port, U8_t value);  - from MINIX
//	push	%ebp
//	movl	%esp, %ebp
//	movl	8(%ebp), %edx	/* port */
//	movl	8+4(%ebp), %eax	/* value */
//	outb	%dx	/* output 1 byte */
//	pop	%ebp
//

void setPortByte(UShort port, UByte value)
{
	__asm__ __volatile__("outb %1, %0"
						 :
						 : "dN"(port), "a"(value));
}
