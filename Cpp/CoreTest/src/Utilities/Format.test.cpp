#include "Utilities/Format.h"

#include "TestCommon.h"

#include <string>

TEST(Format, WorksWithSimpleSubstitution) {
    auto result = StrUtil::Format("%d", 7);
    ASSERT_EQ(result, "7");
}

TEST(Format, WorksWithRawCharacterStrings) {
    auto result = StrUtil::Format("%s", "test");
    ASSERT_EQ(result, "test");
}

TEST(Format, WorksWithNumericFormatting) {
    auto result = StrUtil::Format("%05d", 9);
    ASSERT_EQ(result, "00009");
}

TEST(Format, WorksWithStdStrings) {
    std::string myString = "test";
    auto result = StrUtil::Format("%s", myString);

    ASSERT_EQ(result, myString);
}
