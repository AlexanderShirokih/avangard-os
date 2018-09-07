/*
 * memory.h
 *
 *  Created on: 06 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#ifndef INCLUDE_MEM_H_
#define INCLUDE_MEM_H_

#include <stddef.h>
#include <std/types.h>

Pointer copyMemory(Pointer __restrict dstptr, const Pointer __restrict srcptr, size_t size);

Pointer setMemory(Pointer bufptr, Int value, size_t size);

Pointer moveMemory(Pointer dstptr, const Pointer srcptr, size_t size);

Int compareMemory(const Pointer aptr, const Pointer bptr, size_t size);


// Pointer kMemAlloc(Size size);
// void kFree(Pointer ptr);

#endif
