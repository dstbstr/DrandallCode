#include "Workers/HappyBirthday.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::MatchesRegex;

TEST(HappyBirthday, Sings) {
    HappyBirthday singer;
    ASSERT_NE("", singer.Sing("Test"));
}

TEST(HappyBirthday, IncludesTargetName) {
    HappyBirthday singer;
    ASSERT_EQ("Happy birthday to you.  Happy birthday to you.  Happy birthday dear Test.  Happy birthday to you.", singer.Sing("Test"));
}