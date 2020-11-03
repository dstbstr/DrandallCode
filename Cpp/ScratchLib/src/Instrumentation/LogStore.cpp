#include "Instrumentation/Private/LogStore.h"

#include "Instrumentation/DebugContext.h"
#include "Instrumentation/LogEntry.h"

#include <algorithm>

namespace Log {
    void LogStoreEnabled::FlushAll() {
        for(auto& entry: m_Entries) {
            try {
                RaiseEvent(entry);
            } catch(...) {
                // no-op.  Throwing prevents clearing the entries, and notifying other listeners
            }
        }
        m_Entries.clear();
    }

    void LogStoreEnabled::Flush(size_t maxEntries) {
        if(maxEntries < 1) {
            return;
        }

        size_t toInvoke = std::min(maxEntries, m_Entries.size());
        for(size_t i = 0; i < toInvoke; i++) {
            RaiseEvent(m_Entries[i]);
        }

        m_Entries.erase(m_Entries.begin(), m_Entries.begin() + toInvoke);
    }

    void LogStoreEnabled::AddMessage(const Debug::Context& context, const LogEntry& entry) {
        // m_Entries.push_back(LogEvent(context, entry, std::vector<std::string>()));
        RaiseEvent(LogEvent(context, entry, std::vector<std::string>())); // the old implementation is for game loops
    }

    void LogStoreEnabled::AddMessageImmediate(const Debug::Context& context, const LogEntry& entry) {
        // Essentially here just to allow AssertToException to throw
        RaiseEvent(LogEvent(context, entry, std::vector<std::string>()));
    }
} // namespace Log