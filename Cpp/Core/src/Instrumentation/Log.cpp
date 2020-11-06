#include "Instrumentation/Log.h"

namespace Log {
    LogStore& GetLogStore() {
        static LogStore logStore;
        return logStore;
    }
} // namespace Log
