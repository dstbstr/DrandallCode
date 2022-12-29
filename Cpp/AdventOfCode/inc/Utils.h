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
    for (auto i = 0; i < 20; i++) {
        buf[i] = 0;
    }
    std::to_chars(buf.get(), buf.get() + 20, input);
    return std::string(buf.get());
}

template<>
std::string ToString(std::string input) {
    return input;
}

template<>
std::string ToString(std::string_view input) {
    return std::string(input);
}

u32 Sum(const std::vector<u32>& vec) {
    return std::reduce(std::execution::par, vec.cbegin(), vec.cend(), 0, [](s32 lhs, s32 rhs) { return lhs + rhs;});
}
