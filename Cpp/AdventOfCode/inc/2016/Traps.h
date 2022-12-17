#pragma once

#include "Common.h"

constexpr bool IsTrap(bool left, bool center, bool right) {
    return 
        (left && center && !right) ||
        (center && right && !left) ||
        (left && !center && !right) ||
        (!left && !center && right);
}
static_assert(IsTrap(true, true, false));
static_assert(!IsTrap(true, true, true));
static_assert(!IsTrap(false, false, false));

constexpr std::vector<bool> GetNextRow(const std::vector<bool>& currentRow) {
    std::vector<bool> result;
    result.resize(currentRow.size());

    result[0] = IsTrap(false, currentRow[0], currentRow[1]);
    for (size_t i = 1; i < currentRow.size() - 1; i++) {
        result[i] = IsTrap(currentRow[i - 1], currentRow[i], currentRow[i + 1]);
    }
    result[result.size() - 1] = IsTrap(currentRow[result.size() - 2], currentRow[result.size() - 1], false);

    return result;
}

static_assert(GetNextRow({ false, false, true, true, false }) == std::vector<bool>{false, true, true, true, true});

u32 CountSafeTiles(const std::string& initial, u32 rowCount) {
    std::vector<std::vector<bool>> rows{};
    rows.push_back({});
    for (auto c : initial) {
        rows[0].push_back(c == '^');
    }

    for (size_t i = 1; i < rowCount; i++) {
        rows.push_back(GetNextRow(rows[i -1]));
    }

    u32 safeCount = 0;
    for (auto row : rows) {
        for (bool trap : row) {
            if (!trap) safeCount++;
        }
    }

    return safeCount;
}

std::string Run(const std::vector<std::string>&) {
    //return ToString(CountSafeTiles(".^^^.^.^^^^^..^^^..^..^..^^..^.^.^.^^.^^....^.^...^.^^.^^.^^..^^..^.^..^^^.^^...^...^^....^^.^^^^^^^", 40));
    return ToString(CountSafeTiles(".^^^.^.^^^^^..^^^..^..^..^^..^.^.^.^^.^^....^.^...^.^^.^^.^^..^^..^.^..^^^.^^...^...^^....^^.^^^^^^^", 400000));
}

bool RunTests() {
    std::string initial = ".^^.^.^^^^";
    if (CountSafeTiles(initial, 10) != 38) return false;

    return true;
}