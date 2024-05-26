#pragma once

#include <chrono>
#include <filesystem>
#include <string>

namespace TimeUtils {

    enum TimeUnit { MICRO, MILLI, SECOND, MINUTE, HOUR, DAY, WEEK, YEAR };

    //%Y/%m/%d %H:%M:%S
    std::string DateTimeLocalToString(const std::chrono::system_clock::time_point& dateTime, const std::string& format);
    std::string DateTimeLocalToString(const std::chrono::system_clock::time_point& dateTime);
    std::string DateTimeLocalToString(const std::filesystem::file_time_type::clock::time_point& dateTime, const std::string& format);
    std::string DateTimeLocalToString(const std::filesystem::file_time_type::clock::time_point& dateTime);
    std::string DateTimeUtcToString(const std::chrono::system_clock::time_point& dateTime, const std::string& format);
    std::string DateTimeUtcToString(const std::chrono::system_clock::time_point& dateTime);
    std::string DateTimeUtcToString(const std::filesystem::file_time_type::clock::time_point& dateTime, const std::string& format);
    std::string DateTimeUtcToString(const std::filesystem::file_time_type::clock::time_point& dateTime);

    //%Y/%m/%d %H:%M:%S
    std::string TodayNowLocalToString(const std::string& format);
    std::string TodayNowLocalToString();
    std::string TodayNowUtcToString(const std::string& format);
    std::string TodayNowUtcToString();

    //%Y/%m/%d
    std::string DateLocalToString(const std::chrono::system_clock::time_point& date, const std::string& format);
    std::string DateLocalToString(const std::chrono::system_clock::time_point& date);
    std::string DateLocalToString(const std::filesystem::file_time_type::clock::time_point& date, const std::string& format);
    std::string DateLocalToString(const std::filesystem::file_time_type::clock::time_point& date);
    std::string DateUtcToString(const std::chrono::system_clock::time_point& date, const std::string& format);
    std::string DateUtcToString(const std::chrono::system_clock::time_point& date);
    std::string DateUtcToString(const std::filesystem::file_time_type::clock::time_point& date, const std::string& format);
    std::string DateUtcToString(const std::filesystem::file_time_type::clock::time_point& date);

    //%Y/%m/%d
    std::string TodayLocalToString(const std::string& format);
    std::string TodayLocalToString();
    std::string TodayUtcToString(const std::string& format);
    std::string TodayUtcToString();

    //%H:%M:%S
    std::string TimeLocalToString(const std::chrono::system_clock::time_point& time, const std::string& format);
    std::string TimeLocalToString(const std::chrono::system_clock::time_point& time);
    std::string TimeLocalToString(const std::filesystem::file_time_type::clock::time_point& time, const std::string& format);
    std::string TimeLocalToString(const std::filesystem::file_time_type::clock::time_point& time);
    std::string TimeUtcToString(const std::chrono::system_clock::time_point& time, const std::string& format);
    std::string TimeUtcToString(const std::chrono::system_clock::time_point& time);
    std::string TimeUtcToString(const std::filesystem::file_time_type::clock::time_point& time, const std::string& format);
    std::string TimeUtcToString(const std::filesystem::file_time_type::clock::time_point& time);

    //%H:%M:%S
    std::string NowLocalToString(const std::string& format);
    std::string NowLocalToString();
    std::string NowUtcToString(const std::string& format);
    std::string NowUtcToString();

    std::chrono::system_clock::time_point StringToTimePoint(const std::string& timeString, const std::string& format);

    std::chrono::system_clock::time_point FileTimeToSystemTime(const std::filesystem::file_time_type& fileTime);
    std::filesystem::file_time_type SystemTimeToFileTime(const std::chrono::system_clock::time_point& sysTime);

    //

    std::string DurationToString(const std::chrono::microseconds& duration, TimeUnit minTimeUnit = TimeUnit::MICRO);

} // namespace TimeUtils