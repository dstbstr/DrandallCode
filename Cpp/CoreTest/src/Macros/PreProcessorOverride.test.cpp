#include "Macros/PreProcessorOverride.h"

#include "TestCommon.h"


#define _MY_MACRO_1(x) 1
#define _MY_MACRO_2(x, y) 2
#define _MY_MACRO_3(x, y, z) 3
#define MY_MACRO(...) MACRO_OVERRIDE(_MY_MACRO_, __VA_ARGS__)

TEST(PreProcessorOverride, CallsTheCorrectMacroOverrideByArgNumber) {
    ASSERT_EQ(1, MY_MACRO(a));
    ASSERT_EQ(2, MY_MACRO(a, b));
    ASSERT_EQ(3, MY_MACRO(a, b, c));
}
