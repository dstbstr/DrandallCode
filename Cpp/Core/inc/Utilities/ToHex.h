#pragma once

#include <string>

static constexpr char HexDigitsUpper[] = "0123456789ABCDEF";
static constexpr char HexDigitsLower[] = "0123456789abcdef";

constexpr std::string ToHexUpper(char input) {
    std::string result(2, '0');
    for(size_t i = 0, j = 4; i < 2; i++, j-=4) {
        result[i] = HexDigitsUpper[(input >> j) & 0x0F];
    }

    return result;
}

constexpr std::string ToHexLower(char input) {
    std::string result(2, '0');
    for (size_t i = 0, j = 4; i < 2; i++, j -= 4) {
        result[i] = HexDigitsLower[(input >> j) & 0x0F];
    }

    return result;
}

static_assert(ToHexUpper(char(42)) == "2A");
static_assert(ToHexLower(char(42)) == "2a");

constexpr std::string ToHexUpper(short input) {
    std::string result(4, '0');
    for(size_t i = 0, j = 12; i < 4; i++, j-=4) {
        result[i] = HexDigitsUpper[(input >> j)& 0x0F];
    }

    return result;
}

constexpr std::string ToHexLower(short input) {
    std::string result(4, '0');
    for (size_t i = 0, j = 12; i < 4; i++, j -= 4) {
        result[i] = HexDigitsLower[(input >> j) & 0x0F];
    }

    return result;
}

static_assert(ToHexUpper(short(42)) == "002A");
static_assert(ToHexLower(short(42)) == "002a");

constexpr std::string ToHexUpper(int input) {
    std::string result(8, '0');
    for(size_t i = 0, j = 28; i < 8; i++, j-=4) {
        result[i] = HexDigitsUpper[(input >> j) & 0x0F];
    }

    return result;
}

constexpr std::string ToHexLower(int input) {
    std::string result(8, '0');
    for (size_t i = 0, j = 28; i < 8; i++, j -= 4) {
        result[i] = HexDigitsLower[(input >> j) & 0x0F];
    }

    return result;
}

static_assert(ToHexUpper(42) == "0000002A");
static_assert(ToHexLower(42) == "0000002a");