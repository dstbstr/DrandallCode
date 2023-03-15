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


u32 Sum(const std::vector<u32>& vec) {
    return std::reduce(std::execution::par, vec.cbegin(), vec.cend(), 0, [](s32 lhs, s32 rhs) { return lhs + rhs;});
}
