#pragma once

#include <string>

static constexpr char HexDigits[] = "0123456789ABCDEF";

constexpr std::string ToHex(char input) {
    std::string result(2, '0');
    for(size_t i = 0, j = 4; i < 2; i++, j-=4) {
        result[i] = HexDigits[(input >> j) & 0x0F];
    }

    return result;
}

static_assert(ToHex(char(42)) == "2A");

constexpr std::string ToHex(short input) {
    std::string result(4, '0');
    for(size_t i = 0, j = 12; i < 4; i++, j-=4) {
        result[i] = HexDigits[(input >> j)& 0x0F];
    }

    return result;
}

static_assert(ToHex(short(42)) == "002A");

constexpr std::string ToHex(int input) {
    std::string result(8, '0');
    for(size_t i = 0, j = 28; i < 8; i++, j-=4) {
        result[i] = HexDigits[(input >> j) & 0x0F];
    }

    return result;
}

static_assert(ToHex(42) == "0000002A");