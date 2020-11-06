#ifndef __ILOGLISTENER_H__
#define __ILOGLISTENER_H__

#include "DesignPatterns/PubSub.h"
#include "Instrumentation/LogEvent.h"
#include "Instrumentation/LogFilter.h"

namespace Debug {
    struct Context;
}

namespace Log {
    struct LogEntry;

    struct ILogListener : public ISubscriber<LogEvent> {
        ILogListener(Log::LogFilter filter = Log::LogFilter::MatchAll) : m_Filter(filter) {}

        virtual ~ILogListener() = default;

        ILogListener(const ILogListener& other) = delete;
        ILogListener& operator=(const ILogListener& other) = delete;

        void Notify(const LogEvent& event) override {
            if(m_Filter.MatchesAllFilters(event.Context, event.Entry)) {
                WriteLog(event);
            }
        }
        virtual void WriteLog(const LogEvent& event) const = 0;

    private:
        Log::LogFilter m_Filter;
    };
} // namespace Log
#endif // __ILOGLISTENER_H__