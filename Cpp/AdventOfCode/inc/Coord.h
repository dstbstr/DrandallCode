#pragma once

#include "Platform/Types.h"
#include "Utilities/ConstexprUtils.h"

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

constexpr s32 MDistance(const Coord& lhs, const Coord& rhs) {
    return Constexpr::Abs(lhs.X - rhs.X) + Constexpr::Abs(lhs.Y - rhs.Y);
}

constexpr u32 MDistance(const UCoord& lhs, const UCoord& rhs) {
    u32 result = 0;
    result += (lhs.X < rhs.X ? rhs.X - lhs.X : lhs.X - rhs.X);
    result += (lhs.Y < rhs.Y ? rhs.Y - lhs.Y : lhs.Y - rhs.Y);
    return result;
}