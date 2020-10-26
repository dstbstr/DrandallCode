#include "CommandParser/Option.h"

#include "TestCommon.h"

using ::testing::HasSubstr;

TEST(OptionToString, IncludesShortName) {
    Option<int> opt("a", "", false);
    ASSERT_THAT(opt.ToString(), HasSubstr("-a"));
}