#include <std/types.h>
#include <std/format.h>

#include <sys/system.h>
#include <sys/arch_init.h>
#include <sys/phys_mem.h>

#include <arch/x86_64/paging.h>
#include <sys/phys_mem.h>

using namespace System;

extern "C" void start(Address mbootAddr)
{
    ArchInit::init(mbootAddr);

    PhysMemory::instance()->printRegions(getStdOut());

    kprintln("Waiting for the memory manager!");
}
