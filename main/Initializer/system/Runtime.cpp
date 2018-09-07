//
// Created by Alexander Shirokih on 16.06.18.
//

//CPP ABI functions

#include <std/types.h>

/*
 * The __cxa_pure_virtual function is an error handler that is invoked when a pure virtual function is called.
 */
extern "C" void __cxa_pure_virtual()
{
    while (1)
        ;
}

#ifdef __arm__
struct Guard
{
    Bool isInitialized;    //+8
    Bool isUsed;           //+16
    UByte unusedStuff[16]; //+32
};
#else

struct Guard
{
    Bool isInitialized;         //+8
    Bool isUsed;                //+16
    UByte unusedStuff[32 + 16]; //+64
};
#endif

/*
 * The __cxa_guard functions called for static member initializaions
*/

extern "C" Bool __cxa_guard_acquire(Guard *guard)
{
    //Check if it already has been initialized
    if (guard->isInitialized)
        return false;

    guard->isInitialized = true;

    //Mutex lock is not used because this code runs only by the kernel
    //... and now we have no mutex

    guard->isUsed = true;

    return true;
}

extern "C" void __cxa_guard_release(Guard *guard)
{
    //Just turn off the flag
    guard->isInitialized = false;
}