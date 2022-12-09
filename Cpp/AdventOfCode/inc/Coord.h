#pragma once

#include "Platform/Types.h"

struct Coord {
    s32 X;
    s32 Y;

    constexpr bool operator==(const Coord& c) const {
        return this->X == c.X && this->Y == c.Y;
    }
};

struct CoordHash {
    size_t operator()(const Coord& c) const {
        return c.X ^ c.Y;
    }
};

struct UCoord {
    u32 X;
    u32 Y;
    constexpr bool operator==(const UCoord& c) const {
        return this->X == c.X && this->Y == c.Y;
    }
};

struct UCoordHash {
    size_t operator()(const UCoord& c) const {
        return c.X ^ c.Y;
    }
};