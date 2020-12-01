#ifndef __SCOPEDTIMER_H__
#define __SCOPEDTIMER_H__

#include "Instrumentation/Log.h"
#include "Utilities/Format.h"
#include "Utilities/StringUtils.h"

#include <chrono>

class ScopedTimer {
public:
    enum TimeUnit { MICRO, MILLI, SECOND, MINUTE, HOUR, DAY, WEEK, YEAR };

    explicit ScopedTimer(std::string label = "", TimeUnit minTimeUnit = TimeUnit::MICRO) : m_Label(label), m_MinTimeUnit(minTimeUnit), m_StartTime(std::chrono::high_resolution_clock::now()) {}

    ~ScopedTimer() {
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_StartTime).count();
        if(microseconds < 1000) {
            if(m_MinTimeUnit <= TimeUnit::MICRO) {
                LOG_INFO(GetMessageMicros(microseconds));
            }
            return;
        }
        double elapsed = std::floor(microseconds / 1000);
        if(elapsed < 1000) {
            if(m_MinTimeUnit <= TimeUnit::MILLI) {
                LOG_INFO(GetMessage(elapsed, "milliseconds"));
            }
            return;
        }
        elapsed /= 1000;
        if(elapsed < 60) {
            if(m_MinTimeUnit <= TimeUnit::SECOND) {
                LOG_INFO(GetMessage(elapsed, "seconds"));
            }
            return;
        }
        elapsed /= 60;
        if(elapsed < 60) {
            if(m_MinTimeUnit <= TimeUnit::MINUTE) {
                LOG_INFO(GetMessage(elapsed, "minutes"));
            }
            return;
        }
        elapsed /= 60;
        if(elapsed < 24) {
            if(m_MinTimeUnit <= TimeUnit::HOUR) {
                LOG_INFO(GetMessage(elapsed, "hours"));
            }
            return;
        }
        if(elapsed < 7) {
            if(m_MinTimeUnit <= TimeUnit::DAY) {
                LOG_INFO(GetMessage(elapsed, "days"));
            }
            return;
        }
        elapsed /= 7;
        if(elapsed < 52) {
            if(m_MinTimeUnit <= TimeUnit::WEEK) {
                LOG_INFO(GetMessage(elapsed, "weeks"));
            }
            return;
        }
        elapsed /= 52;
        LOG_INFO(GetMessage(elapsed, "years"));
    }

private:
    std::chrono::steady_clock::time_point m_StartTime;
    std::string m_Label;
    TimeUnit m_MinTimeUnit;

    std::string GetMessage(double elapsed, std::string units) const {
        return StrUtil::Trim(StrUtil::Format("%s Elapsed Time: %.2f %s", m_Label, elapsed, units));
    }

    std::string GetMessageMicros(u64 elapsed) const {
        return StrUtil::Trim(StrUtil::Format("%s Elapsed Time: %d microseconds", m_Label, elapsed));
    }
};
#endif // __SCOPEDTIMER_H__