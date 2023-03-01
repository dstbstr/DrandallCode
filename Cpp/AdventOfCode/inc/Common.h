#pragma once

#include <string>
#include <regex>
#include <utility>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <exception>
#include <functional>
#include <iostream>

#include "Platform/Types.h"
#include "Utils.h"
#include "Coord.h"
#include "Utilities/StringUtils.h"
#include "Utilities/Format.h"

#include "Constexpr/ConstexprBits.h"
#include "Constexpr/ConstexprCollections.h"
#include "Constexpr/ConstexprMath.h"
#include "Constexpr/ConstexprStrUtils.h"

using namespace std::string_view_literals;
using namespace std::string_literals;

enum Direction { Up, Right, Down, Left };

constexpr Direction TurnRight(Direction current) {
    return static_cast<Direction>((current + 1) % 4);
}
constexpr Direction TurnLeft(Direction current) {
    return static_cast<Direction>((4 + (current - 1)) % 4);
}
constexpr Direction TurnAround(Direction current) {
    return static_cast<Direction>((current + 2) % 4);
}
static_assert(TurnRight(Up) == Right);
static_assert(TurnLeft(Up) == Left);
static_assert(TurnRight(Left) == Up);
static_assert(TurnLeft(Left) == Down);
static_assert(TurnAround(Up) == Down);
static_assert(TurnAround(Down) == Up);
static_assert(TurnAround(Left) == Right);
static_assert(TurnAround(Right) == Left);

constexpr void Move(Coord& pos, Direction facing) {
    switch (facing) {
    case Up: pos.Y--; return;
    case Down: pos.Y++; return;
    case Left: pos.X--; return;
    case Right: pos.X++; return;
    default:break;
    }
}

constexpr void Move(UCoord& pos, Direction facing) {
    switch (facing) {
    case Up: pos.Y--; return;
    case Down: pos.Y++; return;
    case Left: pos.X--; return;
    case Right: pos.X++; return;
    default:break;
    }
}

template<typename Key, typename Value, typename Hash = std::hash<Key>>
void PrintMap(const std::unordered_map<Key, Value, Hash>& map, std::function<char(const Key&, const Value&)> iconFn, char defaultIcon) {
    Key min, max;
    GetLimitsFromMap(map, min, max);
    for (auto row = min.Y; row <= max.Y; row++) {
        for (auto col = min.X; col <= max.X; col++) {
            Key key = { col, row };
            char icon = defaultIcon;
            if (map.find(key) != map.end()) {
                Value val = map.at(key);
                icon = iconFn(key, val);
            }
            std::cout << icon;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}