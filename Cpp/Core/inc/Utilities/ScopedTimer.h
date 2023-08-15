#ifndef __SCOPEDTIMER_H__
#define __SCOPEDTIMER_H__

#include "Instrumentation/Log.h"
#include "Utilities/Format.h"
#include "Utilities/StringUtils.h"
#include "Utilities/TimeUtils.h"

#include <chrono>

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
            LOG_INFO(StrUtil::Format("%s ElapsedTime: %s", m_Label, TimeUtils::DurationToString(elapsed, m_MinTimeUnit)));
        }
    }

private:
    std::chrono::steady_clock::time_point m_StartTime;
    std::string m_Label;
    TimeUtils::TimeUnit m_MinTimeUnit;
    std::function<void(std::chrono::microseconds)> m_OnExit{};
};
#endif // __SCOPEDTIMER_H__