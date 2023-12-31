#pragma once
#include <string>
#include <ostream>

#include "Platform/Types.h"
#include "Constexpr/ConstexprMath.h"
#include "Constexpr/ConstexprStrUtils.h"
#include "Constexpr/ConstexprHash.h"

template<typename T>
std::string ToString(T input);

struct Coord {
    s64 X{ 0 };
    s64 Y{ 0 };

    constexpr Coord() = default;
    constexpr Coord(s64 x, s64 y) : X(x), Y(y) {}

    constexpr Coord(std::string_view sv, std::string_view delimiter = ",") {
        auto split = Constexpr::Split(sv, delimiter);
        Constexpr::ParseNumber(split[0], X);
        Constexpr::ParseNumber(split[1], Y);
    }

    constexpr bool operator==(const Coord& c) const {
        return this->X == c.X && this->Y == c.Y;
    }

    constexpr bool operator<(const Coord& c) const {
        return X == c.X ? Y < c.Y : X < c.X;
    }

    constexpr Coord& operator+=(const Coord& c) {
        X += c.X;
        Y += c.Y;
        return *this;
    }
    constexpr Coord operator+(const Coord& c) {
        Coord result{ X, Y };
        return result += c;
    }

    constexpr Coord& operator-=(const Coord& c) {
        X -= c.X;
        Y -= c.Y;
        return *this;
    }
    constexpr Coord operator-(const Coord& c) {
        Coord result{ X, Y };
        return result -= c;
    }
    constexpr Coord& operator*=(s64 v) {
        X *= v;
        Y *= v;
        return *this;
    }
    constexpr Coord operator*(s64 v) {
        Coord result{ X, Y };
        return result *= v;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Coord& coord) {
        stream << "{" << coord.X << "," << coord.Y << "}";
        return stream;
    }
};

template<>
struct std::hash<Coord> {
    std::size_t operator()(const Coord& c) const {
        return c.X ^ c.Y;
    }
};

template<>
inline std::string ToString(Coord coord) {
    return "{" + ToString(coord.X) + "," + ToString(coord.Y) + "}";
}

namespace Constexpr {
    template<>
    constexpr std::string ToString(Coord coord) {
        return "{" + Constexpr::ToString(coord.X) + "," + Constexpr::ToString(coord.Y) + "}";
    }

    template<>
    struct Hasher<Coord> {
        constexpr size_t operator()(const Coord& c) const {
            return mHash(c.X) ^ mHash(c.Y);
        }

        Hasher<size_t> mHash{};
    };

    //constexpr size_t Hash(Coord coord) {
    //    return Hash(coord.X) ^ Hash(coord.Y);
    //}
}

struct UCoord {
    u64 X{ 0 };
    u64 Y{ 0 };
    constexpr UCoord() = default;
    constexpr UCoord(u64 x, u64 y) : X(x), Y(y) {}

    constexpr UCoord(std::string_view sv, std::string_view delimiter = ",") {
        auto split = Constexpr::Split(sv, delimiter);
        Constexpr::ParseNumber(split[0], X);
        Constexpr::ParseNumber(split[1], Y);
    }

    constexpr bool operator==(const UCoord& c) const {
        return this->X == c.X && this->Y == c.Y;
    }
    constexpr bool operator<(const UCoord& c) const {
        return X == c.X ? Y < c.Y : X < c.X;
    }

    constexpr UCoord& operator+=(const UCoord& c) {
        X += c.X;
        Y += c.Y;
        return *this;
    }
    constexpr UCoord operator+(const UCoord& c) {
        UCoord result{ X, Y };
        return result += c;
    }

    constexpr UCoord& operator-=(const UCoord& c) {
        X -= c.X;
        Y -= c.Y;
        return *this;
    }
    constexpr UCoord operator-(const UCoord& c) {
        UCoord result{ X, Y };
        return result -= c;
    }

    friend std::ostream& operator<<(std::ostream& stream, const UCoord& coord) {
        stream << "{" << coord.X << "," << coord.Y << "}";
        return stream;
    }
};

template<>
struct std::hash<UCoord> {
    std::size_t operator()(const UCoord& c) const {
        return c.X ^ c.Y;
    }
};

template<>
inline std::string ToString(UCoord coord) {
    return "{" + ToString(coord.X) + "," + ToString(coord.Y) + "}";
}

