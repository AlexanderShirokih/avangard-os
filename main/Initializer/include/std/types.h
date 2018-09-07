/*
 * types.h
 *
 *  Created on: 3 янв. 2018 г.
 *      Author: Alexander Shirokih
 */

#ifndef INCLUDE_STD_TYPES_H_
#define INCLUDE_STD_TYPES_H_

//typedef unsigned long* Pointer64;

typedef const char *String;

typedef unsigned char UByte; //unsigned 8bit
typedef signed char Byte; //signed 8bit

typedef unsigned short UShort; //unsigned 16bit
typedef signed short Short; //signed 8bit
typedef UShort Char;

typedef unsigned int UInt; //unsigned 32bit
typedef signed int Int; //signed 8bit

typedef unsigned long ULong; //unsigned 64bit
typedef signed long Long; //signed 8bit

typedef ULong Address;

typedef UByte Bool;

typedef void *Pointer;

typedef ULong Size;

#define true  1
#define false 0
#define null 0

#endif /* INCLUDE_STD_TYPES_H_ */
