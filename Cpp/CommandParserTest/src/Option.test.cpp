#include "CommandParser/Option.h"

#include "TestCommon.h"

using ::testing::HasSubstr;

namespace CommandParser {
    TEST(OptionConstruction, AllowsEmptyShortName) {
        ASSERT_NO_THROW(IntOption opt("", "longName", false));
    }

    TEST(OptionConstruction, AllowsEmptyLongName) {
        ASSERT_NO_THROW(IntOption opt("s", "", false));
    }

    TEST(OptionConstruction, DoesNotAllowEmptyShortAndLongName) {
        ASSERT_ANY_THROW(IntOption opt("", "", false));
    }

    TEST(OptionConstruction, AllowsBlankShortName) {
        ASSERT_NO_THROW(IntOption opt(BaseOption::BLANK, "", false));
    }

    TEST(OptionConstruction, AllowsBlankLongName) {
        ASSERT_NO_THROW(IntOption opt("", BaseOption::BLANK, false));
    }

    TEST(OptionConstruction, AllowsShortAndLongToBeBlank) {
        ASSERT_NO_THROW(IntOption opt(BaseOption::BLANK, BaseOption::BLANK, false));
    }

    TEST(OptionConstruction, StripsInvalidCharactersFromShortName) {
        IntOption opt("-a", "", false);
        ASSERT_EQ(opt.GetShortName(), "a");
    }

    TEST(OptionConstruction, StripsInvalidCharactersFromLongName) {
        IntOption opt("", "--longName", false);
        ASSERT_EQ(opt.GetLongName(), "longName");
    }

    TEST(OptionConstruction, RequiresValidCharactersInShortName) {
        ASSERT_ANY_THROW(IntOption opt("--", "", false));
    }

    TEST(OptionConstruction, RequiresValidCharactersInLongName) {
        ASSERT_ANY_THROW(IntOption opt("", "---", false));
    }

    TEST(OptionConstruction, TakesTheFirstValidCharacterSetInShortName) {
        IntOption opt("-a*b&", "", false);
        ASSERT_EQ(opt.GetShortName(), "a");
    }

    TEST(OptionConstruction, TakesTheFirstValidCharacterSetInLongName) {
        IntOption opt("", "-abc*def", false);
        ASSERT_EQ(opt.GetLongName(), "abc");
    }

    TEST(OptionToString, IncludesShortNameIfAvailable) {
        IntOption opt("a", "", false);
        ASSERT_THAT(opt.ToString(), HasSubstr("-a"));
    }

    TEST(OptionToString, IncludesLongNameIfAvailable) {
        IntOption opt("", "longName", false);
        ASSERT_THAT(opt.ToString(), HasSubstr("--longName"));
    }

    TEST(OptionToString, IncludesHelpTextIfAvailable) {
        IntOption opt("a", "", false, "Help Text");
        ASSERT_THAT(opt.ToString(), HasSubstr("Help Text"));
    }

    TEST(OptionToString, IndicatesAnOptionIsRequired) {
        IntOption opt("a", "", true);
        ASSERT_THAT(opt.ToString(), HasSubstr("X"));
    }

    TEST(OptionToString, IndicatesAnOptionIsAFlag) {
        BoolOption opt("a", "", false);
        ASSERT_THAT(opt.ToString(), HasSubstr("X"));
    }

    TEST(OptionParse, CanParseAsString) {
        StringOption opt("a", "", false);
        opt.Populate("test");

        ASSERT_EQ(opt.GetValue(), "test");
    }

    TEST(OptionParse, CanParseAsInt) {
        IntOption opt("a", "", false);
        opt.Populate("123");

        ASSERT_EQ(123, opt.GetValue());
    }

    TEST(OptionParse, CanParseAsBool) {
        BoolOption opt("a", "", false);
        ASSERT_TRUE(opt.GetValue());
    }

    TEST(OptionParse, CanParseAsVectorOfStrings) {
        VecStringOption opt("a", "", false);
        opt.Populate("1,2,3");
        auto output = opt.GetValue();

        ASSERT_EQ(3, output.size());
        ASSERT_EQ(output[0], "1");
        ASSERT_EQ(output[2], "3");
    }

    TEST(OptionParse, CanParseASingleValueAsVectorOfStrings) {
        VecStringOption opt("a", "", false);
        opt.Populate("1");
        auto output = opt.GetValue();

        ASSERT_EQ(1, output.size());
        ASSERT_EQ(output[0], "1");
    }

    TEST(OptionParse, CanParseAsVectorOfInts) {
        VecIntOption opt("a", "", false);
        opt.Populate("1,2,3");
        auto output = opt.GetValue();

        ASSERT_EQ(3, output.size());
        ASSERT_EQ(output[0], 1);
        ASSERT_EQ(output[2], 3);
    }

    TEST(OptionParse, CanParseASingleValueAsVectorOfInts) {
        VecIntOption opt("a", "", false);
        opt.Populate("1");
        auto output = opt.GetValue();

        ASSERT_EQ(1, output.size());
        ASSERT_EQ(output[0], 1);
    }
} // namespace CommandParser