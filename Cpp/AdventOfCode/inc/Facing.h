#pragma once

#include <string>
#include "Constexpr/ConstexprGeometry.h"

enum Facing { Up, Right, Down, Left };
enum Direction {North, East, South, West};

constexpr std::string ToString(Facing facing) {
    switch (facing) {
    case Up: return "Up";
    case Right: return "Right";
    case Down: return "Down";
    case Left: return "Left";
    default: return "Unknown";
    }
}

constexpr std::string ToString(Direction dir) {
    switch (dir) {
    case North: return "North";
    case East: return "East";
    case South: return "South";
    case West: return "West";
    default: return "Unknown";
    }
}

constexpr Facing TurnRight(Facing current) {
    return static_cast<Facing>((current + 1) % 4);
}
constexpr Facing TurnLeft(Facing current) {
    return static_cast<Facing>((4 + (current - 1)) % 4);
}
constexpr Facing TurnAround(Facing current) {
    return static_cast<Facing>((current + 2) % 4);
}

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

static_assert(TurnRight(North) == East);
static_assert(TurnLeft(North) == West);
static_assert(TurnRight(West) == North);
static_assert(TurnLeft(West) == South);
static_assert(TurnAround(North) == South);
static_assert(TurnAround(South) == North);
static_assert(TurnAround(West) == East);
static_assert(TurnAround(East) == West);

constexpr void Move(Coord& pos, Facing facing) {
    switch (facing) {
    case Up: pos.Y--; return;
    case Down: pos.Y++; return;
    case Left: pos.X--; return;
    case Right: pos.X++; return;
    default:break;
    }
}

constexpr void Move(Coord& pos, Facing facing, u32 amount) {
    switch (facing) {
    case Up: pos.Y -= amount; return;
    case Down: pos.Y += amount; return;
    case Left: pos.X -= amount; return;
    case Right: pos.X += amount; return;
    default: break;
    }
}

constexpr void Move(UCoord& pos, Facing facing) {
    switch (facing) {
    case Up: pos.Y--; return;
    case Down: pos.Y++; return;
    case Left: pos.X--; return;
    case Right: pos.X++; return;
    default:break;
    }
}

constexpr void Move(UCoord& pos, Facing facing, u32 amount) {
    switch (facing) {
    case Up: pos.Y -= amount; return;
    case Down: pos.Y += amount; return;
    case Left: pos.X -= amount; return;
    case Right: pos.X += amount; return;
    default: break;
    }
}

constexpr void Move(Coord& pos, Direction facing) {
    switch (facing) {
    case North: pos.Y--; return;
    case South: pos.Y++; return;
    case West: pos.X--; return;
    case East: pos.X++; return;
    default:break;
    }
}

constexpr void Move(Coord& pos, Direction facing, u32 amount) {
    switch (facing) {
    case North: pos.Y -= amount; return;
    case South: pos.Y += amount; return;
    case West: pos.X -= amount; return;
    case East: pos.X += amount; return;
    default:break;
    }
}

constexpr void Move(UCoord& pos, Direction facing) {
    switch (facing) {
    case North: pos.Y--; return;
    case South: pos.Y++; return;
    case West: pos.X--; return;
    case East: pos.X++; return;
    default:break;
    }
}

constexpr void Move(UCoord& pos, Direction facing, u32 amount) {
    switch (facing) {
    case North: pos.Y -= amount; return;
    case South: pos.Y += amount; return;
    case West: pos.X -= amount; return;
    case East: pos.X += amount; return;
    default:break;
    }
}