namespace Constexpr {
    template<>
    constexpr std::string ToString(UCoord coord) {
        return "{" + Constexpr::ToString(coord.X) + "," + Constexpr::ToString(coord.Y) + "}";
    }

    template<>
    struct Hasher<UCoord> {
        constexpr size_t operator()(const UCoord& c) const {
            return mHash(c.X) ^ mHash(c.Y);
        }

        Hasher<s64> mHash{};
    };
    //constexpr size_t Hash(UCoord coord) {
    //    return Hash(coord.X) ^ Hash(coord.Y);
    //}
}

constexpr void GetLimits(const std::vector<UCoord>& coords, UCoord& min, UCoord& max) {
    min.X = 999999;
    min.Y = 999999;
    max.X = 0;
    max.Y = 0;

    for (const auto& coord : coords) {
        min.X = std::min(min.X, coord.X);
        min.Y = std::min(min.Y, coord.Y);
        max.X = std::max(max.X, coord.X);
        max.Y = std::max(max.Y, coord.Y);
    }
}

constexpr std::pair<UCoord, UCoord> GetLimits(const std::vector<UCoord>& coords) {
    UCoord min, max;
    GetLimits(coords, min, max);
    return std::make_pair(min, max);
}

constexpr void GetLimits(const std::vector<Coord>& coords, Coord& min, Coord& max) {
    min.X = 999999;
    min.Y = 999999;
    max.X = -999999;
    max.Y = -999999;

    for (const auto& coord : coords) {
        min.X = std::min(min.X, coord.X);
        min.Y = std::min(min.Y, coord.Y);
        max.X = std::max(max.X, coord.X);
        max.Y = std::max(max.Y, coord.Y);
    }
}
constexpr std::pair<Coord, Coord> GetLimits(const std::vector<Coord>& coords) {
    Coord min, max;
    GetLimits(coords, min, max);
    return std::make_pair(min, max);
}

struct RowCol {
    size_t Row;
    size_t Col;
    constexpr bool operator==(const RowCol& rc) const {
        return Row == rc.Row && Col == rc.Col;
    }
    constexpr bool operator<(const RowCol& rc) const {
        return Row != rc.Row ? Row < rc.Row : Col < rc.Col;
    }
    friend std::ostream& operator<<(std::ostream& stream, const RowCol& rc) {
        stream << "{" << rc.Row << "," << rc.Col << "}";
        return stream;
    }
    constexpr RowCol& operator+=(const RowCol& rc) {
        Row += rc.Row;
        Col += rc.Col;
        return *this;
    }
    constexpr RowCol operator+(const RowCol& rc) const {
        auto result = *this;
        return result += rc;
    }
    constexpr RowCol& operator-=(const RowCol& rc) {
        Row -= rc.Row;
        Col -= rc.Col;
        return *this;
    }
    constexpr RowCol operator-(const RowCol& rc) const {
        auto result = *this;
        return result -= rc;
    }
};

template<>
struct std::hash<RowCol> {
    std::size_t operator()(const RowCol& rc) const {
        return rc.Row ^ rc.Col;
    }
};

template<>
inline std::string ToString(RowCol rc) {
    return "{" + ToString(rc.Row) + "," + ToString(rc.Col) + "}";
}

namespace Constexpr {
    template<>
    constexpr std::string ToString(RowCol rc) {
        return "{" + Constexpr::ToString(rc.Row) + "," + Constexpr::ToString(rc.Col) + "}";
    }

    template<>
    struct Hasher<RowCol> {
        constexpr size_t operator()(const RowCol& rc) const {

            return mHash(rc.Row) ^ mHash(rc.Col);
        }

