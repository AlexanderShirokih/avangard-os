/*
 * system.h
 *
 *  Created on: 5 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#ifndef INCLUDE_SYSTEM_H_
#define INCLUDE_SYSTEM_H_

#include <std/streams.h>

#define STDOUT *getStdOut()
#define STDIN *getStdIn()

namespace System
{

void setStdIn(Std::InputStream *is);

void setStdOut(Std::OutputStream *os);

Std::OutputStream *getStdOut();

Std::InputStream *getStdIn();

} // namespace System

#endif