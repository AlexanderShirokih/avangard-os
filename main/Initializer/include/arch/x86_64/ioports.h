/*
 * ioports.h
 *
 *  Created on: 06 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#ifndef INCLUDE_IOPORTS_H_
#define INCLUDE_IOPORTS_H_

#include <std/types.h>

UByte getPortByte(UShort port);

void setPortByte(UShort port, UByte value);

#endif