        Hasher<size_t> mHash{};
    };
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
constexpr void GetLimitsFromMap(const auto& map, Key& min, Key& max) {
    min.X = std::numeric_limits<decltype(min.X)>::max();
    min.Y = std::numeric_limits<decltype(min.Y)>::max();
    max.X = std::numeric_limits<decltype(max.X)>::min();
    max.Y = std::numeric_limits<decltype(max.Y)>::min();

    for (const auto& pos : map.GetKeys()) {
        min.X = std::min(min.X, pos.X);
        min.Y = std::min(min.Y, pos.Y);
        max.X = std::max(max.X, pos.X);
        max.Y = std::max(max.Y, pos.Y);
    }
}

constexpr void GetLimitsFromMap(const auto& map, RowCol& min, RowCol& max) {
    min.Row = std::numeric_limits<decltype(min.Row)>::max();
    min.Col = std::numeric_limits<decltype(min.Col)>::max();
    max.Row = std::numeric_limits<decltype(max.Row)>::min();
    max.Col = std::numeric_limits<decltype(max.Col)>::min();

    for (const auto& key : map.GetKeys()) {
        min.Row = std::min(min.Row, key.Row);
        min.Col = std::min(min.Col, key.Col);
        max.Row = std::max(max.Row, key.Row);
        max.Col = std::max(max.Col, key.Col);
    }
}

template<typename T>
struct Vec3 {
    T X = {};
    T Y = {};
    T Z = {};

    constexpr Vec3() = default;
    constexpr Vec3(T x, T y, T z) : X(x), Y(y), Z(z) {}

    constexpr Vec3(std::string_view sv, std::string_view delimiter = ",") {
        auto split = Constexpr::Split(sv, delimiter);
        Constexpr::ParseNumber(split[0], X);
        Constexpr::ParseNumber(split[1], Y);
        Constexpr::ParseNumber(split[2], Z);
    }

    constexpr bool operator==(const Vec3<T>& v) const = default;

    constexpr Vec3<T>& operator+=(const Vec3<T>& v) {
        X += v.X;
        Y += v.Y;
        Z += v.Z;
        return *this;
    }
    constexpr Vec3<T> operator+(const Vec3<T>& v) const {
        Vec3<T> result = *this;
        return result += v;
    }

    constexpr Vec3<T>& operator-=(const Vec3<T>& v) {
        X -= v.X;
        Y -= v.Y;
        Z -= v.Z;

        return *this;
    }

    constexpr Vec3<T> operator-(const Vec3<T>& v) const {
        Vec3<T> result = *this;
        return result -= v;
    }

    constexpr Vec3<T>& operator*=(const T& s) {
        X *= s;
        Y *= s;
        Z *= s;
        return *this;
    }
    constexpr Vec3<T>& operator*=(const Vec3<T>& v) {
        X *= v.X;
        Y *= v.Y;
        Z *= v.Z;
        return *this;
    }
    constexpr Vec3<T> operator*(const T s) const {
        Vec3<T> result = *this;
        return result *= s;
    }
    constexpr Vec3<T> operator*(const Vec3<T>& v) const {
        Vec3<T> result = *this;
        return result *= v;
    }

    constexpr Vec3<T>& operator/=(const T& s) {
        X /= s;
        Y /= s;
        Z /= s;
        return *this;
    }
    constexpr Vec3<T>& operator/=(const Vec3<T>& v) {
        X /= v.X;
        Y /= v.Y;
        Z /= v.Z;
        return *this;
    }
    constexpr Vec3<T> operator/(const T& s) const {
        auto result = *this;
        return result /= s;
    }
    constexpr Vec3<T> operator/(const Vec3<T>& v) const {
        auto result = *this;
        return result /= v;
    }

    constexpr bool operator<(const Vec3<T>& v) const {
        return X != v.X ? X < v.X :
            Y != v.Y ? Y < v.Y :
            Z != v.Z ? Z < v.Z :
            false;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vec3<T>& vec) {
        stream << "<" << vec.X << "," << vec.Y << "," << vec.Z << ">";
        return stream;
    }
};

template<typename T>
inline std::string ToString(Vec3<T> vec) {
    return "<" + ToString(vec.X) + "," + ToString(vec.Y) + "," + ToString(vec.Z) + ">";
}

namespace Constexpr {
    template<typename T>
    constexpr std::string ToString(Vec3<T> vec) {
        return "<" + Constexpr::ToString(vec.X) + "," + Constexpr::ToString(vec.Y) + "," + Constexpr::ToString(vec.Z) + ">";
    }

    template<typename T>
    struct Hasher<Vec3<T>> {
        constexpr size_t operator()(const Vec3<T>& vec) const {
            return mHash(vec.X) ^ mHash(vec.Y) ^ mHash(vec.Z);
        }

        Hasher<T> mHash;
    };
}

template<typename T>
struct std::hash<Vec3<T>> {
    std::size_t operator()(const Vec3<T>& v) const {
        return v.X ^ v.Y ^ v.Z;
    }
};


template<typename T>
struct Vec4 {
    T X = {};
    T Y = {};
    T Z = {};
    T W = {};

