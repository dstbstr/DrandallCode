#include "Utilities/ExpressionEvaluator.h"

#include "TestCommon.h"


bool predicate(std::string input) {
    return input == "1";
}

TEST(ExpressionEvaluator, SimpleTrueExpressionReturnsTrue) {
    ASSERT_TRUE(ExpressionEvaluator::Evaluate("1", predicate));
}