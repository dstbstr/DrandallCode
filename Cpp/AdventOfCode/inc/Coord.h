#pragma once

#include "Platform/Types.h"

template<typename T>
std::string ToString(T input);

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

template<>
inline std::string ToString(Coord coord) {
    return "{" + ToString(coord.X) + "," + ToString(coord.Y) + "}";
}

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

template<>
inline std::string ToString(UCoord coord) {
    return "{" + ToString(coord.X) + "," + ToString(coord.Y) + "}";
}

constexpr void GetLimits(const std::vector<UCoord>& coords, UCoord& min, UCoord& max) {
    min.X = 9999;
    min.Y = 9999;
    max.X = 0;
    max.Y = 0;

    for (const auto& coord : coords) {
        min.X = std::min(min.X, coord.X);
        min.Y = std::min(min.Y, coord.Y);
        max.X = std::max(max.X, coord.X);
        max.Y = std::max(max.Y, coord.Y);
    }
}

constexpr void GetLimits(const std::vector<Coord>& coords, Coord& min, Coord& max) {
    min.X = 9999;
    min.Y = 9999;
    max.X = 0;
    max.Y = 0;

    for (const auto& coord : coords) {
        min.X = std::min(min.X, coord.X);
        min.Y = std::min(min.Y, coord.Y);
        max.X = std::max(max.X, coord.X);
        max.Y = std::max(max.Y, coord.Y);
    }
}


struct RowCol {
    size_t Row;
    size_t Col;
    constexpr bool operator==(const RowCol& rc) const {
        return Row == rc.Row && Col == rc.Col;
    }
    constexpr bool operator<(const RowCol& rc) const {
        return Row < rc.Row ? true : Row > rc.Row ? false :
            Col < rc.Col ? true : false;
    }
};

struct RowColHash {
    size_t operator()(const RowCol& rc) const {
        return rc.Row ^ rc.Col;
    }
};

template<>
inline std::string ToString(RowCol rc) {
    return "Row: " + ToString(rc.Row) + " Col: " + ToString(rc.Col);
}

constexpr void GetLimits(const std::vector<RowCol>& coords, RowCol& min, RowCol& max) {
    min.Row = 9999;
    min.Col = 9999;
    max.Row = 0;
    max.Col = 0;

    for (const auto& coord : coords) {
        min.Row = std::min(min.Row, coord.Row);
        min.Col = std::min(min.Col, coord.Col);
        max.Row = std::max(max.Row, coord.Row);
        max.Col = std::max(max.Col, coord.Col);
    }
}

template<typename T>
struct Vec3 {
    T X;
    T Y;
    T Z;

    constexpr bool operator==(const Vec3<T>& v) const = default;
    constexpr Vec3<T> operator+(const Vec3<T>& v) {
        Vec3<T> result;
        result.X = X + v.X;
        result.Y = Y + v.Y;
        result.Z = Z + v.Z;
        return result;
    }

    constexpr void operator+=(const Vec3<T>& v) {
        X += v.X;
        Y += v.Y;
        Z += v.Z;
    }

    constexpr Vec3<T> operator-(const Vec3<T>&v) {
        Vec3<T> result;
        result.X = X - v.X;
        result.Y = Y - v.Y;
        result.Z = Z - v.Z;
        return result;
    }
    constexpr void operator-=(const Vec3<T>& v) {
        X -= v.X;
        Y -= v.Y;
        Z -= v.Z;
    }
};

template<typename T>
struct Vec3Hash {
    size_t operator()(const Vec3<T> v) const {
        return v.X ^ v.Y ^ v.Z;
    }
};

constexpr s32 MDistance(const Coord& lhs, const Coord& rhs) {
    s32 result = 0;
    result += (lhs.X < rhs.X ? rhs.X - lhs.X : lhs.X - rhs.X);
    result += (lhs.Y < rhs.Y ? rhs.Y - lhs.Y : lhs.Y - rhs.Y);
    return result;
}

constexpr u32 MDistance(const UCoord& lhs, const UCoord& rhs) {
    u32 result = 0;
    result += (lhs.X < rhs.X ? rhs.X - lhs.X : lhs.X - rhs.X);
    result += (lhs.Y < rhs.Y ? rhs.Y - lhs.Y : lhs.Y - rhs.Y);
    return result;
}

constexpr size_t MDistance(const RowCol& lhs, const RowCol& rhs) {
    size_t result = 0;
    result += (lhs.Row < rhs.Row ? rhs.Row - lhs.Row : lhs.Row - rhs.Row);
    result += (lhs.Col < rhs.Col ? rhs.Col - lhs.Col : lhs.Col - rhs.Col);
    return result;
}

