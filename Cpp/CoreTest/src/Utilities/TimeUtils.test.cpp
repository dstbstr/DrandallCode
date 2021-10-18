#include "Utilities/TimeUtils.h"

#include "TestCommon.h"

#include <algorithm>

using ::testing::MatchesRegex;

TEST(TimeUtils, CanPrintNow) {
    auto result = TimeUtils::NowUtcToString();

    ASSERT_NE(result, "");
}

TEST(TimeUtils, CanPrintNowWithFormat) {
    auto result = TimeUtils::NowUtcToString("%H_%M_%S");

    ASSERT_THAT(result, MatchesRegex(R"(\d+_\d+_\d+)"));
}

TEST(TimeUtils, CanPrintToday) {
    auto result = TimeUtils::TodayUtcToString();

    ASSERT_NE(result, "");
}

TEST(TimeUtils, CanFormatToday) {
    auto result = TimeUtils::TodayUtcToString("%Y_%m_%d");

    ASSERT_THAT(result, MatchesRegex(R"(\d+_\d+_\d+)"));
}

TEST(TimeUtils, CanPrintTodayNow) {
    auto result = TimeUtils::TodayNowUtcToString();

    ASSERT_NE(result, "");
}

TEST(TimeUtils, CanFormatTodayNow) {
    auto result = TimeUtils::TodayNowUtcToString("%Y_%m_%d %H_%M_%S");

    ASSERT_THAT(result, MatchesRegex(R"(\d+_\d+_\d+ \d+_\d+_\d+)"));
}

TEST(TimeUtils, CanExtractTimeFromString) {
    const std::string format = "%Y_%m_%d %H_%M_%S";
    std::string timeString = TimeUtils::TodayNowUtcToString(format);
    auto time = TimeUtils::StringToTimePoint(timeString, format);

    ASSERT_TRUE(std::chrono::system_clock::now() - time < std::chrono::duration(std::chrono::seconds(1)));
}

TEST(TimeUtils, CanConvertSystemTimeToFileTime) {
    auto sysTime = std::chrono::system_clock::now();
    auto fileTime = TimeUtils::SystemTimeToFileTime(sysTime);
    ASSERT_TRUE(std::filesystem::file_time_type::clock::now() - fileTime < std::chrono::duration(std::chrono::seconds(1)));
}

TEST(TimeUtils, CanConvertFileTimeToSystemTime) {
    auto fileTime = std::filesystem::file_time_type::clock::now();
    auto sysTime = TimeUtils::FileTimeToSystemTime(fileTime);
    ASSERT_TRUE(std::chrono::system_clock::now() - sysTime < std::chrono::duration(std::chrono::seconds(1)));
}