#pragma once

#include "2015/d25_SecretCode.h"

SOLUTION(2015, 25) {
    //1:1 = 20151125
    //2:1 = 20151125 * 252533 % 33554393 = 31916031

    // 1  3  6 10 15 21 28 36
    // 2  5  9 14 20 27 35 44
    // 4  8 13 19 26 34 43 53
    // 7 12 18 25 33 42 52 63

    constexpr size_t CalculateIndex(size_t row, size_t col) {
        size_t topRow = row + col - 1;
        topRow = (topRow * (topRow + 1)) / 2;
        return topRow - row + 1;
    }

    constexpr size_t CalculateCode(size_t index) {
        size_t running = 20151125;
        for (int i = 1; i < index; i++) {
            running *= 252533;
            running %= 33554393;
        }

        return running;
    }

    PART_ONE() {
        //To continue, please consult the code grid in the manual.  Enter the code at row 2981, column 3075.
        const auto numbers = "0123456789";
        const auto line = lines[0];
        auto firstDigitStart = line.find_first_of(numbers);
        auto firstDigitEnd = line.find_first_not_of(numbers, firstDigitStart);
        auto secondDigitStart = line.find_first_of(numbers, firstDigitEnd);
        auto secondDigitEnd = line.find_first_not_of(numbers, secondDigitStart);

        size_t row, col;
        Constexpr::ParseNumber(line.substr(firstDigitStart, firstDigitEnd - firstDigitStart), row);
        Constexpr::ParseNumber(line.substr(secondDigitStart, secondDigitEnd - secondDigitStart), col);

        return Constexpr::ToString(CalculateCode(CalculateIndex(row, col)));
    }

    PART_TWO() {
        (void)lines;
        return "Merry Christmas";
    }

    TESTS() {
        static_assert(CalculateIndex(1, 1) == 1);
        static_assert(CalculateIndex(1, 2) == 3);
        static_assert(CalculateIndex(2, 1) == 2);
        static_assert(CalculateIndex(2, 2) == 5);
        static_assert(CalculateIndex(4, 7) == 52);

        static_assert(CalculateCode(1) == 20151125);
        static_assert(CalculateCode(2) == 31916031);
        static_assert(CalculateCode(3) == 18749137);

        return true;
    }
}