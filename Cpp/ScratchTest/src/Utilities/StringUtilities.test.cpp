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

TEST(Trim, TrimsFromStartAndEnd) {
    std::string test = "  test  ";
    ASSERT_EQ(StrUtil::trim(test), "test");
}

TEST(Trim, DoesNotModifyInputString) {
    std::string test = "  test  ";
    ASSERT_NE(StrUtil::trim(test), test);
}