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

    template<typename Collection>
    constexpr std::string JoinVec(std::string&& delimiter, Collection&& input) {
        std::string result;
        bool first = true;
        for (const auto& elem : input) {
            if (first) first = false;
            else result += delimiter;
            result += elem;
        }

        return result;
    }

    template<typename Collection>
    constexpr std::string JoinVec(std::string&& delimiter, const Collection& input) {
        std::string result;
        bool first = true;
        for (const auto& elem : input) {
            if (first) first = false;
            else result += delimiter;
            result += elem;
        }

        return result;
    }

    template<typename Collection>
    constexpr std::string JoinVec(char&& delimiter, Collection const& input) {
        return JoinVec(std::string(1, delimiter), input);
    }

    static_assert(JoinVec(" ", std::vector<std::string>{ "a", "b", "c" }) == "a b c");
    static_assert(JoinVec(' ', std::vector<std::string>{ "a", "b", "c" }) == "a b c");
    static_assert(JoinVec("Hello", std::vector<std::string>{ "a", "b", "c" }) == "aHellobHelloc");
    /*


    template<typename Collection>
    std::string JoinVec(char&& delimiter, Collection const& input) {
        return JoinVec(std::string(1, delimiter), input);
    }

    */
}