    constexpr Vec4() = default;
    constexpr Vec4(T x, T y, T z, T w) : X(x), Y(y), Z(z), W(w) {}

    constexpr Vec4(std::string_view sv, std::string_view delimiter = ",") {
        auto split = Constexpr::Split(sv, delimiter);
        Constexpr::ParseNumber(split[0], X);
        Constexpr::ParseNumber(split[1], Y);
        Constexpr::ParseNumber(split[2], Z);
        Constexpr::ParseNumber(split[3], W);
    }

    constexpr bool operator==(const Vec4<T>& v) const = default;
    constexpr Vec4<T>& operator+=(const Vec4<T>& v) {
        X += v.X;
        Y += v.Y;
        Z += v.Z;
        W += v.W;
        return *this;
    }

    constexpr Vec4<T> operator+(const Vec4<T>& v) const {
        Vec4<T> result = *this;
        return result += v;
    }

    constexpr Vec4<T> operator-=(const Vec4<T>& v) {
        X -= v.X;
        Y -= v.Y;
        Z -= v.Z;
        W -= v.W;
        return *this;
    }

    constexpr Vec4<T> operator-(const Vec4<T>& v) const {
        Vec4<T> result = *this;
        return result -= v;
    }

    constexpr bool operator<(const Vec4<T>& v) const {
        return X != v.X ? X < v.X :
            Y != v.Y ? Y < v.Y :
            Z != v.Z ? Z < v.Z :
            W != v.W ? W < v.W :
            false;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vec4<T>& vec) {
        stream << "<" << vec.X << "," << vec.Y << "," << vec.Z << "," << vec.W << ">";
        return stream;
    }

};

template<typename T>
inline std::string ToString(Vec4<T> vec) {
    return "<" + ToString(vec.X) + "," + ToString(vec.Y) + "," + ToString(vec.Z) + "," + ToString(vec.W) + ">";
}

namespace Constexpr {
    template<typename T>
    constexpr std::string ToString(Vec4<T> vec) {
        return "<" + Constexpr::ToString(vec.X) + "," + Constexpr::ToString(vec.Y) + "," + Constexpr::ToString(vec.Z) + "," + Constexpr::ToString(vec.W) + ">";
    }

    template<typename T>
    struct Hasher<Vec4<T>> {
        constexpr size_t operator()(const Vec4<T>& vec) const {
            return mHash(vec.X) ^ mHash(vec.Y) ^ mHash(vec.Z) ^ mHash(vec.W);
        }

