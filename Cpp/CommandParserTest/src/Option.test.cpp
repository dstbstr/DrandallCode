#include "CommandParser/Option.h"

#include "TestCommon.h"

using ::testing::HasSubstr;

TEST(OptionToString, IncludesShortNameIfAvailable) {
    CommandParser::Option<int> opt("a", "", false);
    ASSERT_THAT(opt.ToString(), HasSubstr("-a"));
}

TEST(OptionToString, IncludesLongNameIfAvailable) {
    CommandParser::Option<int> opt("", "longName", false);
    ASSERT_THAT(opt.ToString(), HasSubstr("--longName"));
}

TEST(OptionToString, IncludesHelpTextIfAvailable) {
    CommandParser::Option<int> opt("", "", false, "Help Text");
    ASSERT_THAT(opt.ToString(), HasSubstr("Help Text"));
}

TEST(OptionToString, IndicatesAnOptionIsRequired) {
    CommandParser::Option<int> opt("", "", true);
    ASSERT_THAT(opt.ToString(), HasSubstr("X"));
}

TEST(OptionToString, IndicatesAnOptionIsAFlag) {
    CommandParser::Option<bool> opt("", "", false);
    ASSERT_THAT(opt.ToString(), HasSubstr("X"));
}

TEST(OptionParse, CanParseAsString) {
    CommandParser::Option<std::string> opt("", "", false);
    std::string output = opt.Parse("test");

    ASSERT_EQ(output, "test");
}

TEST(OptionParse, CanParseAsInt) {
    CommandParser::Option<int> opt("", "", false);
    int output = opt.Parse("123");

    ASSERT_EQ(123, output);
}

TEST(OptionParse, CanParseAsBool) {
    CommandParser::Option<bool> opt("", "", false);
    ASSERT_TRUE(opt.Parse(""));
}

TEST(OptionParse, CanParseAsVectorOfStrings) {
    CommandParser::Option<std::vector<std::string>> opt("", "", false);
    std::vector<std::string> output = opt.Parse("1,2,3");

    ASSERT_EQ(3, output.size());
    ASSERT_EQ(output[0], "1");
    ASSERT_EQ(output[2], "3");
}

TEST(OptionParse, CanParseASingleValueAsVectorOfStrings) {
    CommandParser::Option<std::vector<std::string>> opt("", "", false);
    std::vector<std::string> output = opt.Parse("1");

    ASSERT_EQ(1, output.size());
    ASSERT_EQ(output[0], "1");
}

TEST(OptionParse, CanParseAsVectorOfInts) {
    CommandParser::Option<std::vector<int>> opt("", "", false);
    std::vector<int> output = opt.Parse("1,2,3");

    ASSERT_EQ(3, output.size());
    ASSERT_EQ(output[0], 1);
    ASSERT_EQ(output[2], 3);
}

TEST(OptionParse, CanParseASingleValueAsVectorOfInts) {
    CommandParser::Option<std::vector<int>> opt("", "", false);
    std::vector<int> output = opt.Parse("1");

    ASSERT_EQ(1, output.size());
    ASSERT_EQ(output[0], 1);
}