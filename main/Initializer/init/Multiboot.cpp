/*
 * Multiboot.cpp
 *
 *  Created on: 7 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#include <std/types.h>
#include <sys/multiboot2.h>
#include <sys/phys_mem.h>
#include <sys/arch_init.h>

extern Address system_start;
extern Address system_end;

namespace System
{

void parseMultibootTags(Address mboot_addr)
{
    PhysMemory *memory = PhysMemory::instance();
    struct multiboot_tag *tag;
    for (tag = (struct multiboot_tag *)(mboot_addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag + ((tag->size + 7) & ~7)))
    {
        switch (tag->type)
        {
        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
            //                    kprint << "Kernel loaded by:";// << ((struct multiboot_tag_string *) tag)->string;
            break;
        case MULTIBOOT_TAG_TYPE_MMAP:
        {
            multiboot_memory_map_t *mmap;

            for (mmap = ((struct multiboot_tag_mmap *)tag)->entries;
                 (multiboot_uint8_t *)mmap < (multiboot_uint8_t *)tag + tag->size;
                 mmap =
                     (multiboot_memory_map_t *)((ULong)mmap + ((struct multiboot_tag_mmap *)tag)->entry_size))
            {
                memory->addRegion(mmap->addr, mmap->len,
                                  mmap->type == MULTIBOOT_MEMORY_AVAILABLE ? true : false);
            }
        }
        break;
        }
    }
    tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag + ((tag->size + 7) & ~7));

    //Kernel
    memory->excludeRegion((Address)(&system_start),
                          (ULong)(&system_end - &system_start));
    //Multiboot
    //	exclude_region(mboot_addr, (Address) tag - mboot_addr);
}
} // namespace System