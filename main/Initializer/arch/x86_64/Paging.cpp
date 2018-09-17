/*
 * Paging.cpp
 *
 *  Created on: 08 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#include <arch/x86_64/paging.h>

#include <sys/system.h>

#include <std/format.h>
#include <std/log.h>

using namespace System;

/**
 * Kernel PML4 Table address that always be the same.
 */
PageEntry *tranlsationTable = (PageEntry *)0xFFFFFFFFFFFFF000;

/**
 * Prints info about the page entry to OutputStream
 */
void PageEntry::print(Std::OutputStream *out)
{
    Std::printf(out, "PageEntry[prsnt=%u, wtr=%u, usr=%u, wrtThr=%u, nCache=%u, acc=%u, drty=%u, huge|Null=%u, glb=%u, noEx=%u, physAddr=%X]\n", isPresent, isWritable, isUserAccessible, isWriteThroughCaching, isNoCache, isAccessed, isDirty, isHugeOrNull, isGlobal, isNoExecute, getPhysicalAddress());
}

/**
 * Returns next level pointer of PageEntry.
 * This method is not checking for the page isPresent.
 */
PageEntry *PageEntry::getNextLevelEntry(UInt index)
{
    Address addr = reinterpret_cast<Address>(this);
    return (PageEntry *)((addr << 9) | (index << 12));
}

PageEntry *PageEntry::getOrCreate(UInt index, System::PhysMemory *allocator)
{
    //We can not create child tables in huge page
    kassert(!isHugeOrNull);

    debug("getOrCreate for:");
    print(getStdOut());

    PageEntry * next = getNextLevelEntry(index);  
    if (!this[index].value)
    {
        debugf("allocating frame for index=%i\n", index);
        this[index].value = allocator->allocFrame();
        debugf("allocated physicalAddress=%X\n", this[index].value);
        //TODO Debug
        next->fillZero();
    }

    return next;
}

/**
 * Returns 52-bit aligned physical address or null is this page is not present.
 */
Address PageEntry::getPhysicalAddress()
{
    if (isPresent)
        return value & 0x000FFFFFFFFFF000;
    return null;
}

void PageEntry::fillZero()
{
    for (UInt i = 0; i < ENTRY_COUNT; i++)
    {
        //physicalAddress is a largest variable of this union
        this[i].value = 0x0;
    }
}

/**
 * Prints info about the virtual address to OutputStream
 */
void VirtualAddress::print(Std::OutputStream *out)
{
    Std::printf(out, "VirtualAddress[PMLi=%u, PDPi=%u, PDi=%u, {PTi=%u, Offset=%u}|{Offset2M=%u}, SignEx=%u]\n", p4Index, p3Index, p2Index, p1Index, offset, offset2M, signExt);
}

/**
 * Returns physical address from virtual or 0 if virtual address is not present
 */
Address VirtualMemory::getPhysicalAddress(VirtualAddress *address)
{
    // translationTable should be always present
    PageEntry *l3 = tranlsationTable->getNextLevelEntry(address->p4Index);

    //Note that 1GiB page is not supported
    if (l3->isPresent)
    {

        PageEntry *l2 = l3->getNextLevelEntry(address->p3Index);

        if (l2->isPresent) //if l2 page is present
        {
            PageEntry *l1OrPage = &l2[address->p2Index];

            if (l1OrPage->isPresent)
            {
                if (l1OrPage->isHugeOrNull) //It's a 2MiB huge page
                {
                    return l1OrPage->getPhysicalAddress() + address->offset2M;
                }

                PageEntry *page = l1OrPage->getNextLevelEntry(address->p1Index);
                if (page->isPresent)
                    return page->getPhysicalAddress() + address->offset;
            }
        }
    }

    return null;
}

void VirtualMemory::mapDirect(Address virtAddr, Address physAddress, UInt pageSize, Bool isWritable, Bool isUser, Bool isExceutable)
{
    kassert(pageSize == HUGE_PAGE_SIZE || pageSize == NORMAL_PAGE_SIZE);
    kassert((physAddress % pageSize == 0) && (virtAddr % pageSize == 0));

    VirtualAddress address(virtAddr);
    PhysMemory *allocator = PhysMemory::instance();

    PageEntry *l3 = tranlsationTable->getOrCreate(address.p4Index, allocator);
    PageEntry *l2 = l3->getOrCreate(address.p3Index, allocator);

    PageEntry *l1OrPage = &l2[address.p2Index];
    PageEntry *page;

    if (pageSize == HUGE_PAGE_SIZE)
    {
        page = l1OrPage;
        page->isHugeOrNull = true;
    }
    else
    {
        page = l1OrPage->getOrCreate(address.p1Index, allocator);
    }

    kassert(page->value == 0);
    page->isPresent = true;
    page->isWritable = isWritable;
    page->isUserAccessible = isUser;
    page->isNoExecute = !isExceutable;
}