#ifndef __LOGFILTER_H__
#define __LOGFILTER_H__

#include "Instrumentation/LogLevel.h"

#include <regex>
#include <string>
#include <vector>

namespace Debug {
    struct Context;
}

namespace Log {
    struct LogEntry;

    class LogFilter {
    public:
        LogFilter& SetLogLevel(LogLevel::Enum level);
        LogFilter& AddModule(std::string module);
        LogFilter& AddSubComponent(std::string subComponent);
        LogFilter& AddFilePattern(std::string filePattern);

        bool MatchesAllFilters(const Debug::Context& context, const LogEntry& entry) const;

        static const LogFilter MatchAll;

    private:
        std::vector<LogLevel::Enum> m_Levels;
        std::vector<std::string> m_Modules;
        std::vector<std::string> m_Components;
        std::vector<std::regex> m_FilePatterns;
        bool m_MatchAll{true};
    };
} // namespace Log
#endif // __LOGFILTER_H__