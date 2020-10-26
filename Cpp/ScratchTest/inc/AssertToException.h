#ifndef __ASSERTTOEXCEPTION_H__
#define __ASSERTTOEXCEPTION_H__

#include "Instrumentation/LogEvent.h"
#include "Instrumentation/LogFilter.h"
#include "Instrumentation/LogWriter/ILogListener.h"
#include "Utilities/StringUtilities.h"

class AssertToException : public Log::ILogListener {
public:
    AssertToException() : ILogListener(Log::LogFilter().SetLogLevel(Log::LogLevel::Error)) {}

    void WriteLog(const LogEvent& event) const override {
        throw std::exception(event.Entry.Message.c_str());
    }
};
#endif // __ASSERTTOEXCEPTION_H__