#include "Extractor/Workers/DefineFileParser.h"

#include "TestCommon.h"

#include <sstream>

using Extractor::DefineFileParser::Parse;
using ::testing::UnorderedElementsAre;

TEST(DefineFileParser, SplitsLineOnSpace) {
    std::stringstream ss;
    ss << "A B C";

    auto result = Parse(ss);
    ASSERT_EQ(3, result.size());

    ASSERT_EQ(result["A"], "");
    ASSERT_EQ(result["B"], "");
    ASSERT_EQ(result["C"], "");
}

TEST(DefineFileParser, GathersAllLines) {
    std::stringstream ss;
    ss << "A\n"
       << "B\n"
       << "C";

    auto result = Parse(ss);
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(result["A"], "");
    ASSERT_EQ(result["B"], "");
    ASSERT_EQ(result["C"], "");
}

TEST(DefineFileParser, RemovesDuplicates) {
    std::stringstream ss;
    ss << "A B A B";
    auto result = Parse(ss);

    ASSERT_EQ(2, result.size());
    ASSERT_EQ(result["A"], "");
    ASSERT_EQ(result["B"], "");
}

TEST(DefineFileParser, DoesNotAddBlanksFromSpaces) {
    std::stringstream ss;
    ss << "A    B ";
    auto result = Parse(ss);

    ASSERT_EQ(2, result.size());
    ASSERT_EQ(result["A"], "");
    ASSERT_EQ(result["B"], "");
}

TEST(DefineFileParser, DoesNotAddBlanksFromLines) {
    std::stringstream ss;
    ss << "A\n\n"
       << "B\n";
    auto result = Parse(ss);

    ASSERT_EQ(2, result.size());
    ASSERT_EQ(result["A"], "");
    ASSERT_EQ(result["B"], "");
}

TEST(DefineFileParser, SplitsOnEquals) {
    std::stringstream ss;
    ss << "A=B";
    auto result = Parse(ss);

    ASSERT_EQ(1, result.size());
    ASSERT_EQ(result["A"], "B");
}