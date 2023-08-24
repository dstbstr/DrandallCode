#include "2016/d9_Decompress.h"

SOLUTION(2016, 9) {
    constexpr u64 Decompress(std::string_view line, bool recurse) {
        u64 result = 0;
        u32 runLength;
        u32 repeatCount;
        size_t index = 0;
        while (index < line.size()) {
            if (line[index] != '(') {
                result++;
                index++;
            }
            else {
                auto close = line.find(')', index);
                auto expr = line.substr(index + 1, close - index - 1);
                auto s = Constexpr::Split(expr, "x");
                Constexpr::ParseNumber(s[0], runLength);
                Constexpr::ParseNumber(s[1], repeatCount);
                if (recurse) {
                    result += repeatCount * Decompress(line.substr(close + 1, runLength), recurse);
                }
                else {
                    result += repeatCount * runLength;
                }
                index = close + runLength + 1;
            }
        }

        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(Decompress(Line, false));
    }

    PART_TWO() {
        return Constexpr::ToString(Decompress(Line, true));
    }

    TESTS() {
        if (Decompress("(3x3)XYZ", true) != 9) return false;
        if (Decompress("X(8x2)(3x3)ABCY", true) != 20) return false;
        if (Decompress("(27x12)(20x12)(13x14)(7x10)(1x12)A", true) != 241920) return false;
        if (Decompress("(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN", true) != 445) return false;
        return true;
    }
}