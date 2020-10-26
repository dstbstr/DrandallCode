#ifndef __LOGLEVEL_H__
#define __LOGLEVEL_H__

#include "Platform/Types.h"

#include <string>

namespace Log {
    struct LogLevel {
        enum Enum : u8 { Verbose, Debug, Info, Warn, Error, Critical };

        static std::string AsString(Enum level) {
            switch(level) {
            case Verbose: return "Verbose";
            case Debug: return "Debug";
            case Info: return "Info";
            case Warn: return "Warn";
            case Error: return "Error";
            case Critical: return "Critical";
            default: return "Unknown";
            }
        }
    };
} // namespace Log
#endif // __LOGLEVEL_H__