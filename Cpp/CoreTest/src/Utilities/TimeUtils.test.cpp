#include "Utilities/TimeUtils.h"

#include "TestCommon.h"

#include <algorithm>

using ::testing::MatchesRegex;

TEST(TimeUtils, CanPrintNow) {
    auto result = TimeUtils::NowToString();

    ASSERT_NE(result, "");
}

TEST(TimeUtils, CanPrintNowWithFormat) {
    auto result = TimeUtils::NowToString("%H_%M_%S");

    ASSERT_THAT(result, MatchesRegex(R"(\d+_\d+_\d+)"));
}

TEST(TimeUtils, CanPrintToday) {
    auto result = TimeUtils::TodayToString();

    ASSERT_NE(result, "");
}

TEST(TimeUtils, CanFormatToday) {
    auto result = TimeUtils::TodayToString("%Y_%m_%d");

    ASSERT_THAT(result, MatchesRegex(R"(\d+_\d+_\d+)"));
}

TEST(TimeUtils, CanPrintTodayNow) {
    auto result = TimeUtils::TodayNowToString();

    ASSERT_NE(result, "");
}

TEST(TimeUtils, CanFormatTodayNow) {
    auto result = TimeUtils::TodayNowToString("%Y_%m_%d %H_%M_%S");

    ASSERT_THAT(result, MatchesRegex(R"(\d+_\d+_\d+ \d+_\d+_\d+)"));
}