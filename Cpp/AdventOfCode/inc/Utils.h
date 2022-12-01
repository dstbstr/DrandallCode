#pragma once

#include <string>
#include <string_view>
#include <charconv>
#include <regex>
#include <algorithm>
#include <numeric>
#include <execution>


bool IsNumber(std::string val) {
    static auto IsNumberRegex = std::regex("-?\\d+");
    return std::regex_match(val, IsNumberRegex);
}

template<typename T>
bool ParseNumber(std::string_view number, T& result) {
    auto returnCode = std::from_chars(number.data(), number.data() + number.size(), result);
    return returnCode.ec == std::errc();
}

template<typename T>
std::string ToString(T input) {
    auto buf = std::unique_ptr<char[]>(new char[20]);
    std::to_chars(buf.get(), buf.get() + 20, input);
    return std::string(buf);
}

constexpr u32 SumConstexpr(const std::vector<u32>& vec) {
    u32 result = 0;
    for(auto e : vec) {
        result += e;
    }

    return result;
}

u32 Sum(const std::vector<u32>& vec) {
    return std::reduce(std::execution::par, vec.cbegin(), vec.cend(), 0, [](s32 lhs, s32 rhs) { return lhs + rhs;});
}

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

constexpr std::vector<u32> GetDivisors(u32 input) {
    auto last = static_cast<u32>(Sqrt(input));
    auto result = std::vector<u32>{};

    for(u32 i = 1; i < last; i++) {
        if(input % i == 0) {
            result.push_back(i);
            result.push_back(input / i);
        }
    }

    if(last * last == input) {
        result.push_back(last);
    } else if(input % last == 0) {
        result.push_back(last);
        result.push_back(input / last);
    }

    return result;
}

static_assert(GetDivisors(24) == std::vector<u32>{1, 24, 2, 12, 3, 8, 4, 6});
static_assert(GetDivisors(9) == std::vector<u32>{1, 9, 3});
static_assert(GetDivisors(1) == std::vector<u32>{1});
