#pragma once

#include "Common.h"

//Enter row 2981, col 3075
//1:1 = 20151125
//2:1 = 20151125 * 252533 % 33554393 = 31916031


// 1  3  6 10 15 21 28 36
// 2  5  9 14 20 27 35 44
// 4  8 13 19 26 34 43 53
// 7 12 18 25 33 42 52 63

constexpr u64 CalculateIndex(u32 row, u32 col) {
    u64 topRow = row + col - 1;
    topRow = (topRow * (topRow + 1)) / 2;
    return topRow - row + 1;
}

static_assert(CalculateIndex(1, 1) == 1);
static_assert(CalculateIndex(1, 2) == 3);
static_assert(CalculateIndex(2, 1) == 2);
static_assert(CalculateIndex(2, 2) == 5);
static_assert(CalculateIndex(4, 7) == 52);

constexpr u64 CalculateCode(u64 index) {
    u64 running = 20151125;
    for(int i = 1; i < index; i++) {
        running *= 252533;
        running %= 33554393;
    }

    return running;
}

static_assert(CalculateCode(1) == 20151125);
static_assert(CalculateCode(2) == 31916031);
static_assert(CalculateCode(3) == 18749137);

constexpr u64 Run() {
    return CalculateCode(CalculateIndex(2981, 3075));
}