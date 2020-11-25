#ifndef __STDOUTLOGWRITER_H__
#define __STDOUTLOGWRITER_H__

#include "Instrumentation/DebugContext.h"
#include "Instrumentation/Log.h"
#include "Instrumentation/LogEntry.h"
#include "Instrumentation/LogEvent.h"
#include "Instrumentation/LogLevel.h"
#include "Instrumentation/LogWriter/ILogListener.h"
#include "Utilities/Format.h"
#include "Utilities/TimeUtils.h"

#include <iostream>

namespace Log {
    constexpr char DateTimeFormat[]{"%Y/%m/%d %H:%M:%S "};

    constexpr char InfoLineFormat[]{
        "[%s] " // Level
        "%s:%d " // File:Line
        "%s\n" // Msg
    };

    constexpr char ErrorLineFormat[]{
        "[%8s] " // Level
        "%s:%d " // File:Line
        "(%s) - " // Function name
        "%s\n" // Msg
    };

    class StdOutLogWriter : public ILogListener {
    public:
        StdOutLogWriter(Log::LogFilter filter = Log::LogFilter::MatchAll) : ILogListener(filter) {}

        void WriteLog(const LogEvent& event) const override {
            auto entry = event.Entry;
            auto context = event.Context;

            auto out = &std::cout;
            if(entry.Level >= LogLevel::Warn) {
                out = &std::cerr;
            }

            if(entry.Level >= LogLevel::Error) {
                *out << TimeUtils::DateTimeToString(entry.Timestamp);
                *out << StrUtil::Format(ErrorLineFormat, LogLevel::AsString(entry.Level), context.FileNameShort, context.LineNumber, context.FunctionName, entry.Message);
                if(!event.StackTrace.empty()) {
                    *out << "\n\t";
                    *out << StrUtil::JoinVec("\n\t", event.StackTrace);
                }
            } else {
                *out << StrUtil::Format(InfoLineFormat, LogLevel::AsString(entry.Level), context.FileNameShort, context.LineNumber, entry.Message);
            }
        }
    };

} // namespace Log
#endif // __STDOUTLOGWRITER_H__