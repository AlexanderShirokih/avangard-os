/*
 * PhysMemory.cpp
 *
 * Created on: 7 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#include <sys/phys_mem.h>
#include <std/types.h>
#include <std/format.h>
#include <std/log.h>
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

        //Если участок меньше размера одной страницы, тогда игнорируем его
        if (length <= PAGE_SIZE)
        {
            logf(LOG_DEBUG, "Too small region [addr=%X, len=%X]. Ignore it.\n", addr, length);
            return;
        }

        //Если начальный адрес не выровнен по размеру страницы, округляем в большую сторону
        ULong start = GET_FRAME(addr);
        if (addr % PAGE_SIZE)
        {
            start++;
        }

        //Если конечный адрес не выровнен по размеру страницы, будет округлён в меньшую сторону
        const ULong end = GET_FRAME(addr + length);

        if (start >= end)
        {
            //Пустая область
            logf(LOG_WARN, "Invalid or empty memory region at start=%X, length=%X\n", addr, length);
            return;
        }

        area->start = start;
        area->nextFreeFrame = area->start;
        area->numFrames = end - start;
        area->availFrames = area->numFrames;

        logf(LOG_DEBUG, "Area [addr=%X, len=%X, start=%i, numFrames=%i]\n", addr, length, area->start, area->numFrames);

        if (currArea)
        {
            areas[currArea - 1].nextArea = area;
        }
        currArea++;
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
    Frame endPage = GET_FRAME(addr + length);

    //Выравниваем регион по размеру страницы
    if (page % PAGE_SIZE)
        page++;

    for (UInt i = 0; i < MAX_AREAS; i++)
    {
        MemoryArea *area = &areas[i];
        if (!area)
            continue;

        if (area->start == page)
        {
            //Вырезаем регион памяти если он попадает на начало участка
            //Вырезать можно только чистые регионы
            area->numFrames -= endPage - area->start;
            area->start = endPage;
            area->nextFreeFrame = area->start;
            area->availFrames = area->numFrames;
            break;
        }
        else if ((page > area->start && page < area->start + area->numFrames) || (endPage > area->start && endPage < area->start + area->numFrames))
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
    Std::printf(out, (&areas[0]) ? "Avalable memory regions:\n" : "No available regions!!!");

    for (UInt i = 0; i < MAX_AREAS; i++)
    {
        MemoryArea *area = &areas[i];
        if (area)
        {
            Std::printf(out, "    Region start=%d, frames=%d\n", area->start, area->numFrames);
            if (!area->nextArea)
                break;
        }
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