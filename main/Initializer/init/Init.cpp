#include <std/types.h>
#include <sys/system.h>
#include <sys/arch_init.h>
#include <std/format.h>

#include <sys/phys_mem.h>

using namespace System;

extern "C" void start(Address mbootAddr)
{
    ArchInit::init(mbootAddr);   

    kprintln("Waiting for the memory manager!");

    PhysMemory::instance()->printRegions(getStdOut());
}