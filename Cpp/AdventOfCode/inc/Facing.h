#pragma once

#include "Core/Constexpr/ConstexprGeometry.h"

#include <string>

enum Facing { Up, Right, Down, Left };
enum Direction {North, East, South, West};

constexpr Facing ParseFacing(char c) {
    switch (c){
    case 'u': case 'U': return Facing::Up;
    case 'd': case 'D': return Facing::Down;
    case 'l': case 'L': return Facing::Left;
    case 'r': case 'R': return Facing::Right;
    default: throw "Unknown";
    }
}
constexpr Direction ParseDirection(char c) {
    switch (c) {
    case 'n': case 'N': return Direction::North;
    case 'e': case 'E': return Direction::East;
    case 'w': case 'W': return Direction::West;
    case 's': case 'S': return Direction::South;
    }
}

constexpr std::string ToString(Facing facing) {
    switch (facing) {
    case Facing::Up: return "Up";
    case Facing::Right: return "Right";
    case Facing::Down: return "Down";
    case Facing::Left: return "Left";
    default: return "Unknown";
    }
}

constexpr std::string ToString(Direction dir) {
    switch (dir) {
    case Direction::North: return "North";
    case Direction::East: return "East";
    case Direction::South: return "South";
    case Direction::West: return "West";
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

constexpr void Move(Coord& pos, Facing facing, size_t amount) {
    switch (facing) {
    case Up: pos.Y -= amount; return;
    case Down: pos.Y += amount; return;
    case Left: pos.X -= amount; return;
    case Right: pos.X += amount; return;
    default: break;
    }
}

constexpr void Move(Coord& pos, Facing facing) {
    Move(pos, facing, 1u);
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

constexpr void Move(UCoord& pos, Facing facing) {
    Move(pos, facing, 1u);
}

constexpr void Move(RowCol& pos, Facing facing, u32 amount) {
    switch (facing) {
    case Up: pos.Row -= amount; return;
    case Down: pos.Row += amount; return;
    case Left: pos.Col -= amount; return;
    case Right: pos.Col += amount; return;
    default: break;
    }
}

constexpr void Move(RowCol& pos, Facing facing) {
    Move(pos, facing, 1u);
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

constexpr void Move(Coord& pos, Direction facing) {
    Move(pos, facing, 1u);
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

constexpr void Move(UCoord& pos, Direction facing) {
    Move(pos, facing, 1u);
}
