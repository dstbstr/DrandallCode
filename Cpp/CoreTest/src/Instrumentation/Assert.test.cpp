#include "Instrumentation/Assert.h"

#include "Instrumentation/LogWriter/AssertToException.h"
#include "TestCommon.h"

#pragma warning(push)
#pragma warning(disable : 4127) // comparing against a constant
class AssertTest : public ::testing::Test {
    AssertToException ate{};
};

TEST_F(AssertTest, AssertWithFalseConditionAsserts) {
    ASSERT_ANY_THROW(ASSERT(false));
}

TEST_F(AssertTest, AssertWithTrueConditionDoesNotAssert) {
    ASSERT_NO_THROW(ASSERT(true));
}

TEST_F(AssertTest, AssertWithFalseExpressionShowsExpression) {
    CAPTURE_EXCEPTION_E(ASSERT(20 < 10), std::exception) {
        ASSERT_STREQ("20 < 10", e.what());
    }
}

TEST_F(AssertTest, AssertMsgFailKeepsMessage) {
    CAPTURE_EXCEPTION_E(ASSERT_MSG(false, "test"), std::exception) {
        ASSERT_STREQ("test", e.what());
    }
}

TEST_F(AssertTest, AssertMsgSuccessDoesNotAssert) {
    ASSERT_NO_THROW(ASSERT_MSG(true, "Test"));
}
#pragma warning(pop)