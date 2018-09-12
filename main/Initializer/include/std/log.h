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
#define logf(log_level, ...)                   \
    *System::getStdOut() << log_level << ": "; \
    Std::printf(System::getStdOut(), __VA_ARGS__);
#define debug(msg) log(LOG_DEBUG, msg)
#define debugf(msg, ...) logf(LOG_DEBUG, msg, __VA_ARGS__)

#else

#define log(log_level, msg)
#define logf(log_level, ...)
#define debug(msg)
#define debugf(msg, args)

#endif

#ifdef KERNEL_ASSERTION

#define kassert(x) \
    if (!x)        \
        System::kerror("KERNEL ASSERTION FAILED", __FILE__, __LINE__);

#else
#define kassert(x)
#endif

#endif
