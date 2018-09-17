/*
 * paging.h
 *
 *  Created on: 08 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#ifndef INCLUDE_PAGING
#define INCLUDE_PAGING

#include <sys/phys_mem.h>

#include <std/types.h>
#include <std/streams.h>

#define ENTRY_COUNT 512

namespace System
{

union PageEntry {
    struct __attribute__((packed))
    {
        Bool isPresent : 1;             //0
        Bool isWritable : 1;            //1
        Bool isUserAccessible : 1;      //2
        Bool isWriteThroughCaching : 1; //3
        Bool isNoCache : 1;             //4
        Bool isAccessed : 1;            //5
        Bool isDirty : 1;               //6
        Bool isHugeOrNull : 1;          //7
        Bool isGlobal : 1;              //8
        ULong skipped : 54;             //9-62
        Bool isNoExecute : 1;           //63
    };

    Address value;

    PageEntry *getNextLevelEntry(UInt index);
    PageEntry *getOrCreate(UInt index, PhysMemory *allocator);
    Address getPhysicalAddress();
    void print(Std::OutputStream *os);
    void fillZero();
};

union VirtualAddress {
    struct __attribute__((packed))
    {
        UInt offset : 12;
        UInt p1Index : 9;
        UInt p2Index : 9;
        UInt p3Index : 9;
        UInt p4Index : 9;
        UInt signExt : 16;
    };

    struct __attribute__((packed))
    {
        UInt offset2M : 21;
    };

    Address address;

    void print(Std::OutputStream *os);

    VirtualAddress() {}
    VirtualAddress(Address addr) { address = addr; }
};

#define HUGE_PAGE_SIZE (1 << 21)
#define NORMAL_PAGE_SIZE (1 << 12)

struct VirtualMemory
{
    Address getPhysicalAddress(VirtualAddress *addr);

    void mapDirect(Address virtAddr, Address physAddress, UInt pageSize, Bool isWritable, Bool isUser, Bool isExceutable);
};

} // namespace System

#endif