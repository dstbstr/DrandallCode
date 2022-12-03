#pragma once

#include "Common.h"

#include <bitset>

enum struct Direction { North, South, East, West };
enum struct Turn { Left, Right };
constexpr Direction NewDirection(Direction current, Turn turn) {
    if(turn == Turn::Left) {
        switch(current) {
            case Direction::North: return Direction::West;
            case Direction::East: return Direction::North;
            case Direction::South: return Direction::East;
            default: case Direction::West: return Direction::South;
        }
    } else {
        switch(current) {
            case Direction::North: return Direction::East;
            case Direction::East: return Direction::South;
            case Direction::South: return Direction::West;
            default: case Direction::West: return Direction::North;
        }
    }
}

static_assert(NewDirection(Direction::South, Turn::Left) == Direction::East);

constexpr u32 FindManhattenDistance(s32 x, s32 y) {
    return Constexpr::Abs(x) + Constexpr::Abs(y);
}

static_assert(FindManhattenDistance(3, 3) == 6);
static_assert(FindManhattenDistance(-3, 3) == 6);
static_assert(FindManhattenDistance(3, -3) == 6);
static_assert(FindManhattenDistance(-3, -3) == 6);


constexpr void Move(Direction facing, u32 amount, s32& x, s32& y) {
    switch(facing)
    {
        case Direction::North: y += amount; break;
        case Direction::South: y -= amount; break;
        case Direction::East: x += amount; break;
        case Direction::West: x -= amount; break;
    }
}

u32 FindTarget(const std::string& directions) {
    auto split = StrUtil::Split(directions, ", ");
    Turn turn;
    Direction facing = Direction::North;
    u32 amount;
    s32 targetX = 0;
    s32 targetY = 0;

    for(auto instruction : split) {
        turn = instruction[0] == 'L' ? Turn::Left : Turn::Right;
        ParseNumber(instruction.substr(1), amount);
        facing = NewDirection(facing, turn);

        Move(facing, amount, targetX, targetY);
    }

    return FindManhattenDistance(targetX, targetY);
}

u32 FindTarget2(const std::string& directions) {
    const u32 offset = 500;
    std::bitset<1000 * 1000> seen;
    Turn turn;
    Direction facing = Direction::North;
    u32 amount;
    u32 x = offset;
    u32 y = offset;
    s32 finalX = 0;
    s32 finalY = 0;
    bool found = false;

    auto move = [&](u32& value, s8 change) {
        for(u32 i = 0; i < amount; i++) {
            auto index = x * 500 + y;
            if(seen[index]) {
                finalX = x - offset;
                finalY = y - offset;
                found = true;
                break;
            }
            seen.set(index);
            value += change;
        }
    };

    auto split = StrUtil::Split(directions, ", ");
    for(auto instruction : split) {
        turn = instruction[0] == 'L' ? Turn::Left : Turn::Right;
        ParseNumber(instruction.substr(1), amount);
        facing = NewDirection(facing, turn);

        switch(facing) {
            case Direction::North: move(y, 1); break;
            case Direction::South: move(y, -1); break;
            case Direction::East: move(x, 1); break;
            case Direction::West: move(x, -1); break;
        }
        
        if(found) {
            break;
        }
    }

    return FindManhattenDistance(finalX, finalY);
}

bool RunTests() {
    if(FindTarget("R2, L3") != 5) return false;
    if(FindTarget("R2, R2, R2") != 2) return false;
    if(FindTarget("R5, L5, R5, R3") != 12) return false;

    if(FindTarget2("R8, R4, R4, R8") != 4) return false;
    return true;
}
