#include "Core/Utilities/TimeUtils.h"

#include "Core/Utilities/Format.h"
#include "Core/Utilities/StringUtils.h"

#include <functional>
#include <iomanip>
#include <sstream>

using c = std::chrono::system_clock;
using tp = c::time_point;
using ft = std::filesystem::file_time_type;

namespace {
    constexpr char DefaultDateFormat[]{"%Y/%m/%d"};
    constexpr char DefaultDateTimeFormat[]{"%Y/%m/%d %H:%M:%S"};
    constexpr char DefaultTimeFormat[]{"%H:%M:%S"};

    constexpr f64 MicroToMilli = 1.0 / 1000.0;
    constexpr f64 MicroToSecond = MicroToMilli / 1000.0;
    constexpr f64 MicroToMinute = MicroToSecond / 60;
    constexpr f64 MicroToHour = MicroToMinute / 60;
    constexpr f64 MicroToDay = MicroToHour / 24;
    constexpr f64 MicroToWeek = MicroToDay / 7;
    constexpr f64 MicroToYear = MicroToDay / 365;
    constexpr u64 MinMillis = 1000;
    constexpr u64 MinSeconds = MinMillis * 1000;
    constexpr u64 MinMinutes = MinSeconds * 60;
    constexpr u64 MinHours = MinMinutes * 60;
    constexpr u64 MinDays = MinHours * 24;
    constexpr u64 MinWeeks = MinDays * 7;
    constexpr u64 MinYears = MinDays * 365;

    std::string TimePointToString(const tp& time, const char* format, std::function<errno_t(tm*, const time_t*)> convertFunc) {
        std::stringstream ss;

        auto ts = c::to_time_t(time);
        tm theTime;
        convertFunc(&theTime, &ts);

        ss << std::put_time(&theTime, format);
        return ss.str();
    }

    std::string TimePointLocalToString(const tp& time, const char* format) {
        return TimePointToString(time, format, _localtime64_s);
    }
    std::string TimePointLocalToString(const ft& time, const char* format) {
        return TimePointLocalToString(TimeUtils::FileTimeToSystemTime(time), format);
    }

    std::string TimePointUtcToString(const tp& time, const char* format) {
        return TimePointToString(time, format, _gmtime64_s);
    }
    std::string TimePointUtcToString(const ft& time, const char* format) {
        return TimePointUtcToString(TimeUtils::FileTimeToSystemTime(time), format);
    }

    std::string DurationToStringImpl(bool negative, f64 elapsed, const char* unit) {
        if(negative) {
            elapsed *= -1.0;
        }
        return StrUtil::Trim(StrUtil::Format("%.2f %s", elapsed, unit));
    }
} // namespace

namespace TimeUtils {
    std::string DateTimeLocalToString(const tp& dateTime, const std::string& format) {
        return TimePointLocalToString(dateTime, format.c_str());
    }
    std::string DateTimeLocalToString(const tp& dateTime) {
        return TimePointLocalToString(dateTime, DefaultDateTimeFormat);
    }
    std::string DateTimeLocalToString(const ft& dateTime, const std::string& format) {
        return TimePointLocalToString(dateTime, format.c_str());
    }
    std::string DateTimeLocalToString(const ft& dateTime) {
        return TimePointLocalToString(dateTime, DefaultDateTimeFormat);
    }
    std::string DateTimeUtcToString(const tp& dateTime, const std::string& format) {
        return TimePointUtcToString(dateTime, format.c_str());
    }
    std::string DateTimeUtcToString(const tp& dateTime) {
        return TimePointUtcToString(dateTime, DefaultDateTimeFormat);
    }
    std::string DateTimeUtcToString(const ft& dateTime, const std::string& format) {
        return TimePointUtcToString(dateTime, format.c_str());
    }
    std::string DateTimeUtcToString(const ft& dateTime) {
        return TimePointUtcToString(dateTime, DefaultDateTimeFormat);
    }

    std::string TodayNowLocalToString(const std::string& format) {
        return TimePointLocalToString(c::now(), format.c_str());
    }
    std::string TodayNowLocalToString() {
        return TimePointLocalToString(c::now(), DefaultDateTimeFormat);
    }
    std::string TodayNowUtcToString(const std::string& format) {
        return TimePointUtcToString(c::now(), format.c_str());
    }
    std::string TodayNowUtcToString() {
        return TimePointUtcToString(c::now(), DefaultDateTimeFormat);
    }

    std::string DateLocalToString(const tp& date, const std::string& format) {
        return TimePointLocalToString(date, format.c_str());
    }
    std::string DateLocalToString(const tp& date) {
        return TimePointLocalToString(date, DefaultDateFormat);
    }
    std::string DateLocalToString(const ft& date, const std::string& format) {
        return TimePointLocalToString(date, format.c_str());
    }
    std::string DateLocalToString(const ft& date) {
        return TimePointLocalToString(date, DefaultDateFormat);
    }
    std::string DateUtcToString(const tp& date, const std::string& format) {
        return TimePointUtcToString(date, format.c_str());
    }
    std::string DateUtcToString(const tp& date) {
        return TimePointUtcToString(date, DefaultDateFormat);
    }
    std::string DateUtcToString(const ft& date, const std::string& format) {
        return TimePointUtcToString(date, format.c_str());
    }
    std::string DateUtcToString(const ft& date) {
        return TimePointUtcToString(date, DefaultDateFormat);
    }

