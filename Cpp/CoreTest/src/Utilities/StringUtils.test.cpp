#include "TestCommon.h"

#include "Core/Utilities/StringUtils.h"

TEST(TrimStart, RemovesSpaceFromStart) {
    std::string test = " test";
    ASSERT_EQ(StrUtil::TrimStart(test), "test");
}

TEST(TrimStart, RemovesMultipleSpacesFromStart) {
    std::string test = "   test";
    ASSERT_EQ(StrUtil::TrimStart(test), "test");
}

TEST(TrimStart, DoesNotRemoveSpacesFromEnd) {
    std::string test = "test   ";
    ASSERT_EQ(StrUtil::TrimStart(test), test);
}

TEST(TrimStart, DoesNotModifyTheInputString) {
    std::string test = "  test";
    ASSERT_NE(StrUtil::TrimStart(test), test);
}

TEST(TrimStart, CanClearAnEntireString) {
    std::string test = "   ";
    ASSERT_EQ(StrUtil::TrimStart(test), "");
}

TEST(TrimStart, WorksWithEmptyInput) {
    std::string test = "";
    ASSERT_EQ(StrUtil::TrimStart(test), "");
}

TEST(TrimEnd, RemovesSpaceFromEnd) {
    std::string test = "test ";
    ASSERT_EQ(StrUtil::TrimEnd(test), "test");
}

TEST(TrimEnd, RemovesMultiplesSpacesFromEnd) {
    std::string test = "test   ";
    ASSERT_EQ(StrUtil::TrimEnd(test), "test");
}

TEST(TrimEnd, DoesNotRemoveSpacesFromStart) {
    std::string test = "  test";
    ASSERT_EQ(StrUtil::TrimEnd(test), test);
}

TEST(TrimEnd, DoesNotModifyTheInputString) {
    std::string test = "test  ";
    ASSERT_NE(StrUtil::TrimEnd(test), test);
}

TEST(TrimEnd, CanClearAnEntireString) {
    std::string test = "   ";
    ASSERT_EQ(StrUtil::TrimEnd(test), "");
}

TEST(TrimEnd, WorksWithEmptyInput) {
    std::string test = "";
    ASSERT_EQ(StrUtil::TrimEnd(test), "");
}

TEST(Trim, TrimsFromStartAndEnd) {
    std::string test = "  test  ";
    ASSERT_EQ(StrUtil::Trim(test), "test");
}

TEST(Trim, DoesNotModifyInputString) {
    std::string test = "  test  ";
    ASSERT_NE(StrUtil::Trim(test), test);
}

TEST(Trim, CanClearAnEntireString) {
    std::string test = "   ";
    ASSERT_EQ(StrUtil::Trim(test), "");
}

TEST(Trim, WorksWithEmptyInput) {
    std::string test = "";
    ASSERT_EQ(StrUtil::Trim(test), "");
}

TEST(EqualsIgnoreCase, ExactMatchReturnsTrue) {
    ASSERT_TRUE(StrUtil::EqualsIgnoreCase("test", "test"));
}

TEST(EqualsIgnoreCase, MismatchReturnsFalse) {
    ASSERT_FALSE(StrUtil::EqualsIgnoreCase("hello", "world"));
}

TEST(EqualsIgnoreCase, MatchWithDifferentCaseReturnsTrue) {
    ASSERT_TRUE(StrUtil::EqualsIgnoreCase("test", "TEST"));
}

TEST(EqualsIgnoreCase, CompareAgainstEmptyReturnsFalse) {
    ASSERT_FALSE(StrUtil::EqualsIgnoreCase("hello", ""));
}

TEST(EqualsIgnoreCase, EmptyEqualsEmpty) {
    ASSERT_TRUE(StrUtil::EqualsIgnoreCase("", ""));
}

TEST(ToUpper, EmptyReturnsEmpty) {
    ASSERT_EQ(StrUtil::ToUpper(""), "");
}

TEST(ToUpper, UppercaseWordIsUnchanged) {
    ASSERT_EQ(StrUtil::ToUpper("UPPER"), "UPPER");
}

TEST(ToUpper, LowercaseWordIsUppercased) {
    ASSERT_EQ(StrUtil::ToUpper("lower"), "LOWER");
}

