#ifndef __LOGSTORE_H__
#define __LOGSTORE_H__

#include "DesignPatterns/PubSub.h"
#include "Instrumentation/LogEvent.h"
#include "Instrumentation/LogFilter.h"

#include <vector>

namespace Debug {
    struct Context;
    struct ContextWithStack;
} // namespace Debug

namespace Log {
    struct LogEntry;

    class LogStoreEnabled : public IPublisher<LogEvent> {
    public:
        void FlushAll();
        void Flush(size_t maxEntries);
        void AddMessage(const Debug::Context& context, const LogEntry& entry);
        void AddMessage(const Debug::ContextWithStack& context, const LogEntry& entry);

    private:
        std::vector<LogEvent> m_Entries;
    };

    class LogStoreDisabled {
    public:
        void FlushAll() {}
        void Flush(size_t) {}
        void AddMessage(const Debug::Context&, const LogEntry&) {}
        void AddMessage(const Debug::ContextWithStack&, const LogEntry&) {}
    };

#ifndef FINAL
    using LogStore = LogStoreEnabled;
#else
    using LogStore = LogStoreDisabled;
#endif
} // namespace Log

#endif // __LOGSTORE_H__