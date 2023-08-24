#include "2017/d1_Captcha.h"

SOLUTION(2017, 1) {
    constexpr u32 SumDigits(std::string_view line, size_t offset) {
        u32 result = 0;
        for (auto i = 0; i < line.size(); i++) {
            if (line[i] == line[(i + offset) % line.size()]) {
                result += line[i] - '0';
            }
        }
        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(SumDigits(Line, 1));
    }

    PART_TWO() {
        return Constexpr::ToString(SumDigits(Line, Line.size() / 2));
    }

    TESTS() {
        static_assert(SumDigits("1122", 1) == 3);
        static_assert(SumDigits("1111", 1) == 4);
        static_assert(SumDigits("1234", 1) == 0);
        static_assert(SumDigits("91212129", 1) == 9);
        static_assert(SumDigits("1212", 2) == 6);
        static_assert(SumDigits("1221", 2) == 0);
        static_assert(SumDigits("123425", 3) == 4);

        return true;
    }
}