TEST(ToUpper, SymbolsAreUnchanged) {
    ASSERT_EQ(StrUtil::ToUpper("!@#$%^&*()-_=+,./<>?;':\"[]{}\\|`~"), "!@#$%^&*()-_=+,./<>?;':\"[]{}\\|`~");
}

TEST(ToUpper, NumbersAreUnchanged) {
    ASSERT_EQ(StrUtil::ToUpper("1234567890"), "1234567890");
}

TEST(ToLower, EmptyReturnsEmpty) {
    ASSERT_EQ(StrUtil::ToLower(""), "");
}

TEST(ToLower, UppercaseWordIsLowercased) {
    ASSERT_EQ(StrUtil::ToLower("UPPER"), "upper");
}

TEST(ToLower, LowercaseWordIsUnchanged) {
    ASSERT_EQ(StrUtil::ToLower("lower"), "lower");
}

TEST(ToLower, SymbolsAreUnchanged) {
    ASSERT_EQ(StrUtil::ToLower("!@#$%^&*()-_=+,./<>?;':\"[]{}\\|`~"), "!@#$%^&*()-_=+,./<>?;':\"[]{}\\|`~");
}

TEST(ToLower, NumbersAreUnchanged) {
    ASSERT_EQ(StrUtil::ToLower("1234567890"), "1234567890");
}

TEST(StringJoin, JoinVecCombinesArguments) {
    std::vector<std::string> input{"one", "two", "three"};
    auto result = StrUtil::JoinVec("", input);

    ASSERT_STREQ("onetwothree", result.c_str());
}

TEST(StringJoin, JoinVecWithDelimiterDoesNotStartWithDelimiter) {
    std::vector<std::string> input{"one", "two", "three"};
    auto result = StrUtil::JoinVec(',', input);

    ASSERT_THAT(result, Not(StartsWith(",")));
}

TEST(StringJoin, JoinVecWithDelimiterDoesNotEndWithDelimiter) {
    std::vector<std::string> input{"one", "two", "three"};
    auto result = StrUtil::JoinVec(',', input);

    ASSERT_THAT(result, Not(EndsWith(",")));
}

TEST(StringJoin, JoinVecWithDelimiterInsertsDelimiters) {
    std::vector<std::string> input{"one", "two", "three"};
    auto result = StrUtil::JoinVec(',', input);

    ASSERT_STREQ("one,two,three", result.c_str());
}

TEST(StringJoin, JoinVecWithOneElementReturnsUnchanged) {
    std::vector<std::string> input{"one"};
    auto result = StrUtil::JoinVec("", input);

    ASSERT_STRCASEEQ("one", result.c_str());
}

TEST(StringJoin, JoinVecWithOneElementAndDelimiterReturnsUnchanged) {
    std::vector<std::string> input{"one"};
    auto result = StrUtil::JoinVec(',', input);

    ASSERT_STREQ("one", result.c_str());
}

TEST(StringJoin, JoinVecWithStringDelimiterInsertsDelimiters) {
    std::vector<std::string> input{"one", "two", "three"};
    auto result = StrUtil::JoinVec(", ", input);

    ASSERT_STREQ("one, two, three", result.c_str());
}

TEST(StringJoin, AsVectorWithStringDelimiterSingleElementUnchanged) {
    std::vector<std::string> input{"one"};
    auto result = StrUtil::JoinVec(", ", input);

    ASSERT_STREQ("one", result.c_str());
}

TEST(StringJoin, JoinCombinesStrings) {
    auto result = StrUtil::Join("", "one", "two", "three");

    ASSERT_STREQ("onetwothree", result.c_str());
}

TEST(StringJoin, JoinWithDelimiterDoesNotStartWithDelimiter) {
    auto result = StrUtil::Join(',', "one", "two", "three");

    ASSERT_STREQ("one,two,three", result.c_str());
}

TEST(StringJoin, JoinWithStringDelimiterSingleElementUnchanged) {
    auto result = StrUtil::Join(",", "one");

    ASSERT_STREQ("one", result.c_str());
}

TEST(StringJoin, JoinWithStringDelimitersInsertsDelimiters) {
    auto result = StrUtil::Join(", ", "one", "two", "three");

    ASSERT_STREQ("one, two, three", result.c_str());
}

