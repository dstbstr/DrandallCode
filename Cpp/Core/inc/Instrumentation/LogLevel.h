#ifndef __LOGLEVEL_H__
#define __LOGLEVEL_H__

#include "Platform/Types.h"

#include <string>

namespace Log {
    struct LogLevel {
        enum Enum : u8 { Verbose, Debug, Info, Warn, Error, Critical };

        static std::string AsString(Enum level) {
            switch(level) {
            case Verbose: return "VERBOSE";
            case Debug: return "DEBUG";
            case Info: return "INFO";
            case Warn: return "WARN";
            case Error: return "ERROR";
            case Critical: return "CRITICAL";
            default: return "Unknown";
            }
        }
    };
} // namespace Log
#endif // __LOGLEVEL_H__