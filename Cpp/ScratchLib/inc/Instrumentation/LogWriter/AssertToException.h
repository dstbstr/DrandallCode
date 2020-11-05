#ifndef __ASSERTTOEXCEPTION_H__
#define __ASSERTTOEXCEPTION_H__

#include "Instrumentation/LogEvent.h"
#include "Instrumentation/LogFilter.h"
#include "Instrumentation/LogWriter/ILogListener.h"
#include "Utilities/Require.h"
#include "Utilities/StringUtilities.h"


class AssertToException : public Log::ILogListener {
public:
    AssertToException() : ILogListener(Log::LogFilter().SetLogLevel(Log::LogLevel::Error)) {}

    void WriteLog(const LogEvent& event) const override {
        Require::False(true, event.Entry.Message);
    }
};
#endif // __ASSERTTOEXCEPTION_H__