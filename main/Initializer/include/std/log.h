#ifndef SYS_LOG_H
#define SYS_LOG_H 1

//Enable debugging
//-------------
#define DEBUG
#define KERNEL_ASSERTION
//--------------

#ifdef DEBUG

#include <std/log.h>
#include <std/types.h>
#include <std/format.h>

#define debug(msg) *System::getStdOut() << "DEBUG: " << msg << "\n";
#define debugf(...) *System::getStdOut()<< "DEBUG:"; Std::printf(System::getStdOut(), __VA_OPT__(,) __VA_ARGS__);
#else
#define debug(msg)
#define debugf(msg, args)
#endif

#ifdef KERNEL_ASSERTION

// #include <sys/panic.h>
// #define assert(x) if(!x) kpanic_at("KERNEL ASSERTION FAILED", __FILE__, __LINE__);

#else
#define assert(x)
#endif

#endif
