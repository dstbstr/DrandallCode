#include "CommandParser/Option.h"

#include "TestCommon.h"

using ::testing::HasSubstr;

TEST(OptionToString, IncludesShortNameIfAvailable) {
    CommandParser::IntOption opt("a", "", false);
    ASSERT_THAT(opt.ToString(), HasSubstr("-a"));
}

TEST(OptionToString, IncludesLongNameIfAvailable) {
    CommandParser::IntOption opt("", "longName", false);
    ASSERT_THAT(opt.ToString(), HasSubstr("--longName"));
}

TEST(OptionToString, IncludesHelpTextIfAvailable) {
    CommandParser::IntOption opt("", "", false, "Help Text");
    ASSERT_THAT(opt.ToString(), HasSubstr("Help Text"));
}

TEST(OptionToString, IndicatesAnOptionIsRequired) {
    CommandParser::IntOption opt("", "", true);
    ASSERT_THAT(opt.ToString(), HasSubstr("X"));
}

TEST(OptionToString, IndicatesAnOptionIsAFlag) {
    CommandParser::BoolOption opt("", "", false);
    ASSERT_THAT(opt.ToString(), HasSubstr("X"));
}

TEST(OptionParse, CanParseAsString) {
    CommandParser::StringOption opt("", "", false);
    opt.Populate("test");

    ASSERT_EQ(opt.GetValue(), "test");
}

TEST(OptionParse, CanParseAsInt) {
    CommandParser::IntOption opt("", "", false);
    opt.Populate("123");

    ASSERT_EQ(123, opt.GetValue());
}

TEST(OptionParse, CanParseAsBool) {
    CommandParser::BoolOption opt("", "", false);
    ASSERT_TRUE(opt.GetValue());
}

TEST(OptionParse, CanParseAsVectorOfStrings) {
    CommandParser::VecStringOption opt("", "", false);
    opt.Populate("1,2,3");
    auto output = opt.GetValue();

    ASSERT_EQ(3, output.size());
    ASSERT_EQ(output[0], "1");
    ASSERT_EQ(output[2], "3");
}

TEST(OptionParse, CanParseASingleValueAsVectorOfStrings) {
    CommandParser::VecStringOption opt("", "", false);
    opt.Populate("1");
    auto output = opt.GetValue();

    ASSERT_EQ(1, output.size());
    ASSERT_EQ(output[0], "1");
}

TEST(OptionParse, CanParseAsVectorOfInts) {
    CommandParser::VecIntOption opt("", "", false);
    opt.Populate("1,2,3");
    auto output = opt.GetValue();

    ASSERT_EQ(3, output.size());
    ASSERT_EQ(output[0], 1);
    ASSERT_EQ(output[2], 3);
}

TEST(OptionParse, CanParseASingleValueAsVectorOfInts) {
    CommandParser::VecIntOption opt("", "", false);
    opt.Populate("1");
    auto output = opt.GetValue();

    ASSERT_EQ(1, output.size());
    ASSERT_EQ(output[0], 1);
}