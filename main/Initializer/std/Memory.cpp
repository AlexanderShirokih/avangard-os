/*
 * Memory.cpp
 *
 *  Created on: 06 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#include <stddef.h>
#include <std/memory.h>
#include <std/types.h>

Pointer copyMemory(Pointer __restrict dstptr, const Pointer __restrict srcptr,
				   size_t size)
{
	UByte *dst = (UByte *)dstptr;
	const unsigned char *src = (const UByte *)srcptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

Pointer setMemory(Pointer bufptr, Int value, size_t size)
{
	UByte *buf = (UByte *)bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (UByte)value;
	return bufptr;
}

Pointer moveMemory(Pointer dstptr, const Pointer srcptr, size_t size)
{
	UByte *dst = (UByte *)dstptr;
	const UByte *src = (const UByte *)srcptr;
	if (dst < src)
	{
		for (size_t i = 0; i < size; i++)
			dst[i] = src[i];
	}
	else
	{
		for (size_t i = size; i != 0; i--)
			dst[i - 1] = src[i - 1];
	}
	return dstptr;
}

Int compareMemory(const Pointer aptr, const Pointer bptr, size_t size)
{
	const UByte *a = (const UByte *)aptr;
	const UByte *b = (const UByte *)bptr;
	for (size_t i = 0; i < size; i++)
	{
		if (a[i] < b[i])
			return -true;
		else if (b[i] < a[i])
			return true;
	}
	return false;
}
