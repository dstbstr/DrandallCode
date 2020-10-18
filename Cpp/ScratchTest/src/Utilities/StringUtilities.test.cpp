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