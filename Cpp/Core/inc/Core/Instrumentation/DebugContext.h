#pragma once

#include <source_location>
#include <thread>

namespace Debug {
	struct Context {
		Context(std::source_location sourceLoc = std::source_location::current()) :
			FileName(sourceLoc.file_name()),
			Function(sourceLoc.function_name()),
			LineNumber(sourceLoc.line()),
			ThreadId(std::this_thread::get_id())
		{}

		std::string FileName{};
		std::string Function{};
		size_t LineNumber{0};
		std::thread::id ThreadId{};
	};
}