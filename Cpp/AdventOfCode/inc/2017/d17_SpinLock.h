#pragma once

#include "Common.h"
constexpr size_t FindInsertIndex(size_t bufferSize, size_t currentPos, size_t stepSize) {
    return ((currentPos + stepSize) % bufferSize) + 1;
}

static_assert(FindInsertIndex(1, 0, 3) == 1);
static_assert(FindInsertIndex(2, 1, 3) == 1);
static_assert(FindInsertIndex(3, 1, 3) == 2);
static_assert(FindInsertIndex(4, 2, 3) == 2);
static_assert(FindInsertIndex(5, 2, 3) == 1);
static_assert(FindInsertIndex(6, 1, 3) == 5);
static_assert(FindInsertIndex(7, 5, 3) == 2);
static_assert(FindInsertIndex(8, 2, 3) == 6);
static_assert(FindInsertIndex(9, 6, 3) == 1);

auto PartOne(const size_t stepSize) {
    std::vector<size_t> buffer{ 0 };
    size_t pos = 0;
    for (auto i = 1; i < 2018; i++) {
        auto insertPos = FindInsertIndex(buffer.size(), pos, stepSize);
        buffer.insert(buffer.begin() + insertPos, i);
        pos = insertPos;
    }

    return buffer[pos + 1];
}

auto PartTwo(const size_t stepSize) {
    size_t pos = 0;
    size_t bufferSize = 1;
    size_t val1 = 0;

    for (auto i = 1; i < 50'000'001; i++) {
        pos = FindInsertIndex(bufferSize, pos, stepSize);
        if (pos == 1) {
            val1 = i;
        }
        bufferSize++;
    }
    return val1;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(343));
    return ToString(PartTwo(343));
}

bool RunTests() {
    if (PartOne(3) != 638) return false;
    return true;
}