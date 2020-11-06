#ifndef __LOGEVENT_H__
#define __LOGEVENT_H__

#include "DesignPatterns/BaseEvent.h"
#include "Instrumentation/DebugContext.h"
#include "Instrumentation/LogEntry.h"

struct LogEvent : public BaseEvent<LogEvent> {
    LogEvent(Debug::Context context, Log::LogEntry entry, std::vector<std::string> stackTrace = {})
        : Context(context)
        , Entry(entry)
        , StackTrace(stackTrace) {}

    Debug::Context Context;
    Log::LogEntry Entry;
    std::vector<std::string> StackTrace;
};
#endif // __LOGEVENT_H__