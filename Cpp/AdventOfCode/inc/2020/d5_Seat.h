#pragma once

#include "Common.h"

constexpr RowCol FindSeat(const std::string& pass) {
    //F = Lower
    //B = Upper
    //L = Lower
    //R = Upper
    size_t minRow = 0;
    size_t maxRow = 127;
    size_t minCol = 0;
    size_t maxCol = 7;

    for (auto c : pass) {
        switch (c) {
        case 'F': maxRow = minRow + ((maxRow - minRow) / 2); break;
        case 'B': minRow = maxRow - ((maxRow - minRow) / 2); break;
        case 'L': maxCol = minCol + ((maxCol - minCol) / 2); break;
        case 'R': minCol = maxCol - ((maxCol - minCol) / 2); break;
        }
    }

    return { minRow, minCol };
}

static_assert(FindSeat("FBFBBFFRLR") == RowCol{ 44, 5 });
static_assert(FindSeat("FFFBBBFRRR") == RowCol{ 14, 7 });
static_assert(FindSeat("BFFFBBFRRR") == RowCol{ 70, 7 });
static_assert(FindSeat("BBFFBBFRLL") == RowCol{ 102, 4 });
static_assert(FindSeat("FFFFFFFLLL") == RowCol{ 0, 0 });
static_assert(FindSeat("BBBBBBBRRR") == RowCol{ 127, 7 });

constexpr size_t GetId(RowCol seat) {
    return (seat.Row * 8) + seat.Col;
}

static_assert(GetId({ 44, 5 }) == 357);

auto PartOne(const std::vector<std::string>& lines) {
    size_t max = 0;
    for (const auto& line : lines) {
        max = std::max(max, GetId(FindSeat(line)));
    }

    return max;
}

auto PartTwo(const std::vector<std::string>& lines) {
    std::vector<size_t> seatIds;
    for (const auto& line : lines) {
        seatIds.push_back(GetId(FindSeat(line)));
    }
    std::sort(seatIds.begin(), seatIds.end());
    for (size_t i = 0; i < seatIds.size() - 1; i++) {
        if (seatIds[i] + 1 != seatIds[i + 1]) {
            return seatIds[i] + 1;
        }
    }
    
    return 0ull;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {

    };

    if (PartOne(lines) != 0) return false;
    return true;
}