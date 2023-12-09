#pragma once

#include "2015/d23_TuringLock.h"

SOLUTION(2015, 23) {
    //wow, this works, and is faster
    constexpr auto GenerateFunc(std::string_view line) {
        auto s = Constexpr::Split(line, " ");
        std::vector<std::string> parts;
        std::transform(s.cbegin(), s.cend(), std::back_inserter(parts), [](auto sv) { return std::string(sv); });

        bool isRegA = s[1][0] == 'a';
        return [parts, isRegA](size_t& ip, size_t& a, size_t& b) {
            s32 offset;
            if (parts[0] == "hlf") isRegA ? a /= 2 : b /= 2;
            else if (parts[0] == "tpl") isRegA ? a *= 3 : b *= 3;
            else if (parts[0] == "inc") isRegA ? a++ : b++;
            else if (parts[0] == "jmp") {
                Constexpr::ParseNumber(parts[1], offset);
                ip += offset;
                return;
            }
            else if (parts[0] == "jie") {
                if (isRegA ? a % 2 == 0 : b % 2 == 0) {
                    Constexpr::ParseNumber(parts[2], offset);
                    ip += offset;
                    return;
                }
            }
            else if (parts[0] == "jio") {
                if (isRegA ? a == 1 : b == 1) {
                    Constexpr::ParseNumber(parts[2], offset);
                    ip += offset;
                    return;
                }
            }
            ip++;
        };
    }

    constexpr size_t Solve(const auto& lines, size_t initialAValue) {
        auto instructions = ParseLines(lines, GenerateFunc);

        size_t a = initialAValue;
        size_t b = 0;
        size_t ip = 0;
        while (ip < instructions.size()) {
            instructions[ip](ip, a, b);
        }

        return b;
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(lines, 0));
    }
    PART_TWO() {
        return Constexpr::ToString(Solve(lines, 1));
    }
    TESTS() {
        return true;
    }
}