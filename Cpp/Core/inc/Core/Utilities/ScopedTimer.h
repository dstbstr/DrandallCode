#pragma once

#include "Core/Instrumentation/Logging.h"
#include "Core/Utilities/Format.h"
#include "Core/Utilities/StringUtils.h"
#include "Core/Utilities/TimeUtils.h"

#include <chrono>
#include <functional>
#include <string>

class ScopedTimer {
public:
    explicit ScopedTimer(std::string label = "", TimeUtils::TimeUnit minTimeUnit = TimeUtils::TimeUnit::MICRO) : m_Label(label), m_MinTimeUnit(minTimeUnit), m_StartTime(std::chrono::high_resolution_clock::now()) {}
    ScopedTimer(std::function<void(std::chrono::microseconds)> OnExit, std::string label = "", TimeUtils::TimeUnit minTimeUnit = TimeUtils::TimeUnit::MICRO) : ScopedTimer(label, minTimeUnit) {
        m_OnExit = OnExit;
    }

    ~ScopedTimer() {
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_StartTime);
        if (m_OnExit) {
            m_OnExit(elapsed);
        }
        else {
            Log::Info(StrUtil::Format("%s ElapsedTime: %s", m_Label, TimeUtils::DurationToString(elapsed, m_MinTimeUnit)));
        }
    }

private:
    std::chrono::steady_clock::time_point m_StartTime;
    std::string m_Label;
    TimeUtils::TimeUnit m_MinTimeUnit;
    std::function<void(std::chrono::microseconds)> m_OnExit{};
};