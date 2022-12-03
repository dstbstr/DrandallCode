#pragma once

#include <vector>
#include <array>

namespace Constexpr {
    template<typename T>
    constexpr auto Sum(const T& container) {
        auto result = 0;
        for(auto e : container) {
            result += e;
        }

        return result;
    }

    static_assert(Sum(std::vector{1,2,3}) == 6);
    static_assert(Sum(std::array{-1, -2, -3}) == -6);

    template<typename T>
    constexpr T Abs(T t) {
        return t < 0 ? -t : t;
    }

    static_assert(Abs(3) == 3);
    static_assert(Abs(-3) == 3);

    namespace Detail {
        constexpr double SqrtImpl(double x, double curr, double prev) {
            return curr == prev ? curr : SqrtImpl(x, 0.5 * (curr + x / curr), curr);
        }
    }
    constexpr double Sqrt(double x) {
        return x >= 0 && x < std::numeric_limits<double>::infinity() 
        ? Detail::SqrtImpl(x, x, 0)
        : std::numeric_limits<double>::quiet_NaN();
    }

    static_assert(Sqrt(9) == 3.0);
    static_assert(Sqrt(1) == 1.0);
    static_assert(Sqrt(-1) != Sqrt(-1)); // no way to test for NaN, but NaN != NaN
}