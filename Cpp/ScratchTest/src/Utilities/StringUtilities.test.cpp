#include "Utilities/StringUtilities.h"

#include "TestCommon.h"

TEST(TrimStart, RemovesSpaceFromStart) {
    std::string test = " test";
    ASSERT_EQ(StrUtil::trimStart(test), "test");
}

TEST(TrimStart, RemovesMultipleSpacesFromStart) {
    std::string test = "   test";
    ASSERT_EQ(StrUtil::trimStart(test), "test");
}

TEST(TrimStart, DoesNotRemoveSpacesFromEnd) {
    std::string test = "test   ";
    ASSERT_EQ(StrUtil::trimStart(test), test);
}

TEST(TrimStart, DoesNotModifyTheInputString) {
    std::string test = "  test";
    ASSERT_NE(StrUtil::trimStart(test), test);
}

TEST(TrimStart, CanClearAnEntireString) {
    std::string test = "   ";
    ASSERT_EQ(StrUtil::trimStart(test), "");
}

TEST(TrimStart, WorksWithEmptyInput) {
    std::string test = "";
    ASSERT_EQ(StrUtil::trimStart(test), "");
}

TEST(TrimEnd, RemovesSpaceFromEnd) {
    std::string test = "test ";
    ASSERT_EQ(StrUtil::trimEnd(test), "test");
}

TEST(TrimEnd, RemovesMultiplesSpacesFromEnd) {
    std::string test = "test   ";
    ASSERT_EQ(StrUtil::trimEnd(test), "test");
}

TEST(TrimEnd, DoesNotRemoveSpacesFromStart) {
    std::string test = "  test";
    ASSERT_EQ(StrUtil::trimEnd(test), test);
}

TEST(TrimEnd, DoesNotModifyTheInputString) {
    std::string test = "test  ";
    ASSERT_NE(StrUtil::trimEnd(test), test);
}

TEST(TrimEnd, CanClearAnEntireString) {
    std::string test = "   ";
    ASSERT_EQ(StrUtil::trimEnd(test), "");
}

TEST(TrimEnd, WorksWithEmptyInput) {
    std::string test = "";
    ASSERT_EQ(StrUtil::trimEnd(test), "");
}

TEST(Trim, TrimsFromStartAndEnd) {
    std::string test = "  test  ";
    ASSERT_EQ(StrUtil::trim(test), "test");
}

TEST(Trim, DoesNotModifyInputString) {
    std::string test = "  test  ";
    ASSERT_NE(StrUtil::trim(test), test);
}

TEST(Trim, CanClearAnEntireString) {
    std::string test = "   ";
    ASSERT_EQ(StrUtil::trim(test), "");
}

TEST(Trim, WorksWithEmptyInput) {
    std::string test = "";
    ASSERT_EQ(StrUtil::trim(test), "");
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

TEST(SplitString, StringEndingWIthDelimiterDoesNotAddEmptyEntry) {
    auto result = StrUtil::Split("one,two,", ",");
    ASSERT_EQ(2, result.size());
    ASSERT_EQ(result[0], "one");
    ASSERT_EQ(result[1], "two");
}