TEST(StringJoin, JoinWorksWithMultipleTypes) {
    auto result = StrUtil::Join("", true, 3.14, "hello", '\t', -1);

    ASSERT_STREQ("13.14hello\t-1", result.c_str());
}

TEST(StringJoin, JoinWorksWithDelimiterAndMultipleTypes) {
    auto result = StrUtil::Join(" ", true, 3.14, "hello", '\t', -1);

    ASSERT_STREQ("1 3.14 hello \t -1", result.c_str());
}

TEST(SplitString, SplitSingleCharacterEntriesWithSingleCharacterDelimiter) {
    auto result = StrUtil::Split("1,2,3", ",");
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(result[0], "1");
    ASSERT_EQ(result[2], "3");
}

TEST(SplitString, SplitSingleWordReturnsVectorWithSingleElement) {
    auto result = StrUtil::Split("test", ",");
    ASSERT_EQ(1, result.size());
    ASSERT_EQ(result[0], "test");
}

TEST(SplitString, SplitStringDoesNotIncludeDelimiterInResult) {
    auto result = StrUtil::Split("hello world", " ");
    ASSERT_EQ(2, result.size());
    ASSERT_EQ(result[0], "hello");
    ASSERT_EQ(result[1], "world");
}

TEST(SplitString, SplitStringWorksWithMultipleCharacterDelimiters) {
    auto result = StrUtil::Split("one, two, three", ", ");
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(result[0], "one");
    ASSERT_EQ(result[2], "three");
}

TEST(SplitString, StringStartingWithDelimiterDoesNotAddEmptyEntry) {
    auto result = StrUtil::Split(",one,two", ",");
    ASSERT_EQ(2, result.size());
    ASSERT_EQ(result[0], "one");
}

TEST(SplitString, SplitStringRemovesEmptyEntries) {
    auto result = StrUtil::Split("one,,,,two", ",");
    ASSERT_EQ(2, result.size());
    ASSERT_EQ(result[0], "one");
    ASSERT_EQ(result[1], "two");
}

TEST(SplitString, StringEndingWithDelimiterDoesNotAddEmptyEntry) {
    auto result = StrUtil::Split("one,two,", ",");
    ASSERT_EQ(2, result.size());
    ASSERT_EQ(result[0], "one");
    ASSERT_EQ(result[1], "two");
}

TEST(StartsWith, FoundPrefixReturnsTrue) {
    ASSERT_TRUE(StrUtil::StartsWith("Welcome", "Wel"));
}

TEST(StartsWith, BeginningDoesNotMatchReturnsFalse) {
    ASSERT_FALSE(StrUtil::StartsWith("Hello", "ell"));
}

TEST(StartsWith, ExactMatchReturnsTrue) {
    ASSERT_TRUE(StrUtil::StartsWith("Hello", "Hello"));
}

TEST(StartsWith, TooLongPrefixReturnsFalse) {
    ASSERT_FALSE(StrUtil::StartsWith("Hi", "Hello"));
}

TEST(StartsWith, BlankPrefixReturnsTrue) {
    ASSERT_TRUE(StrUtil::StartsWith("Hello", ""));
}

TEST(EndsWith, FoundSuffixReturnsTrue) {
    ASSERT_TRUE(StrUtil::EndsWith("Welcome", "come"));
}

TEST(EndsWith, SuffixNotFoundReturnsFalse) {
    ASSERT_FALSE(StrUtil::EndsWith("Hello", "ell"));
}

TEST(EndsWith, ExactMatchReturnsTrue) {
    ASSERT_TRUE(StrUtil::EndsWith("Hello", "Hello"));
}

TEST(EndsWith, TooLongSuffixReturnsFalse) {
    ASSERT_FALSE(StrUtil::EndsWith("Hi", "Hello"));
}

TEST(EndsWith, BlankSuffixReturnsTrue) {
    ASSERT_TRUE(StrUtil::EndsWith("Hello", ""));
}

TEST(EndsWith, SuffixOneLongerThanStringReturnsFalse) {
    ASSERT_FALSE(StrUtil::EndsWith("Test", ".xlsx"));
}
