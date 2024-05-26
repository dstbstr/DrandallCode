#include "TestCommon.h"

#include "Core/Utilities/ExpressionEvaluator.h"

bool predicate(std::string input) {
    return input == "1";
}

TEST(ExpressionEvaluator, SimpleTrueExpressionReturnsTrue) {
    ASSERT_TRUE(ExpressionEvaluator::Evaluate("1", predicate));
}

TEST(ExpressionEvaluator, SimpleFalseExpressionReturnsFalse) {
    ASSERT_FALSE(ExpressionEvaluator::Evaluate("0", predicate));
}

TEST(ExpressionEvaluator, TrueAndTrueReturnsTrue) {
    ASSERT_TRUE(ExpressionEvaluator::Evaluate("1 && 1", predicate));
}

TEST(ExpressionEvaluator, FalseAndFalseReturnsFalse) {
    ASSERT_FALSE(ExpressionEvaluator::Evaluate("0 && 0", predicate));
}

TEST(ExpressionEvaluator, TrueAndFalseReturnsFalse) {
    ASSERT_FALSE(ExpressionEvaluator::Evaluate("1 && 0", predicate));
}

TEST(ExpressionEvaluator, FalseAndTrueReturnsFalse) {
    ASSERT_FALSE(ExpressionEvaluator::Evaluate("0 && 1", predicate));
}

TEST(ExpressionEvaluator, TrueOrTrueReturnsTrue) {
    ASSERT_TRUE(ExpressionEvaluator::Evaluate("1 || 1", predicate));
}

TEST(ExpressionEvaluator, FalseOrFalseReturnsFalse) {
    ASSERT_FALSE(ExpressionEvaluator::Evaluate("0 || 0", predicate));
}

TEST(ExpressionEvaluator, TrueOrFalseReturnsTrue) {
    ASSERT_TRUE(ExpressionEvaluator::Evaluate("1 || 0", predicate));
}

TEST(ExpressionEvaluator, FalseOrTrueReturnsTrue) {
    ASSERT_TRUE(ExpressionEvaluator::Evaluate("0 || 1", predicate));
}

TEST(ExpressionEvaluator, ParenTestOne) {
    ASSERT_TRUE(ExpressionEvaluator::Evaluate("0 || (1 && 1)", predicate));
}

TEST(ExpressionEvaluator, ParenTestTwo) {
    ASSERT_TRUE(ExpressionEvaluator::Evaluate("(1 || 0) && 1", predicate));
}

TEST(ExpressionEvaluator, ParenTestThree) {
    ASSERT_TRUE(ExpressionEvaluator::Evaluate("(1 || 0) && ( 0 || 1)", predicate));
}

TEST(ExpressionEvaluator, NestedParens) {
    ASSERT_TRUE(ExpressionEvaluator::Evaluate("1 && (0 || (1 && 1))", predicate));
}

TEST(ExpressionEvaluator, NoSpacesWorksFine) {
    ASSERT_TRUE(ExpressionEvaluator::Evaluate("1||0", predicate));
}

TEST(ExpressionEvaluator, ExtraSpacesWorkFine) {
    ASSERT_TRUE(ExpressionEvaluator::Evaluate("    0   ||   1", predicate));
}

TEST(ExpressionEvaluator, MultipleLogicalOperatorsAreEvaluatedLeftToRight) {
    ASSERT_TRUE(ExpressionEvaluator::Evaluate("1 || 0 && 1", predicate)); // equivalent to (0 || 1) && 1
}