    std::string TodayLocalToString(const std::string& format) {
        return TimePointLocalToString(c::now(), format.c_str());
    }
    std::string TodayLocalToString() {
        return TimePointLocalToString(c::now(), DefaultDateFormat);
    }
    std::string TodayUtcToString(const std::string& format) {
        return TimePointUtcToString(c::now(), format.c_str());
    }
    std::string TodayUtcToString() {
        return TimePointUtcToString(c::now(), DefaultDateFormat);
    }

    std::string TimeLocalToString(const tp& time, const std::string& format) {
        return TimePointLocalToString(time, format.c_str());
    }
    std::string TimeLocalToString(const tp& time) {
        return TimePointLocalToString(time, DefaultTimeFormat);
    }
    std::string TimeLocalToString(const ft& time, const std::string& format) {
        return TimePointLocalToString(time, format.c_str());
    }
    std::string TimeLocalToString(const ft& time) {
        return TimePointLocalToString(time, DefaultTimeFormat);
    }
    std::string TimeUtcToString(const tp& time, const std::string& format) {
        return TimePointUtcToString(time, format.c_str());
    }
    std::string TimeUtcToString(const tp& time) {
        return TimePointUtcToString(time, DefaultTimeFormat);
    }
    std::string TimeUtcToString(const ft& time, const std::string& format) {
        return TimePointUtcToString(time, format.c_str());
    }
    std::string TimeUtcToString(const ft& time) {
        return TimePointUtcToString(time, DefaultTimeFormat);
    }

    std::string NowLocalToString(const std::string& format) {
        return TimePointLocalToString(c::now(), format.c_str());
    }
    std::string NowLocalToString() {
        return TimeLocalToString(c::now());
    }
    std::string NowUtcToString(const std::string& format) {
        return TimePointUtcToString(c::now(), format.c_str());
    }
    std::string NowUtcToString() {
        return TimeUtcToString(c::now());
    }

    std::string DurationToString(const std::chrono::microseconds& duration, TimeUnit minUnit) {
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        bool negative = microseconds < 0;
        microseconds = std::abs(microseconds);

        TimeUnit timeUnit = minUnit;

        if(microseconds < MinMillis) {
            timeUnit = std::max(timeUnit, TimeUnit::MICRO);
        } else if(microseconds < MinSeconds) {
            timeUnit = std::max(timeUnit, TimeUnit::MILLI);
        } else if(microseconds < MinMinutes) {
            timeUnit = std::max(timeUnit, TimeUnit::SECOND);
        } else if(microseconds < MinHours) {
            timeUnit = std::max(timeUnit, TimeUnit::MINUTE);
        } else if(microseconds < MinDays) {
            timeUnit = std::max(timeUnit, TimeUnit::HOUR);
        } else if(microseconds < MinWeeks) {
            timeUnit = std::max(timeUnit, TimeUnit::DAY);
        } else if(microseconds < MinYears) {
            timeUnit = std::max(timeUnit, TimeUnit::WEEK);
        } else {
            timeUnit = TimeUnit::YEAR;
        }

        switch(timeUnit) {
        case TimeUnit::MICRO: return DurationToStringImpl(negative, microseconds * 1.0, "microseconds");
        case TimeUnit::MILLI: return DurationToStringImpl(negative, microseconds * MicroToMilli, "milliseconds");
        case TimeUnit::SECOND: return DurationToStringImpl(negative, microseconds * MicroToSecond, "seconds");
        case TimeUnit::MINUTE: return DurationToStringImpl(negative, microseconds * MicroToMinute, "minutes");
        case TimeUnit::HOUR: return DurationToStringImpl(negative, microseconds * MicroToHour, "hours");
        case TimeUnit::DAY: return DurationToStringImpl(negative, microseconds * MicroToDay, "days");
        case TimeUnit::WEEK: return DurationToStringImpl(negative, microseconds * MicroToWeek, "weeks");
        case TimeUnit::YEAR: return DurationToStringImpl(negative, microseconds * MicroToYear, "years");
        default: return "Invalid time unit.";
        };
    }

    tp StringToTimePoint(const std::string& timeString, const std::string& format) {
        std::tm tm = {};
        std::stringstream ss(timeString);
        ss >> std::get_time(&tm, format.c_str());
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }

    tp FileTimeToSystemTime(const ft& fileTime) {
        auto fileNow = ft::clock::now();
        auto sysNow = c::now();

        return sysNow - (fileNow - fileTime);
    }

    ft SystemTimeToFileTime(const tp& sysTime) {
        auto fileNow = ft::clock::now();
        auto sysNow = c::now();

        return fileNow - (sysNow - sysTime);
    }

} // namespace TimeUtils