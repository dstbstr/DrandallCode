#pragma once
#include "2015/d10_SeeAndSay.h"

SOLUTION(2015, 10) {
    constexpr void AppendNext(char lastCharacter, int count, std::string & result) {
        switch (count) {
        case 1: result += "1"; break;
        case 2: result += "2"; break;
        case 3: result += "3"; break;
        }
        result += lastCharacter;
    }

    constexpr std::string Next(const std::string & input) {
        std::string result;
        char lastCharacter = input[0];
        int count = 0;
        for (const auto& c : input) {
            if (c == lastCharacter) {
                count++;
            }
            else {
                AppendNext(lastCharacter, count, result);
                lastCharacter = c;
                count = 1;
            }
        }
        AppendNext(lastCharacter, count, result);

        return result;
    }

    constexpr std::string ApplyTimes(std::string_view input, size_t times) {
        std::string running = std::string(input);

        for (auto i = 0; i < times; i++) {
            running = Next(running);
        }

        return running;
    }


    PART_ONE() {
        return Constexpr::ToString(ApplyTimes(Line, 40).size());
    }
    PART_TWO() {
        return Constexpr::ToString(ApplyTimes(Line, 50).size());
    }
    TESTS() {
        static_assert(Next("1") == "11");
        static_assert(Next("11") == "21");
        static_assert(Next("21") == "1211");
        static_assert(Next("1211") == "111221");
        static_assert(Next("111221") == "312211");
        //static_assert(ApplyTimes("1113222113", 40).size() == 252594);

        return true;
    }
}