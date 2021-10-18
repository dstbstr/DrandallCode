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

    ~ScopedTimer() {
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_StartTime);
        LOG_INFO(StrUtil::Format("%s ElapsedTime: %s", m_Label, TimeUtils::DurationToString(elapsed, m_MinTimeUnit)));
    }

private:
    std::chrono::steady_clock::time_point m_StartTime;
    std::string m_Label;
    TimeUtils::TimeUnit m_MinTimeUnit;
};
#endif // __SCOPEDTIMER_H__