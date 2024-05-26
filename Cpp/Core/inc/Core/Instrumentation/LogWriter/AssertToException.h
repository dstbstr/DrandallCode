#pragma once

#include "Core/Instrumentation/ISink.h"
#include "Core/Utilities/Require.h"
#include "Core/Utilities/StringUtils.h"

struct AssertToException : public Log::ISink {
    AssertToException() : Log::ISink(Log::Filter().WithLevel(Log::Level::Error)) {}

    void Write(const Log::Entry& entry) override {
        Require::False(true, entry.Message);
    }
};