#pragma once

#include "Core/Instrumentation/ISink.h"

struct StdOutLogWriter : public Log::ISink {
	StdOutLogWriter(Log::Filter filter);

	void Write(const Log::Entry& entry) override;
};