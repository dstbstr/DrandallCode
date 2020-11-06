#include "Instrumentation/Log.h"

#include "Instrumentation/LogWriter/ILogListener.h"
#include "TestCommon.h"

#include <string>
#include <vector>

using testing::HasSubstr;

class TestLogWriter : public Log::ILogListener {
public:
    TestLogWriter() : Log::ILogListener(Log::LogFilter::MatchAll) {}

    void WriteLog(const LogEvent& event) const override {
        m_EventLogs.push_back(LogEvent(event));
    }

    std::vector<LogEvent> GetLogs() {
        return m_EventLogs;
    }

private:
    mutable std::vector<LogEvent> m_EventLogs;
};

TestLogWriter writer;

class DebugLogTest : public ::testing::Test {
protected:
    TestLogWriter m_LogWriter{};
};

TEST_F(DebugLogTest, LogContextContainsRequiredInfo) {
    LOG_INFO("Message");
    Log::GetLogStore().FlushAll();

    auto logs = m_LogWriter.GetLogs();
    ASSERT_EQ(1, logs.size());
    auto context = logs[0].Context;
    ASSERT_FALSE(context.FileName.empty());
    ASSERT_THAT(context.FileName, HasSubstr(context.FileNameShort));
    ASSERT_STRCASEEQ("Log.test.cpp", context.FileNameShort.c_str());
    ASSERT_THAT(context.FunctionSignature, HasSubstr(context.FunctionClass));
    ASSERT_THAT(context.FunctionSignature, HasSubstr(context.FunctionName));
    ASSERT_GT(context.LineNumber, u32(0));
}

TEST_F(DebugLogTest, LogEntryContainsRelevantInformation) {
    LOG_INFO("Message");
    Log::GetLogStore().FlushAll();

    auto logs = m_LogWriter.GetLogs();
    ASSERT_EQ(1, logs.size());
    auto entry = logs[0].Entry;

    ASSERT_EQ(Log::LogLevel::Info, entry.Level);
    ASSERT_STREQ("Message", entry.Message.c_str());
    ASSERT_GT(entry.Timestamp.time_since_epoch(), std::chrono::milliseconds(0));
}
