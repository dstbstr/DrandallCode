#pragma once

#include <string_view>
#include <bitset>
#include <array>
#include <memory>
#include <unordered_set>
#include <algorithm>
#include <charconv>

#include "Platform/Types.h"

static constexpr size_t GridSize = 1000;
static constexpr size_t BitsSize = GridSize * GridSize;

constexpr size_t GetPos(u32 x, u32 y)
{
    return (x * GridSize) + y;
}

constexpr void Move(char c, u32& x, u32& y) {
    switch(c) {
        case '<' : x--; break;
        case '>' : x++; break;
        case '^' : y++; break;
        case 'v' : y--; break;
    }
}

constexpr u32 UpdateSeen(u32 x, u32 y, u32& seenIndex, std::array<size_t, 8000>& seen) {
    auto coord = GetPos(x, y);
    if(std::find(seen.begin(), seen.end(), coord) == seen.end()) {
        seen[seenIndex++] = coord;
        return 1;
    }

    return 0;
}

constexpr u32 CountDeliveries(std::string_view input) {
    auto seen = std::array<size_t, 8000>{};
    u32 seenIndex = 0;
    u32 x = GridSize / 2;
    u32 y = GridSize / 2;
    u32 result = 1;

    seen[seenIndex++] = GetPos(x, y);

    for(const auto c : input) {
        Move(c, x, y);

        result += UpdateSeen(x, y, seenIndex, seen);
    }

    return result;
}

static_assert(CountDeliveries(">") == 2);
static_assert(CountDeliveries("^>v<") == 4);


constexpr u32 CountDeliveriesWithRobot(std::string_view input) {
    auto seen = std::array<size_t, 8000>{};
    u32 seenIndex = 0;
    u32 santaX = GridSize / 2;
    u32 santaY = GridSize / 2;
    u32 botX = santaX;
    u32 botY = santaY;
    u32 result = 1;

    seen[seenIndex++] = GetPos(santaX, santaY);

    for(auto i = 0; i < input.size(); i+= 2)
    {
        Move(input[i], santaX, santaY);
        result += UpdateSeen(santaX, santaY, seenIndex, seen);

        Move(input[i+1], botX, botY);
        result += UpdateSeen(botX, botY, seenIndex, seen);
    }
    
    return result;
}

static_assert(CountDeliveriesWithRobot("^v") == 3);
static_assert(CountDeliveriesWithRobot("^>v<") == 3);
static_assert(CountDeliveriesWithRobot("^v^v^v^v^v") == 11);