/*
 * arch_init.h
 *
 *  Created on: 5 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#ifndef INCLUDE_ARCH_INIT_H_
#define INCLUDE_ARCH_INIT_H_

#include <std/types.h>

namespace System
{

class ArchInit
{
  public:
    static void init(const Address mbootAddr);
};

void parseMultibootTags(Address mboot_addr);
} // namespace System

#endif