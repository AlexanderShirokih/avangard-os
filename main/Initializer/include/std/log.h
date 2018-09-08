#ifndef SYS_LOG_H
#define SYS_LOG_H 1

//Enable debugging
//-------------
#define LOG_NONE 0
#define LOG_DEBUG "D"
#define LOG_INFO "I"
#define LOG_WARN "W"
#define LOG_ERROR "E"

#define LOGGING
#define LOG_LEVEL LOG_DEBUG
#define KERNEL_ASSERTION
//--------------

#ifdef LOGGING

#include <std/log.h>
#include <std/types.h>
#include <std/format.h>

#define log(log_level, msg) *System::getStdOut() << log_level << ": " << msg << "\n";
#define logf(log_level, ...)                       \
    *System::getStdOut() << log_level << ": ";     \
    Std::printf(System::getStdOut(), __VA_ARGS__); \
    *System::getStdOut() << "\n";
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
