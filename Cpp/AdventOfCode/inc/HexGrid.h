#pragma once

#include "Common.h"

//https://www.redblobgames.com/grids/hexagons/

namespace FlatHex {
    /*
     \ N  /
      +--+
  NW /    \ NE
 ---+      +---
  SW \    / SE
      +--+
     / S  \
    */
    enum struct Direction { N, NE, SE, S, SW, NW };

    //q + r + s = 0
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

        case Direction::SE: pos.Q++, pos.S--; break;
        case Direction::NW: pos.Q--, pos.S++; break;
        }
    }

    constexpr Direction ParseDirection(std::string_view dir) {
        if (dir == "n") return Direction::N;
        else if (dir == "ne") return Direction::NE;
        else if (dir == "se") return Direction::SE;
        else if (dir == "s") return Direction::S;
        else if (dir == "sw") return Direction::SW;
        else if (dir == "nw") return Direction::NW;
        throw "Unknown Direction";
    }
}

namespace PointyHex {
    /*
         \+/
       NW/ \ NE
     ---+   +---
      W |   | E
     ---+   +---
      SW \ / SE
         /+\
    */

    enum struct Direction { NE, E, SE, SW, W, NW };

    //q + r + s = 0
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
        case Direction::NE: pos.Q++, pos.R--; break;
        case Direction::SW: pos.Q--, pos.R++; break;

        case Direction::E: pos.Q++, pos.S--; break;
        case Direction::W: pos.Q--, pos.S++; break;

        case Direction::SE: pos.R++, pos.S--; break;
        case Direction::NW: pos.R--, pos.S++; break;
        }
    }

    constexpr Direction ParseDirection(std::string_view dir) {
        if (dir == "ne") return Direction::NE;
        else if (dir == "e") return Direction::E;
        else if (dir == "se") return Direction::SE;
        else if (dir == "sw") return Direction::SW;
        else if (dir == "w") return Direction::W;
        else if (dir == "nw") return Direction::NW;
        throw "Unknown Direction";
    }
}