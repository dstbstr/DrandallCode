#pragma once

#include "Common.h"

constexpr size_t FindFloor(const std::string_view input) {
    return std::count(input.cbegin(), input.cend(), '(') - std::count(input.cbegin(), input.cend(), ')');
}

static_assert(FindFloor("") == 0);
static_assert(FindFloor("(())") == 0);
static_assert(FindFloor("()()") == 0);
static_assert(FindFloor("(((") == 3);
static_assert(FindFloor("(()(()(") == 3);
static_assert(FindFloor("())") == -1);
static_assert(FindFloor("))(") == -1);
static_assert(FindFloor(")))") == -3);
static_assert(FindFloor(")())())") == -3);

constexpr size_t FindBasementStep(const std::string_view input) {
    s32 currentFloor = 0;
    for (size_t step = 0; step < input.size(); step++) {
        currentFloor += (input[step] == '(' ? 1 : -1);
        if (currentFloor < 0) return step + 1;
    }
    return 0;
}

static_assert(FindBasementStep(")") == 1);
static_assert(FindBasementStep("()())") == 5);

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(FindFloor(lines[0]));
    return ToString(FindBasementStep(lines[0]));
}

bool RunTests() {
    return true;
}