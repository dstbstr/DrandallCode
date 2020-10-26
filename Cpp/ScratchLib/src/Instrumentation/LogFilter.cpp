#include "Instrumentation/LogFilter.h"

#include "Instrumentation/DebugContext.h"
#include "Instrumentation/LogEntry.h"
#include "Utilities/StringUtilities.h"

namespace Log {
    const LogFilter LogFilter::MatchAll = {};

    LogFilter& LogFilter::SetLogLevel(LogLevel::Enum level) {
        m_Levels.clear();
        m_Levels.push_back(level);
        m_MatchAll = false;
        return *this;
    }

    LogFilter& LogFilter::AddFilePattern(std::string filePattern) {
        m_FilePatterns.push_back(std::regex(filePattern));
        m_MatchAll = false;
        return *this;
    }

    bool LogFilter::MatchesAllFilters(const Debug::Context& context, const LogEntry& entry) const {
        if(m_MatchAll) {
            return true;
        }

        if(!m_Levels.empty()) {
            if(entry.Level < m_Levels[0]) {
                return false;
            }
        }

        if(!m_FilePatterns.empty()) {
            bool foundMatch = false;
            for(auto& regex: m_FilePatterns) {
                if(std::regex_match(context.FileName, regex)) {
                    foundMatch = true;
                    break;
                }
            }

            if(!foundMatch) {
                return false;
            }
        }

        return true;
    }
} // namespace Log