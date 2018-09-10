/*
 * paging.h
 *
 *  Created on: 08 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#ifndef INCLUDE_PAGING
#define INCLUDE_PAGING

#include <std/types.h>
#include <std/streams.h>

namespace System
{

struct __attribute__((packed)) PageEntry
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
    UByte unused1 : 3;              //9-11
    Address physicalAddress : 40;   //12-52
    UShort unused2 : 11;            //53-62
    Bool isNoExecute : 1;           //63

    void print(Std::OutputStream *os);
    PageEntry *getNextLevelEntry(UInt index);
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

    Address address;

    void print(Std::OutputStream *os);

    VirtualAddress() {}
    VirtualAddress(Address addr) { address = addr; }
};

struct VirtualMemory
{
};

} // namespace System

#endif