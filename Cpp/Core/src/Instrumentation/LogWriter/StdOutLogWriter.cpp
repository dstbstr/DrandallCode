#include "Core/Instrumentation/LogWriter/StdOutLogWriter.h"

namespace {
    constexpr char InfoLineFormat[]{
        "[%s] " // Level
        "%s:%zu " // File:Line
        "%s\n" // Msg
    };

    constexpr char ErrorLineFormat[]{
        "[%8s] " // Level
        "%s:%zu " // File:Line
        "(%s) - " // Function name
        "%s\n" // Msg
    };
}

StdOutLogWriter::StdOutLogWriter(Log::Filter filter)
	: ISink(filter) {}

void StdOutLogWriter::Write(const Log::Entry& entry) {
    switch (entry.LogLevel) {
	case Log::Level::Info:
		std::printf(InfoLineFormat, "Info", entry.Context.FileName.c_str(), entry.Context.LineNumber, entry.Message.c_str());
		break;
	case Log::Level::Warning:
	case Log::Level::Error:
		std::printf(ErrorLineFormat, entry.LogLevel == Log::Level::Warning ? "Warning" : "Error", entry.Context.FileName.c_str(), entry.Context.LineNumber, entry.Context.Function.c_str(), entry.Message.c_str());
		break;
	}
}