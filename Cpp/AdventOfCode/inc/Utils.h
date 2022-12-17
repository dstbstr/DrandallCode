#pragma once

#include <string>
#include <string_view>
#include <charconv>
#include <regex>
#include <algorithm>
#include <numeric>
#include <execution>

#include "Utilities/ConstexprUtils.h"

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
    for (auto i = 0; i < 20; i++) {
        buf[i] = 0;
    }
    std::to_chars(buf.get(), buf.get() + 20, input);
    return std::string(buf.get());
}

u32 Sum(const std::vector<u32>& vec) {
    return std::reduce(std::execution::par, vec.cbegin(), vec.cend(), 0, [](s32 lhs, s32 rhs) { return lhs + rhs;});
}

constexpr std::vector<u32> GetDivisors(u32 input) {
    auto last = static_cast<u32>(Constexpr::Sqrt(input));
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

template<size_t Rows, size_t Cols>
constexpr size_t GetIndex(size_t row, size_t col) {
    return row * Cols + col;
}

static_assert(GetIndex<3, 3>(0, 0) == 0);
static_assert(GetIndex<3, 3>(2, 2) == 8);

template<size_t Rows, size_t Cols>
constexpr void GetRowCol(size_t index, size_t& row, size_t& col) {
    row = index / Cols;
    col = index % Cols;
}
