/*
 * IOports.cpp
 *
 *  Created on: 06 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#include <sys/ioports.h>

UByte getPortByte(UShort port)
{
	UByte ret;

	__asm__ __volatile__("inb %1, %0"
						 : "=a"(ret)
						 : "dN"(port));
	return ret;
}

void setPortByte(UShort port, UByte value)
{
	__asm__ __volatile__("outb %1, %0"
						 :
						 : "dN"(port), "a"(value));
}

UShort getPortWord(UShort port)
{
	UShort ret;

	__asm__ __volatile__("inw %1, %0"
						 : "=a"(ret)
						 : "dN"(port));
	return ret;
}

void setPortWord(UShort port, UShort value)
{
	__asm__ __volatile__("outw %1, %0"
						 :
						 : "dN"(port), "a"(value));
}