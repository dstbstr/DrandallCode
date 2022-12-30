#pragma once

#include "Common.h"

/*
      N
  NW    NE

  SW    SE
      S

q + r + s = 0
*/

enum struct Direction {N, NE, SE, S, SW, NW};

struct Pos {
    s32 Q;
    s32 R;
    s32 S;
};

constexpr u32 MDistance(Pos lhs, Pos rhs) {
    return (
        Constexpr::Abs(lhs.Q - rhs.Q) +
        Constexpr::Abs(lhs.R - rhs.R) +
        Constexpr::Abs(lhs.S - rhs.S)
        ) / 2;
}

static_assert(MDistance({ 0, 0, 0 }, { 1, 0, -1 }) == 1);

constexpr void Move(Pos& pos, Direction dir) {
    switch (dir) {
    case Direction::N: pos.S++, pos.R--; break;
    case Direction::S: pos.S--, pos.R++; break;
    case Direction::NE: pos.Q++, pos.R--; break;
    case Direction::SW: pos.Q--, pos.R++; break;
    case Direction::NW: pos.Q--, pos.S++; break;
    case Direction::SE: pos.Q++, pos.S--; break;
    }
}

constexpr Direction ParseDirection(std::string_view dir) {
    if (dir == "n") return Direction::N;
    else if (dir == "ne") return Direction::NE;
    else if (dir == "se") return Direction::SE;
    else if (dir == "s") return Direction::S;
    else if (dir == "sw") return Direction::SW;
    else if (dir == "nw") return Direction::NW;
    return Direction::N; //should never happen
}

constexpr auto PartOne(const std::string& line) {
    auto split = Constexpr::Split(line, ",");
    Pos pos{ 0, 0, 0 };
    for (auto sv : split) {
        Move(pos, ParseDirection(sv));
    }

    return MDistance(pos, { 0, 0, 0 });
}

static_assert(PartOne("ne,ne,ne") == 3);
static_assert(PartOne("ne,ne,sw,sw") == 0);
static_assert(PartOne("ne,ne,s,s") == 2);
static_assert(PartOne("se,sw,se,sw,sw") == 3);

constexpr auto PartTwo(const std::string& line) {
    auto split = Constexpr::Split(line, ",");
    u32 max = 0;
    Pos pos{ 0, 0, 0 };
    const Pos origin{ 0, 0, 0 };
    for (auto sv : split) {
        Move(pos, ParseDirection(sv));
        max = std::max(max, MDistance(pos, origin));
    }

    return max;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines[0]));
    return ToString(PartTwo(lines[0]));
}

bool RunTests() {
    if (PartOne("ne,ne,ne") != 3) return false;
    if (PartOne("ne,ne,sw,sw") != 0) return false;
    if (PartOne("ne,ne,s,s") != 2) return false;
    if (PartOne("se,sw,se,sw,sw") != 3) return false;
    return true;
}