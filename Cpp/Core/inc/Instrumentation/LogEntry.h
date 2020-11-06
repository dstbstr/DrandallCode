#ifndef __LOGENTRY_H__
#define __LOGENTRY_H__

#include "Instrumentation/LogLevel.h"

#include <chrono>
#include <string>

namespace Log {
    struct LogEntry {
        LogEntry(LogLevel::Enum level, std::string msg) : Level(level), Message(msg), Timestamp(std::chrono::system_clock::now()) {}

        std::chrono::time_point<std::chrono::system_clock> Timestamp;
        LogLevel::Enum Level;
        std::string Message;
    };
} // namespace Log

#endif // __LOGENTRY_H__