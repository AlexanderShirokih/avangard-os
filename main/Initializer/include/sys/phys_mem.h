/*
 * phys_mem.h
 *
 *  Created on: 7 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#ifndef INCLUDE_PHYS_MEM_H_
#define INCLUDE_PHYS_MEM_H_

#include <std/types.h>
#include <std/streams.h>

#define PAGE_SIZE (1 << 12) // Return 4KiB pages again // (1 << 21) //2MiB Huge Page
#define MAX_AREAS 8
#define GET_FRAME(addr) (Frame)((addr) / PAGE_SIZE)

namespace System
{

typedef UInt Frame;

struct MemoryArea
{
  Frame start;
  Frame nextFreeFrame;
  UInt numFrames;
  UInt availFrames;
  MemoryArea *nextArea;
};

class PhysMemory
{
  MemoryArea areas[MAX_AREAS];
  MemoryArea *currArea;
  //Frame frame_pool[MAX_POOL_FRAMES];
  PhysMemory();

public:
  static PhysMemory *instance()
  {
    static PhysMemory mem;
    return &mem;
  };
  void addRegion(const Address addr, const ULong length, const Bool avail);
  void excludeRegion(const Address addr, const ULong length);
  Frame allocFrame();
  void deallocFrame(Frame);
  void printRegions(Std::OutputStream *out);
};

} // namespace System
#endif