#ifndef __TIMEUTILS_H__
#define __TIMEUTILS_H__

#include <chrono>
#include <string>

namespace TimeUtils {
    //%Y/%m/%d %H:%M:%S
    std::string DateTimeToString(std::chrono::system_clock::time_point dateTime, std::string format);
    std::string DateTimeToString(std::chrono::system_clock::time_point dateTime);
    //%Y/%m/%d %H:%M:%S
    std::string TodayNowToString(std::string format);
    std::string TodayNowToString();

    //%Y/%m/%d
    std::string DateToString(std::chrono::system_clock::time_point date, std::string format);
    std::string DateToString(std::chrono::system_clock::time_point date);
    //%Y/%m/%d
    std::string TodayToString(std::string format);
    std::string TodayToString();

    //%H:%M:%S
    std::string TimeToString(std::chrono::system_clock::time_point time, std::string format);
    std::string TimeToString(std::chrono::system_clock::time_point time);
    //%H:%M:%S
    std::string NowToString(std::string format);
    std::string NowToString();
} // namespace TimeUtils
#endif // __TIMEUTILS_H__