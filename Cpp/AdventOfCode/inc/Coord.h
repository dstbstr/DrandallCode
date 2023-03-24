#pragma once

#include "Platform/Types.h"
#include <string>
#include <ostream>

template<typename T>
std::string ToString(T input);

struct Coord {
    s32 X;
    s32 Y;

    constexpr bool operator==(const Coord& c) const {
        return this->X == c.X && this->Y == c.Y;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Coord& coord) {
        stream << "{" << coord.X << "," << coord.Y << "}";
        return stream;
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
    
    friend std::ostream& operator<<(std::ostream& stream, const UCoord& coord) {
        stream << "{" << coord.X << "," << coord.Y << "}";
        return stream;
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
    friend std::ostream& operator<<(std::ostream& stream, const RowCol& rc) {
        stream << "Row: " << rc.Row << " Col: " << rc.Col;
        return stream;
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

template<typename T, typename Start, typename End>
constexpr void GetLimits(Start start, End end, T& min, T& max) {
    min.X = std::numeric_limits<decltype(min.X)>::max();
    min.Y = std::numeric_limits<decltype(min.Y)>::max();
    max.X = std::numeric_limits<decltype(max.X)>::min();
    max.Y = std::numeric_limits<decltype(max.Y)>::min();

    while (start != end) {
        min.X = std::min(min.X, start->X);
        min.Y = std::min(min.Y, start->Y);
        max.X = std::max(max.X, start->X);
        max.Y = std::max(max.Y, start->Y);
        start++;
    }
}

template<typename Start, typename End>
constexpr void GetLimits(Start start, End end, RowCol& min, RowCol& max) {
    min.Row = std::numeric_limits<decltype(min.Row)>::max();
    min.Col = std::numeric_limits<decltype(min.Col)>::max();
    max.Row = std::numeric_limits<decltype(max.Row)>::min();
    max.Col = std::numeric_limits<decltype(max.Col)>::min();

    while (start != end) {
        min.Row = std::min(min.Row, start->Row);
        min.Col = std::min(min.Col, start->Col);
        max.Row = std::max(max.Row, start->Row);
        max.Col = std::max(max.Col, start->Col);
        start++;
    }
}

template<typename Key>
void GetLimitsFromMap(const auto& map, Key& min, Key& max) {
    min.X = std::numeric_limits<decltype(min.X)>::max();
    min.Y = std::numeric_limits<decltype(min.Y)>::max();
    max.X = std::numeric_limits<decltype(max.X)>::min();
    max.Y = std::numeric_limits<decltype(max.Y)>::min();

    for (const auto& kvp : map) {
        min.X = std::min(min.X, kvp.first.X);
        min.Y = std::min(min.Y, kvp.first.Y);
        max.X = std::max(max.X, kvp.first.X);
        max.Y = std::max(max.Y, kvp.first.Y);
    }
}

void GetLimitsFromMap(const auto& map, RowCol& min, RowCol& max) {
    min.Row = std::numeric_limits<decltype(min.Row)>::max();
    min.Col = std::numeric_limits<decltype(min.Col)>::max();
    max.Row = std::numeric_limits<decltype(max.Row)>::min();
    max.Col = std::numeric_limits<decltype(max.Col)>::min();

    for (const auto& kvp : map) {
        min.Row = std::min(min.Row, kvp.first.Row);
        min.Col = std::min(min.Col, kvp.first.Col);
        max.Row = std::max(max.Row, kvp.first.Row);
        max.Col = std::max(max.Col, kvp.first.Col);
    }
}

template<typename T>
struct Vec3 {
    T X = 0;
    T Y = 0;
    T Z = 0;

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

    friend std::ostream& operator<<(std::ostream& stream, const Vec3<T>& vec) {
        stream << "<" << vec.X << "," << vec.Y << "," << vec.Z << ">";
        return stream;
    }
};

template<typename T>
struct Vec3Hash {
    size_t operator()(const Vec3<T> v) const {
        return v.X ^ v.Y ^ v.Z;
    }
};

template<typename T>
struct Vec4 {
    T X;
    T Y;
    T Z;
    T W;

    constexpr bool operator==(const Vec4<T>& v) const = default;
    constexpr Vec4<T> operator+(const Vec4<T>& v) {
        Vec4<T> result;
        result.X = X + v.X;
        result.Y = Y + v.Y;
        result.Z = Z + v.Z;
        result.W = W + v.W;
        return result;
    }

    constexpr void operator+=(const Vec4<T>& v) {
        X += v.X;
        Y += v.Y;
        Z += v.Z;
        W += v.W;
    }

    constexpr Vec4<T> operator-(const Vec4<T>& v) {
        Vec4<T> result;
        result.X = X - v.X;
        result.Y = Y - v.Y;
        result.Z = Z - v.Z;
        result.W = W - v.W;
        return result;
    }
    constexpr void operator-=(const Vec4<T>& v) {
        X -= v.X;
        Y -= v.Y;
        Z -= v.Z;
        W -= v.W;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vec4<T>& vec) {
        stream << "<" << vec.X << "," << vec.Y << "," << vec.Z << "," << vec.W << ">";
        return stream;
    }

};
template<typename T>
struct Vec4Hash {
    size_t operator()(const Vec4<T> v) const {
        return v.X ^ v.Y ^ v.Z ^ v.W;
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

template<typename T>
constexpr T MDistance(const Vec4<T> lhs, const Vec4<T> rhs) {
    T result = 0;
    result += (lhs.X < rhs.X ? rhs.X - lhs.X : lhs.X - rhs.X);
    result += (lhs.Y < rhs.Y ? rhs.Y - lhs.Y : lhs.Y - rhs.Y);
    result += (lhs.Z < rhs.Z ? rhs.Z - lhs.Z : lhs.Z - rhs.Z);
    result += (lhs.W < rhs.W ? rhs.W - lhs.W : lhs.W - rhs.W);
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