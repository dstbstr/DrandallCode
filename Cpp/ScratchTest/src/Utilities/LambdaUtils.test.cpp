#include "Utilities/LambdaUtils.h"

#include "TestCommon.h"

#include <algorithm>
#include <sstream>

template<typename T, class... Input>
void Fun(T Callback, Input&&... inputs) {
    LambdaUtils::ForEach(Callback, inputs...);
}

TEST(ForEachUtil, CallsCallBackInOrder) {
    std::stringstream stream;

    Fun([&](auto it) { stream << it; }, 1, 2, 3, 4, 5);

    ASSERT_EQ(stream.str(), "12345");
}

TEST(ForEachUtil, WorksWithNoInputs) {
    Fun([](auto it) { ASSERT_FALSE(true); });
}

TEST(ForEachUtil, WorksAsTransform) {
    int one = 1;
    int two = 2;
    int three = 3;
    Fun([](auto& it) { it *= 2; }, one, two, three);

    ASSERT_EQ(2, one);
    ASSERT_EQ(4, two);
    ASSERT_EQ(6, three);
}
