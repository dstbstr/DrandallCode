#ifndef __LOG_H__
#define __LOG_H__

#include "Instrumentation/DebugContext.h"
#include "Instrumentation/LogEntry.h"
#include "Instrumentation/LogLevel.h"
#include "Instrumentation/Private/LogStore.h"
#include "Utilities/StringUtils.h"

#ifndef FINAL
#define LOG_VERBOSE(msg) Log::GetLogStore().AddMessage(DEBUG_CONTEXT, Log::LogEntry(Log::LogLevel::Verbose, msg));
#define LOG_DEBUG(msg) Log::GetLogStore().AddMessage(DEBUG_CONTEXT, Log::LogEntry(Log::LogLevel::Debug, msg));
#define LOG_INFO(msg) Log::GetLogStore().AddMessage(DEBUG_CONTEXT, Log::LogEntry(Log::LogLevel::Info, msg));
#define LOG_WARN(msg) Log::GetLogStore().AddMessage(DEBUG_CONTEXT, Log::LogEntry(Log::LogLevel::Warn, msg));
#define LOG_ERROR(msg) Log::GetLogStore().AddMessageImmediate(DEBUG_CONTEXT, Log::LogEntry(Log::LogLevel::Error, msg));
#define LOG_CRITICAL(msg) Log::GetLogStore().AddMessageImmediate(DEBUG_CONTEXT, Log::LogEntry(Log::LogLevel::Critical, msg));

#else
#define LOG_VERBOSE(msg)
#define LOG_DEBUG(msg)
#define LOG_INFO(msg)
#define LOG_WARN(msg)
#define LOG_ERROR(msg)
#define LOG_CRITICAL(msg)

#endif

namespace Log {
    LogStore& GetLogStore();
}

#endif // __LOG_H__