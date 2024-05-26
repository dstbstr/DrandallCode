#pragma once

#include "Core/Instrumentation/DebugContext.h"
#include "Core/DesignPatterns/ServiceLocator.h"

#include <source_location>
#include <string>
#include <chrono>
#include <format>

namespace Log {
	enum struct Level {
		Info,
		Warning,
		Error
	};

	struct Entry {
		Entry(Level level, std::string msg, Debug::Context context)
			: LogLevel(level)
			, Message(msg)
			, Time(std::chrono::system_clock::now())
			, Context(context)
		{}

		Level LogLevel{};
		std::string Message{};
		std::chrono::system_clock::time_point Time{};
		Debug::Context Context{};
	};

	void Initialize();
	void Flush();

	void Info(const std::string& message, std::source_location loc = std::source_location::current());
	void Warn(const std::string& message, std::source_location loc = std::source_location::current());
	void Error(const std::string& message, std::source_location loc = std::source_location::current());

	#define DR_ASSERT_MSG(condition, message) \
		if(!(condition)) { \
			Log::Error(std::format("ASSERT: '{}' {}", #condition, message)); \
		}

	#define DR_ASSERT(condition) DR_ASSERT_MSG(condition, "")
}