        Hasher<T> mHash{};
    };
}

template<typename T>
struct std::hash<Vec4<T>> {
    std::size_t operator()(const Vec4<T>& v) const {
        return v.X ^ v.Y ^ v.Z ^ v.W;
    }
};

constexpr size_t MDistance(const Coord& pos) {
    return Constexpr::Abs(pos.X) + Constexpr::Abs(pos.Y);
}

constexpr size_t MDistance(const Coord& lhs, const Coord& rhs) {
    return Constexpr::AbsDistance(lhs.X, rhs.X) + Constexpr::AbsDistance(lhs.Y, rhs.Y);
}

constexpr size_t MDistance(const UCoord& pos) {
    return pos.X + pos.Y;
}
constexpr size_t MDistance(const UCoord& lhs, const UCoord& rhs) {
    return Constexpr::AbsDistance(lhs.X, rhs.X) + Constexpr::AbsDistance(lhs.Y, rhs.Y);
}

constexpr size_t MDistance(const RowCol& pos) {
    return pos.Row + pos.Col;
}

constexpr size_t MDistance(const RowCol& lhs, const RowCol& rhs) {
    return Constexpr::AbsDistance(lhs.Row, rhs.Row) + Constexpr::AbsDistance(lhs.Col, rhs.Col);
}

template<typename T>
constexpr size_t MDistance(const Vec3<T> pos) {
    return static_cast<size_t>(Constexpr::Abs(pos.X) + Constexpr::Abs(pos.Y) + Constexpr::Abs(pos.Z));
}

template<typename T>
constexpr size_t MDistance(const Vec3<T> lhs, const Vec3<T> rhs) {
    return static_cast<size_t>(
        Constexpr::AbsDistance(lhs.X, rhs.X) +
        Constexpr::AbsDistance(lhs.Y, rhs.Y) +
        Constexpr::AbsDistance(lhs.Z, rhs.Z));
}

template<typename T>
constexpr size_t MDistance(const Vec4<T> pos) {
    return static_cast<size_t>(Constexpr::Abs(pos.X) + Constexpr::Abs(pos.Y) + Constexpr::Abs(pos.Z) + Constexpr::Abs(pos.W));
}

template<typename T>
constexpr size_t MDistance(const Vec4<T> lhs, const Vec4<T> rhs) {
    return static_cast<size_t>(
        Constexpr::AbsDistance(lhs.X, rhs.X) +
        Constexpr::AbsDistance(lhs.Y, rhs.Y) +
        Constexpr::AbsDistance(lhs.Z, rhs.Z) +
        Constexpr::AbsDistance(lhs.W, rhs.W));
}

constexpr std::vector<Coord> GetDirectNeighbors(const Coord& pos, const Coord& max, const Coord& min = { 0, 0 }) {
    std::vector<Coord> result;
    if (pos.X > min.X) result.push_back({ pos.X - 1, pos.Y });
    if (pos.Y > min.Y) result.push_back({ pos.X, pos.Y - 1 });
    if (pos.X < max.X) result.push_back({ pos.X + 1, pos.Y });
    if (pos.Y < max.Y) result.push_back({ pos.X, pos.Y + 1 });
    return result;
}

constexpr std::vector<UCoord> GetDirectNeighbors(const UCoord& pos, const UCoord& max, const UCoord& min = { 0, 0 }) {
    std::vector<UCoord> result;
    if (pos.X > min.X) result.push_back({ pos.X - 1, pos.Y });
    if (pos.Y > min.Y) result.push_back({ pos.X, pos.Y - 1 });
    if (pos.X < max.X) result.push_back({ pos.X + 1, pos.Y });
    if (pos.Y < max.Y) result.push_back({ pos.X, pos.Y + 1 });
    return result;
}

constexpr std::vector<RowCol> GetDirectNeighbors(const RowCol& pos, const RowCol& max, const RowCol& min = { 0, 0 }) {
    std::vector<RowCol> result;
    if (pos.Row > min.Row) result.push_back({ pos.Row - 1, pos.Col });
    if (pos.Col > min.Col) result.push_back({ pos.Row, pos.Col - 1 });
    if (pos.Row < max.Row) result.push_back({ pos.Row + 1, pos.Col });
    if (pos.Col < max.Col) result.push_back({ pos.Row, pos.Col + 1 });
    return result;
}

template<typename T>
constexpr std::vector<Vec3<T>> GetDirectNeighbors(const Vec3<T>& pos, const Vec3<T>& max, const Vec3<T>& min = { 0, 0, 0 }) {
    std::vector<Vec3<T>> result;
    if (pos.X > min.X) result.push_back({ pos.X - 1, pos.Y, pos.Z });
    if (pos.Y > min.Y) result.push_back({ pos.X, pos.Y - 1, pos.Z });
    if (pos.Z > min.Z) result.push_back({ pos.X, pos.Y, pos.Z - 1 });

    if (pos.X < max.X) result.push_back({ pos.X + 1, pos.Y, pos.Z });
    if (pos.Y < max.Y) result.push_back({ pos.X, pos.Y + 1, pos.Z });
    if (pos.Z < max.Z) result.push_back({ pos.X, pos.Y, pos.Z + 1 });

    return result;
}

constexpr std::vector<Coord> GetAllNeighbors(const Coord& pos, const Coord& max, const Coord& min = { 0, 0 }) {
    std::vector<Coord> result = GetDirectNeighbors(pos, max, min);

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

constexpr std::vector<UCoord> GetAllNeighbors(const UCoord& pos, const UCoord& max, const UCoord& min = { 0, 0 }) {
    std::vector<UCoord> result = GetDirectNeighbors(pos, max, min);

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

constexpr std::vector<RowCol> GetAllNeighbors(const RowCol& pos, const RowCol& max, const RowCol& min = { 0, 0 }) {
    std::vector<RowCol> result = GetDirectNeighbors(pos, max, min);

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

template<typename T>
constexpr std::vector<Vec3<T>> GetDirectNeighbors(const Vec3<T>& pos) {
    std::vector<Vec3<T>> result;
    result.push_back({ pos.X - 1, pos.Y, pos.Z });
    result.push_back({ pos.X + 1, pos.Y, pos.Z });
    result.push_back({ pos.X, pos.Y - 1, pos.Z });
    result.push_back({ pos.X, pos.Y + 1, pos.Z });
    result.push_back({ pos.X, pos.Y, pos.Z - 1 });
    result.push_back({ pos.X, pos.Y, pos.Z + 1 });
    return result;
}

template<typename T>
constexpr std::vector<Vec3<T>> GetAllNeighbors(const Vec3<T>& pos) {
    std::vector<Vec3<T>> result;
    for (auto x = -1; x < 2; x++) {
        for (auto y = -1; y < 2; y++) {
            for (auto z = -1; z < 2; z++) {
                result.push_back({ pos.X + x, pos.Y + y, pos.Z + z });
            }
        }
    }

    result.erase(std::remove(result.begin(), result.end(), pos));
    return result;
}

template<typename T>
constexpr std::vector<Vec4<T>> GetAllNeighbors(const Vec4<T>& pos) {
    std::vector<Vec4<T>> result;
    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            for (int z = -1; z < 2; z++) {
                for (int w = -1; w < 2; w++) {
                    result.push_back({ pos.X + x, pos.Y + y, pos.Z + z, pos.W + w });
                }
            }
        }
    }

    result.erase(std::remove(result.begin(), result.end(), pos));
    return result;
}

template<typename T>
constexpr void GetLimits(const auto& collection, Vec3<T>& outMinValues, Vec3<T>& outMaxValues) {
    T minX = std::numeric_limits<T>::max();
    T minY = minX;
    T minZ = minX;
    T maxX = std::numeric_limits<T>::min();
    T maxY = maxX;
    T maxZ = maxX;

    for (const auto& point : collection) {
        minX = std::min(minX, point.X);
        minY = std::min(minY, point.Y);
        minZ = std::min(minZ, point.Z);
        maxX = std::max(maxX, point.X);
        maxY = std::max(maxY, point.Y);
        maxZ = std::max(maxZ, point.Z);
    }

    outMinValues = { minX, minY, minZ };
    outMaxValues = { maxX, maxY, maxZ };
}

template<typename T>
constexpr void GetLimits(const auto& collection, Vec4<T>& outMinValues, Vec4<T>& outMaxValues) {
    T minX = std::numeric_limits<T>::max();
    T minY = minX;
    T minZ = minX;
    T minW = minX;
    T maxX = std::numeric_limits<T>::min();
    T maxY = maxX;
    T maxZ = maxX;
    T maxW = maxX;

    for (const auto& point : collection) {
        minX = std::min(minX, point.X);
        minY = std::min(minY, point.Y);
        minZ = std::min(minZ, point.Z);
        minW = std::min(minW, point.W);

        maxX = std::max(maxX, point.X);
        maxY = std::max(maxY, point.Y);
        maxZ = std::max(maxZ, point.Z);
        maxW = std::max(maxW, point.W);
    }

    outMinValues = { minX, minY, minZ, minW };
    outMaxValues = { maxX, maxY, maxZ, maxW };
}

template<typename T>
constexpr Vec3<T> DotProduct(const Vec3<T>& pos, const std::array<std::array<s32, 3>, 3>& matrix) {
    Vec3<T> result;
    result.X = ((pos.X * matrix[0][0]) + (pos.Y * matrix[0][1]) + (pos.Z * matrix[0][2]));
    result.Y = ((pos.X * matrix[1][0]) + (pos.Y * matrix[1][1]) + (pos.Z * matrix[1][2]));
    result.Z = ((pos.X * matrix[2][0]) + (pos.Y * matrix[2][1]) + (pos.Z * matrix[2][2]));
    return result;
}

namespace Constexpr {
    enum struct Orientation { Linear, Clockwise, CounterClockwise };

    template<typename Point>
    constexpr Orientation GetOrientation(Point p, Point q, Point r) {
        static_assert(std::is_signed_v<decltype(q.X)>, "GetOrientation requires signed types");

        auto val = (q.Y - p.Y) * (r.X - q.X) - (q.X - p.X) * (r.Y - q.Y);
        return val == 0 ? Orientation::Linear :
            val < 0 ? Orientation::CounterClockwise : Orientation::Clockwise;
    }

    template<typename Point>
    constexpr bool IsOnSegment(Point p, Point q, Point r) {
        return q.X <= std::max(p.X, r.X) && q.X >= std::min(p.X, r.X) &&
            q.Y <= std::max(p.Y, r.Y) && q.Y >= std::min(p.Y, r.Y);
    }

    template<typename Point>
    constexpr bool DoIntersect(const Point& start1, const Point& end1, const Point& start2, const Point& end2) {
        auto o1 = GetOrientation(start1, end1, start2);
        auto o2 = GetOrientation(start1, end1, end2);
        auto o3 = GetOrientation(start2, end2, start1);
        auto o4 = GetOrientation(start2, end2, end1);

        if (o1 != o2 && o3 != o4) return true;
        if (o1 == Orientation::Linear && IsOnSegment(start1, start2, end1)) return true;
        if (o2 == Orientation::Linear && IsOnSegment(start1, end2, end1)) return true;
        if (o3 == Orientation::Linear && IsOnSegment(start2, start1, end2)) return true;
        if (o4 == Orientation::Linear && IsOnSegment(start2, end1, end2)) return true;
        return false;
    }

    template<typename Point>
    constexpr bool DoIntersect(const Point& start, const Point& end, const Point& point) {
        auto dist = [](const Point& a, const Point& b) -> double {
            double x = static_cast<double>(a.X < b.X ? (b.X - a.X) : (a.X - b.X));
            double y = static_cast<double>(a.Y < b.Y ? (b.Y - a.Y) : (a.Y - b.Y));

            return Constexpr::Sqrt((x * x) + (y * y));
        };

        return Constexpr::Abs(dist(start, end) - ((dist(start, point) + dist(end, point)))) < 0.000001;
    }

    template<typename Matrix, typename Func>
    constexpr void ApplyToMatrixIndex(Matrix& matrix, Func func) {
        for (size_t row = 0; row < matrix.size(); row++) {
            for (size_t col = 0; col < matrix[row].size(); col++) {
                func(row, col);
            }
        }
    }

    template<typename Matrix, typename Func>
    constexpr void ApplyToMatrixValue(Matrix& matrix, Func func) {
        for (auto& row : matrix) {
            for (auto& val : row) {
                func(val);
            }
        }
    }

    constexpr void ApplyToMatrixValueConst(const auto& matrix, auto func) {
        for (const auto& row : matrix) {
            for (const auto& value : row) {
                func(value);
            }
        }
    }
    
    template<typename Matrix, typename Func>
    constexpr void ApplyToMatrix3D(Matrix& matrix, Func func) {
        for (size_t row = 0; row < matrix.size(); row++) {
            for (size_t col = 0; col < matrix[row].size(); col++) {
                for (size_t dep = 0; dep < matrix[row][col].size(); dep++) {
                    func(row, col, dep);
                }
            }
        }
    }

    template<typename Matrix>
    [[nodiscard]] constexpr Matrix FlipX(const Matrix& matrix) {
        auto result = matrix;
        size_t n = matrix.size() - 1;
        ApplyToMatrixIndex(matrix, [&](size_t row, size_t col) {
            result[row][col] = matrix[n - row][col];
            });
        return result;
    }

    template<typename Matrix>
    [[nodiscard]] constexpr Matrix FlipY(const Matrix& matrix) {
        auto result = matrix;
        size_t n = matrix[0].size() - 1;
        ApplyToMatrixIndex(matrix, [&](size_t row, size_t col) {
            result[row][col] = matrix[row][n - col];
            });

        return result;
    }

    template<typename Matrix>
    [[nodiscard]] constexpr Matrix Rotate(const Matrix& matrix) {
        auto result = matrix;
        size_t n = matrix.size() - 1;
        ApplyToMatrixIndex(matrix, [&](size_t row, size_t col) {
            result[row][col] = matrix[n - col][row];
            });

        return result;
    }

    template<typename Point>
    [[nodiscard]] constexpr Point GetSlope(const Point& start, const Point& end) {
        auto relative = Point{ end.X - start.X, end.Y - start.Y };
        auto gcd = Constexpr::FindGcd(Constexpr::Abs(relative.X), Constexpr::Abs(relative.Y));
        return { relative.X / gcd, relative.Y / gcd };
    }

    namespace ConstexprGeometryTests {
        bool RunTests();
    }
}