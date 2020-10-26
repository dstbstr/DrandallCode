#ifndef __STDOUTLOGWRITER_H__
#define __STDOUTLOGWRITER_H__

#include "Instrumentation/DebugContext.h"
#include "Instrumentation/Log.h"
#include "Instrumentation/LogEntry.h"
#include "Instrumentation/LogEvent.h"
#include "Instrumentation/LogLevel.h"
#include "Instrumentation/LogWriter/ILogListener.h"
#include "Utilities/Format.h"

#include <chrono>
#include <iomanip>
#include <iostream>

namespace Log {
    constexpr char DateTimeFormat[]{"%Y/%m/%d %H:%M:%S "};

    constexpr char LogLineFormat[]{
        "[%07s] " // Level
        "%s:%d " // File:Line
        "(%s) - " // Function name
        "%s\n" // Msg
    };

    class StdOutLogWriter : public ILogListener {
    public:
        StdOutLogWriter(Log::LogFilter filter) : ILogListener(filter) {}

        void WriteLog(const LogEvent& event) const override {
            auto entry = event.Entry;
            auto context = event.Context;

            auto out = &std::cout;
            if(entry.Level >= LogLevel::Warn) {
                out = &std::cerr;
            }

            auto ts = std::chrono::system_clock::to_time_t(entry.Timestamp);
            auto utc = *std::gmtime(&ts);
            *out << std::put_time(&utc, DateTimeFormat);

            *out << StrUtil::Format(
                LogLineFormat, LogLevel::AsString(entry.Level), context.FileNameShort, context.LineNumber, context.FunctionName, entry.Message);
        }
    };

} // namespace Log
#endif // __STDOUTLOGWRITER_H__