#pragma once

#include <string>
#include <string_view>
#include <charconv>
#include <regex>

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
