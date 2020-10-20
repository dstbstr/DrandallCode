#include "Utilities/StringUtilities.h"
#include "gtest/gtest.h"

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