template<typename T>
constexpr T MDistance(const Vec3<T> lhs, const Vec3<T> rhs) {
    T result = 0;
    result += (lhs.X < rhs.X ? rhs.X - lhs.X : lhs.X - rhs.X);
    result += (lhs.Y < rhs.Y ? rhs.Y - lhs.Y : lhs.Y - rhs.Y);
    result += (lhs.Z < rhs.Z ? rhs.Z - lhs.Z : lhs.Z - rhs.Z);
    return result;
}

constexpr std::vector<Coord> Get4Neighbors(const Coord& pos, const Coord& max, const Coord& min = { 0, 0 }) {
    std::vector<Coord> result;
    if (pos.X > min.X) result.push_back({ pos.X - 1, pos.Y });
    if (pos.Y > min.Y) result.push_back({ pos.X, pos.Y - 1 });
    if (pos.X < max.X) result.push_back({ pos.X + 1, pos.Y });
    if (pos.Y < max.Y) result.push_back({ pos.X, pos.Y + 1 });
    return result;
}

constexpr std::vector<UCoord> Get4Neighbors(const UCoord& pos, const UCoord& max, const UCoord& min = { 0, 0 }) {
    std::vector<UCoord> result;
    if (pos.X > min.X) result.push_back({ pos.X - 1, pos.Y });
    if (pos.Y > min.Y) result.push_back({ pos.X, pos.Y - 1 });
    if (pos.X < max.X) result.push_back({ pos.X + 1, pos.Y });
    if (pos.Y < max.Y) result.push_back({ pos.X, pos.Y + 1 });
    return result;
}

constexpr std::vector<RowCol> Get4Neighbors(const RowCol& pos, const RowCol& max, const RowCol& min = { 0, 0 }) {
    std::vector<RowCol> result;
    if (pos.Row > min.Row) result.push_back({ pos.Row - 1, pos.Col });
    if (pos.Col > min.Col) result.push_back({ pos.Row, pos.Col - 1 });
    if (pos.Row < max.Row) result.push_back({ pos.Row + 1, pos.Col });
    if (pos.Col < max.Col) result.push_back({ pos.Row, pos.Col + 1 });
    return result;
}

constexpr std::vector<Coord> Get8Neighbors(const Coord& pos, const Coord& max, const Coord& min = { 0, 0 }) {
    std::vector<Coord> result = Get4Neighbors(pos, max, min);

    if (pos.X > min.X) {
        if (pos.Y > min.Y) result.push_back({ pos.X - 1, pos.Y - 1 });
        if (pos.Y < max.Y) result.push_back({ pos.X - 1, pos.Y + 1 });
    }
    if (pos.X < max.X) {
        if (pos.Y > min.Y) result.push_back({ pos.X + 1, pos.Y - 1 });
        if (pos.Y < max.Y) result.push_back({ pos.X + 1, pos.Y + 1 });
    }
    return result;
}

constexpr std::vector<UCoord> Get8Neighbors(const UCoord& pos, const UCoord& max, const UCoord& min = { 0, 0 }) {
    std::vector<UCoord> result = Get4Neighbors(pos, max, min);

    if (pos.X > min.X) {
        if (pos.Y > min.Y) result.push_back({ pos.X - 1, pos.Y - 1 });
        if (pos.Y < max.Y) result.push_back({ pos.X - 1, pos.Y + 1 });
    }
    if (pos.X < max.X) {
        if (pos.Y > min.Y) result.push_back({ pos.X + 1, pos.Y - 1 });
        if (pos.Y < max.Y) result.push_back({ pos.X + 1, pos.Y + 1 });
    }
    return result;
}

constexpr std::vector<RowCol> Get8Neighbors(const RowCol& pos, const RowCol& max, const RowCol& min = { 0, 0 }) {
    std::vector<RowCol> result = Get4Neighbors(pos, max, min);

    if (pos.Row > min.Row) {
        if (pos.Col > min.Col) result.push_back({ pos.Row - 1, pos.Col - 1 });
        if (pos.Col < max.Col) result.push_back({ pos.Row - 1, pos.Col + 1 });
    }
    if (pos.Row < max.Row) {
        if (pos.Col > min.Col) result.push_back({ pos.Row + 1, pos.Col - 1 });
        if (pos.Col < max.Col) result.push_back({ pos.Row + 1, pos.Col + 1 });
    }
    return result;
}