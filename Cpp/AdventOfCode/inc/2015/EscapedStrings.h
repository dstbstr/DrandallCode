#pragma once

#include "Platform/Types.h"

#include <string_view>

constexpr u32 CountCodeCharacters(std::string_view input) {
    return static_cast<u32>(input.size());
}

static_assert(CountCodeCharacters(R"("")") == 2);
static_assert(CountCodeCharacters(R"("abc")") == 5);
static_assert(CountCodeCharacters(R"("aaa\"aaa")") == 10);
static_assert(CountCodeCharacters(R"("\x27")") == 6);

constexpr u32 CountMemoryCharacters(std::string_view input) {
    u32 result = 0;
    bool isEscaping = false;
    for(const auto& c : input) {
        if(isEscaping) {
            switch(c) {
                case '\\' : result++; break;
                case '"' : result++; break;
                case 'x' : result--; break; // \x27 is 4 code characters, we skipped the \ and the next iteration will add the '27'
            }
            isEscaping = false;
        } else {
            if(c == '\\') {
                isEscaping = true;
            } else {
                result++;
            }
        }
    }
    return result -2; //strip off the quotes
}

static_assert(CountMemoryCharacters(R"("")") == 0);
static_assert(CountMemoryCharacters(R"("abc")") == 3);
static_assert(CountMemoryCharacters(R"("aaa\"aaa")") == 7);
static_assert(CountMemoryCharacters(R"("\x27")") == 1);

constexpr u32 CalculateDelta(std::string_view input) {
    return CountCodeCharacters(input) - CountMemoryCharacters(input);
}

static_assert(CalculateDelta(R"("")") == 2);
static_assert(CalculateDelta(R"("abc")") == 2);
static_assert(CalculateDelta(R"("aaa\"aaa")") == 3);
static_assert(CalculateDelta(R"("\x27")") == 5);

constexpr u32 CountReencodedCharacters(std::string_view input) {
    u32 result = static_cast<u32>(input.size() + 2); //2 unescaped quotes
    for(const auto& c : input) {
        if(c == '\\' || c == '"') {
            result++;
        }
    }

    return result;
}

static_assert(CountReencodedCharacters(R"("")") == 6);
static_assert(CountReencodedCharacters(R"("abc")") == 9);
static_assert(CountReencodedCharacters(R"("aaa\"aaa")") == 16);
static_assert(CountReencodedCharacters(R"("\x27")") == 11);

constexpr u32 CalculateGrowth(std::string_view input) {
    return CountReencodedCharacters(input) - CountCodeCharacters(input);
}

static_assert(CalculateGrowth(R"("")") == 4);
static_assert(CalculateGrowth(R"("abc")") == 4);
static_assert(CalculateGrowth(R"("aaa\"aaa")") == 6);
static_assert(CalculateGrowth(R"("\x27")") == 5);
