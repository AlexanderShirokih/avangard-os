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
            area->prevArea = &areas[currArea - 1];
        }
        currArea++;
    }
    //else ignore unmapped area
}

void static inline checkIfEmpty(System::MemoryArea *area, UInt aId, System::MemoryArea *areas)
{
    //Исключаемый регион памяти больше участка
    if (area->numFrames <= 0)
    {
        if (area->prevArea)
            area->prevArea->nextArea = area->nextArea;

        //Trim items list
        for (UInt i = aId + 1; i < MAX_AREAS; i++)
        {
            areas[i - 1] = areas[i];
        }

        //Update prev/next links
        for (UInt i = 1; i < MAX_AREAS; i++)
        {
            if (i == 1)
                areas[i - 1].prevArea = 0;
            if (&areas[i] || areas[i].numFrames)
                areas[i - 1].nextArea = &areas[i];
            else
                areas[i - 1].nextArea = 0;
            areas[i].prevArea = &areas[i - 1];
        }
    }
}

/*
 * Исключает регион из участка размеченной памяти.
 * NOTE Временно исключаем ядро из доступной размеченной памяти
 * Вырезать можно только чистые регионы.
 */
void System::PhysMemory::excludeRegion(const Address addr, const ULong length)
{
    Frame page = GET_FRAME(addr);
    Frame endPage = GET_FRAME(addr + length);

    //Выравниваем регион по размеру страницы
    if (endPage % PAGE_SIZE)
        endPage++;

    for (UInt i = 0; i < MAX_AREAS; i++)
    {
        MemoryArea *area = &areas[i];
        if (!area || !area->numFrames)
            continue;

        if (page <= area->start && endPage > area->start)
        {
            //Регион памяти попадает на начало участка
            area->numFrames -= endPage - area->start;
            area->start = endPage;
            area->nextFreeFrame = area->start;
            area->availFrames = area->numFrames;

            checkIfEmpty(area, i, areas);
        }
        else if (page < area->start + area->numFrames && endPage >= area->start + area->numFrames)
        {

            //Регион памяти попадает на конец участка
            area->numFrames -= area->numFrames + area->start - endPage;
            area->availFrames = area->numFrames;

            checkIfEmpty(area, i, areas);
        }
        else if ((page > area->start && page < area->start + area->numFrames) || (endPage > area->start && endPage < area->start + area->numFrames))
        {
            /*
			 * Если регион пямяти в середине участка, то ничего не делаем
			 * т.к нужно разделять на 2 участка и т.д.,
			 * а фактически этот случай не должен произойти
			 * потому-что пока есть только один участок и это ядро
			 */
            logf(LOG_ERROR, "Can't exclude[start=%i, end=%i] from region[start=%i, end=%i]\n", page, endPage, area->start, area->start + area->numFrames);
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
        if (area && area->numFrames)
        {
            Std::printf(out, "    Region start=%d, frames=%d\n", area->start, area->numFrames);
            if (!area->nextArea)
                break;
        }
    }
}

Address System::PhysMemory::allocFrame()
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
    return frame * PAGE_SIZE;
}

void System::PhysMemory::deallocFrame(Address f)
{
    (void)f;
    kerror("Can't deallocate frame");
}