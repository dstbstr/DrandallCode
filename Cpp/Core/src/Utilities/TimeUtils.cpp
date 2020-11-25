#include "Utilities/TimeUtils.h"

#include <iomanip>
#include <sstream>

using c = std::chrono::system_clock;
using tp = c::time_point;

namespace {
    constexpr char DefaultDateFormat[]{"%Y/%m/%d"};
    constexpr char DefaultDateTimeFormat[]{"%Y/%m/%d %H:%M:%S"};
    constexpr char DefaultTimeFormat[]{"%H:%M:%S"};

    std::string TimePointToString(tp time, const char* format) {
        std::stringstream ss;

        auto ts = c::to_time_t(time);
        tm utc;
        _gmtime64_s(&utc, &ts);

        ss << std::put_time(&utc, format);
        return ss.str();
    }
} // namespace

namespace TimeUtils {
    std::string DateTimeToString(tp dateTime, std::string format) {
        return TimePointToString(dateTime, format.c_str());
    }
    std::string DateTimeToString(tp dateTime) {
        return TimePointToString(dateTime, DefaultDateTimeFormat);
    }
    std::string TodayNowToString(std::string format) {
        return TimePointToString(c::now(), format.c_str());
    }
    std::string TodayNowToString() {
        return TimePointToString(c::now(), DefaultDateTimeFormat);
    }

    std::string DateToString(tp date, std::string format) {
        return TimePointToString(date, format.c_str());
    }
    std::string DateToString(tp date) {
        return TimePointToString(date, DefaultDateFormat);
    }
    std::string TodayToString(std::string format) {
        return TimePointToString(c::now(), format.c_str());
    }
    std::string TodayToString() {
        return TimePointToString(c::now(), DefaultDateFormat);
    }

    std::string TimeToString(tp time, std::string format) {
        return TimePointToString(time, format.c_str());
    }
    std::string TimeToString(tp time) {
        return TimePointToString(time, DefaultTimeFormat);
    }
    std::string NowToString(std::string format) {
        return TimePointToString(c::now(), format.c_str());
    }
    std::string NowToString() {
        return TimeToString(c::now());
    }
} // namespace TimeUtils