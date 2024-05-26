#include "TestCommon.h"

#include "Core/Macros/PreProcessorOverride.h"

#define _MY_MACRO_1(x) 1
#define _MY_MACRO_2(x, y) 2
#define _MY_MACRO_3(x, y, z) 3
#define MY_MACRO(...) MACRO_OVERRIDE(_MY_MACRO_, __VA_ARGS__)

TEST(PreProcessorOverride, CallsTheCorrectMacroOverrideByArgNumber) {
    ASSERT_EQ(1, MY_MACRO(a));
    ASSERT_EQ(2, MY_MACRO(a, b));
    ASSERT_EQ(3, MY_MACRO(a, b, c));
}

#define CHAIN_MACRO_1(x, ...) x
#define CHAIN_MACRO_2(x, ...) x, EXPAND(CHAIN_MACRO_1(__VA_ARGS__))
#define CHAIN_MACRO_3(x, ...) x, EXPAND(CHAIN_MACRO_2(__VA_ARGS__))
#define CHAIN_MACRO_4(x, ...) x, EXPAND(CHAIN_MACRO_3(__VA_ARGS__))
#define CHAIN_MACRO_5(x, ...) x, EXPAND(CHAIN_MACRO_4(__VA_ARGS__))
#define CHAIN_MACRO(...) MACRO_OVERRIDE(CHAIN_MACRO_, __VA_ARGS__)

TEST(PreProcessorOverride, ChainMacroWithSingleElement) {
    std::vector<int> ints{CHAIN_MACRO(1)};

    ASSERT_EQ(1, ints.size());
    ASSERT_EQ(1, ints[0]);
}

TEST(PreProcessorOverride, ChainMacroWithTwoElements) {
    std::vector<int> ints{CHAIN_MACRO(5, 3)};

    ASSERT_EQ(2, ints.size());
    ASSERT_EQ(5, ints[0]);
    ASSERT_EQ(3, ints[1]);
}

TEST(PreProcessorOverride, ChainMacroWithMoreThanTwoElements) {
    std::vector<int> ints{CHAIN_MACRO(2, 3, 5, 7, 11)};

    ASSERT_EQ(5, ints.size());
    ASSERT_EQ(11, ints[4]);
}
