/*
 * PhysMemory.cpp
 *
 * Created on: 7 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#include <sys/phys_mem.h>
#include <std/types.h>
#include <std/format.h>
#include <sys/system.h>

System::PhysMemory::PhysMemory()
{
    currArea = areas;
}

void System::PhysMemory::addRegion(const Address addr, const ULong length, const Bool avail)
{
    static UInt currArea = 0;

    if (avail)
    {
        //Участки памяти не должны перекрываться
        //Но boot дает нормальные участки
        MemoryArea *area = &areas[currArea];
        area->start = GET_FRAME(addr);
        area->nextFreeFrame = area->start;
        area->numFrames = GET_FRAME(addr + length) - area->start;
        area->availFrames = area->numFrames;
        if (currArea)
        {
            areas[currArea - 1].nextArea = area;
        }
        currArea++;
        //		area->nextArea = &areas[++currArea];
    }
    //else ignore unmapped area
}

/*
 * Исключает регион из участка размеченной памяти.
 * NOTE Временно исключаем ядро из доступной размеченной памяти
 */
void System::PhysMemory::excludeRegion(const Address addr, const ULong length)
{
    Frame page = GET_FRAME(addr);
    MemoryArea *area = areas;
    while (area->nextArea)
    {
        if (area->start == page)
        {
            const Frame new_start = GET_FRAME(addr + length) + 1;
            //Вырезаем регион памяти если он попадает на начало участка
            //Вырезать можно только чистые регионы
            area->numFrames -= new_start - area->start;
            area->start = new_start;
            area->nextFreeFrame = area->start;
            area->availFrames = area->numFrames;
            break;
        }
        else if (page < area->start + area->numFrames)
        {
            /*
			 * Если регион пямяти в середине участка, то ничего не делаем
			 * т.к нужно разделять на 2 участка и т.д.,
			 * а фактически этот случай не должен произойти
			 * потому-что пока есть только один участок и это ядро
			 */
            kerror("Page shrinking required, but it not implemented by kernel");
        }
        area = area->nextArea;
    }
}

void System::PhysMemory::printRegions(Std::OutputStream *out)
{
    MemoryArea *area = areas;

    Std::printf(out, (area->nextArea) ? "Avalable memory regions:\n" : "No available regions!!!");

    while (area->nextArea)
    {
        Std::printf(out, "    Region start=%d, frames=%d\n", area->start, area->numFrames);
        area = area->nextArea;
    }
}

System::Frame System::PhysMemory::allocFrame()
{
    Frame frame = currArea->nextFreeFrame;
    if (!(--(currArea->availFrames)))
    { //Если это была последняя страница
        if (!(currArea = currArea->nextArea))
        { //Если это был последний участок памяти
            kerror("Out Of Memory!");
            return 0;
        }
    }
    else
    {
        currArea->nextFreeFrame++;
    }
    return frame;
}

void System::PhysMemory::deallocFrame(Frame f)
{
    (void)f;
    kerror("Can't deallocate frame");
}