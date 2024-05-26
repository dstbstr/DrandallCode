#include "TestCommon.h"

#include "Core/Instrumentation/Logging.h"
#include "Core/Instrumentation/ISink.h"

struct TestLogSink : public Log::ISink {
	TestLogSink(Log::Filter filter)
		: ISink(filter) {}

	void Write(const Log::Entry& entry) override {
		messages.push_back(entry);
	}

	std::vector<Log::Entry> messages;
};

struct LoggingTest : public testing::Test {
	void SetUp() override {
		Log::Initialize();

		WarnFilter.WithLevel(Log::Level::Warning);
		ErrorFilter.WithLevel(Log::Level::Error);
	}

	void TearDown() override {
		
	}

	bool DoesLog(Log::Level messageLevel, Log::Filter filter) {
		TestLogSink sink(filter);
		switch (messageLevel) {
			case Log::Level::Info: Log::Info("Test Info"); break;
			case Log::Level::Warning: Log::Warn("Test Warning"); break;
			case Log::Level::Error: Log::Error("Test Error"); break;
		}

		Log::Flush();

		return sink.messages.size() == 1;
	}

	Log::Filter NoFilter{};
	Log::Filter WarnFilter{};
	Log::Filter ErrorFilter{};
};

TEST_F(LoggingTest, NoFilter_WithInfoMessage_WritesMessage) {
	ASSERT_TRUE(DoesLog(Log::Level::Info, NoFilter));
}

TEST_F(LoggingTest, NoFilter_WithWarning_WritesMessage) {
	ASSERT_TRUE(DoesLog(Log::Level::Warning, NoFilter));
}

TEST_F(LoggingTest, NoFilter_WithError_WritesMessage) {
	ASSERT_TRUE(DoesLog(Log::Level::Error, NoFilter));
}

TEST_F(LoggingTest, WarnFilter_WithInfoMessage_DoesNotWrite) {
	ASSERT_FALSE(DoesLog(Log::Level::Info, WarnFilter));
}

TEST_F(LoggingTest, WarnFilter_WithWarning_WritesMessage) {
	ASSERT_TRUE(DoesLog(Log::Level::Warning, WarnFilter));
}

TEST_F(LoggingTest, WarnFilter_WithError_WritesMessage) {
	ASSERT_TRUE(DoesLog(Log::Level::Error, WarnFilter));
}

TEST_F(LoggingTest, ErrorFilter_WithInfoMessage_DoesNotWrite) {
	ASSERT_FALSE(DoesLog(Log::Level::Info, ErrorFilter));
}

TEST_F(LoggingTest, ErrorFilter_WithWarning_DoesNotWrite) {
	ASSERT_FALSE(DoesLog(Log::Level::Warning, ErrorFilter));
}

TEST_F(LoggingTest, ErrorFilter_WithError_WritesMessage) {
	ASSERT_TRUE(DoesLog(Log::Level::Error, ErrorFilter));
}

TEST_F(LoggingTest, CustomFilter_WithMatch_WritesMessage) {
	auto filter = Log::Filter().WithCustomMatcher([](const Log::Entry& entry) {
		return entry.Context.LineNumber > 1;
	});

	ASSERT_TRUE(DoesLog(Log::Level::Info, filter));
}

TEST_F(LoggingTest, CustomFilter_WithNoMatch_DoesNotWrite) {
	auto filter = Log::Filter().WithCustomMatcher([](const Log::Entry& entry) {
		return entry.Context.LineNumber == 1;
	});

	ASSERT_FALSE(DoesLog(Log::Level::Info, filter));
}