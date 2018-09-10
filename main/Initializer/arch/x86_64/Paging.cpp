/*
 * Paging.cpp
 *
 *  Created on: 08 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#include <arch/x86_64/paging.h>

#include <std/format.h>

void System::PageEntry::print(Std::OutputStream *out)
{
    Std::printf(out, "PageEntry[prsnt=%u, wtr=%u, usr=%u, wrtThr=%u, nCache=%u, acc=%u, drty=%u, huge|Null=%u, glb=%u, noEx=%u, physAddr=%X]\n", isPresent, isWritable, isUserAccessible, isWriteThroughCaching, isNoCache, isAccessed, isDirty, isHugeOrNull, isGlobal, isNoExecute, physicalAddress);
}

/**
 * Returns next level pointer of PageEntry or null this entry is a huge page or not present
 */
System::PageEntry *System::PageEntry::getNextLevelEntry(UInt index)
{
    if (isPresent && !isHugeOrNull)
    {
        Address addr = *((Address *)this);
        return (PageEntry *)((addr << 9) | (index << 12));
    }
    return null;
}

void System::VirtualAddress::print(Std::OutputStream *out)
{
    Std::printf(out, "VirtualAddress[PMLi=%u, PDPi=%u, PDi=%u, PTi=%u, Offset=%u, SignEx=%u]\n", p4Index, p3Index, p2Index, p1Index, offset, signExt);
}