#pragma once
#include <string>
#include <ostream>

#include "Platform/Types.h"
#include "Constexpr/ConstexprMath.h"
#include "Constexpr/ConstexprStrUtils.h"
#include "Constexpr/ConstexprHash.h"

template<typename T>
std::string ToString(T input);

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
            return GenericHash(rc.Row, rc.Col);
        }
    };
}

template<typename T>
struct Vec2 {
    T X{};
    T Y{};

    constexpr Vec2() = default;
    constexpr Vec2(T x, T y) : X(x), Y(y) {}
    constexpr Vec2(std::string_view sv, std::string_view delimiter = ",") {
        auto s = Constexpr::Split(sv, delimiter);
        Constexpr::ParseNumber(s[0], X);
        Constexpr::ParseNumber(s[1], Y);
    }

    static constexpr Vec2<T> FromValue(T val) {
        return Vec2<T>(val, val);
    }

    constexpr bool operator==(const Vec2<T>& v) const = default;

    constexpr bool operator<(const Vec2<T>& v) const {
        return X == v.X ? Y < v.Y : X < v.X;
    }

    constexpr Vec2<T>& operator+=(const Vec2<T>& v) {
        X += v.X;
        Y += v.Y;
        return *this;
    }
    constexpr Vec2<T> operator+(const Vec2<T>& v) const {
        Vec2<T> result{ X, Y };
        return result += v;
    }

    constexpr Vec2<T>& operator-=(const Vec2<T>& v) {
        X -= v.X;
        Y -= v.Y;
        return *this;
    }
    constexpr Vec2<T> operator-(const Vec2<T>& v) const {
        Vec2<T> result{ X, Y };
        return result -= v;
    }
    constexpr Vec2<T>& operator*=(s64 v) {
        X *= v;
        Y *= v;
        return *this;
    }
    constexpr Vec2<T> operator*(s64 v) const {
        Vec2<T> result{ X, Y };
        return result *= v;
    }

    constexpr Vec2<T>& operator/=(const T& s) {
        X /= s;
        Y /= s;
        return *this;
    }
    constexpr Vec2<T>& operator/=(const Vec2<T>& v) {
        X /= v.X;
        Y /= v.Y;
        return *this;
    }
    constexpr Vec2<T> operator/(const T& s) const {
        auto result = *this;
        return result /= s;
    }
    constexpr Vec2<T> operator/(const Vec2<T>& v) const {
        auto result = *this;
        return result /= v;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vec2<T>& v) {
        stream << "{" << v.X << "," << v.Y << "}";
        return stream;
    }

    constexpr bool Between(Vec2<T> min, Vec2<T> max) const {
        return min.X <= X && X <= max.X &&
            min.Y <= Y && Y <= max.Y;
    }
};

template<typename T>
inline std::string ToString(Vec2<T> vec) {
    return "<" + ToString(vec.X) + "," + ToString(vec.Y) + ">";
}

namespace Constexpr {
    template<typename T>
    constexpr std::string ToString(Vec2<T> vec) {
        return "<" + Constexpr::ToString(vec.X) + "," + Constexpr::ToString(vec.Y) + ">";
    }

    template<typename T>
    struct Hasher<Vec2<T>> {
        constexpr size_t operator()(const Vec2<T>& vec) const {
            return GenericHash(vec.X, vec.Y);
        }
    };
}

template<typename T>
struct std::hash<Vec2<T>> {
    std::size_t operator()(const Vec2<T>& v) const {
        return Constexpr::GenericHash(v.X, v.Y);
    }
};

using Coord = Vec2<s64>;
using UCoord = Vec2<u64>;

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

    static constexpr Vec3<T> FromValue(T val) {
        return Vec3<T>(val, val, val);
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

    constexpr bool Between(Vec3<T> min, Vec3<T> max) const {
        return min.X <= X && X <= max.X &&
            min.Y <= Y && Y <= max.Y &&
            min.Z <= Z && Z <= max.Z;
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
            return GenericHash(vec.X, vec.Y, vec.Z);
        }
    };
}

template<typename T>
struct std::hash<Vec3<T>> {
    std::size_t operator()(const Vec3<T>& v) const {
        return Constexpr::GenericHash(v.X, v.Y, v.Z);
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

    static constexpr Vec4<T> FromValue(T val) {
        return Vec4(val, val, val, val);
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

    constexpr bool Between(Vec4<T> min, Vec4<T> max) const {
        return min.X <= X && X <= max.X &&
            min.Y <= Y && Y <= max.Y &&
            min.Z <= Z && Z <= max.Z &&
            min.W <= W && W <= max.W;
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
            return GenericHash(vec.X, vec.Y, vec.Z, vec.W);
        }
    };
}

template<typename T>
struct std::hash<Vec4<T>> {
    std::size_t operator()(const Vec4<T>& v) const {
        return Constexpr::GenericHash(v.X, v.Y, v.Z, v.W);
    }
};

constexpr size_t MDistance(const RowCol& pos) {
    return pos.Row + pos.Col;
}

constexpr size_t MDistance(const RowCol& lhs, const RowCol& rhs) {
    return Constexpr::AbsDistance(lhs.Row, rhs.Row) + Constexpr::AbsDistance(lhs.Col, rhs.Col);
}

template<template<typename> typename Container, typename T>
constexpr size_t MDistance(Container<T> p) {
    using Raw = std::remove_cvref_t<Container<T>>;
    T sum = Constexpr::Abs(p.X) + Constexpr::Abs(p.Y);
    if constexpr (!std::is_same_v<Vec2<T>, Raw>) {
        sum += Constexpr::Abs(p.Z);
        if constexpr (!std::is_same_v<Vec3<T>, Raw>) {
            sum += Constexpr::Abs(p.W);
        }
    }

    return static_cast<size_t>(sum);
}

template<template<typename> typename Container, typename T>
constexpr size_t MDistance(Container<T> a, Container<T> b) {
    using Raw = std::remove_cvref_t<Container<T>>;
    T sum = Constexpr::AbsDistance(a.X, b.X) + Constexpr::AbsDistance(a.Y, b.Y);
    if constexpr (!std::is_same_v<Vec2<T>, Raw>) {
        sum += Constexpr::AbsDistance(a.Z, b.Z);
        if constexpr (!std::is_same_v<Vec3<T>, Raw>) {
            sum += Constexpr::AbsDistance(a.W, b.W);
        }
    }

    return static_cast<size_t>(sum);
}

namespace _Impl {
    template<template<typename> typename Container, typename T>
    constexpr std::vector<Container<T>> GetDeltas() {
        using Raw = std::remove_cvref_t<Container<T>>;

        if constexpr (std::is_same_v<Vec2<T>, Raw>) {
            return { {0, 1}, {1, 0} };
        }
        else if constexpr (std::is_same_v<Vec3<T>, Raw>) {
            return { {0, 0, 1}, {0, 1, 0}, {1, 0, 0} };
        }
        else if constexpr (std::is_same_v<Vec4<T>, Raw>) {
            return { {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 1, 0, 0}, {1, 0, 0, 0} };
        }
        else {
            throw "Wat?";
        }
    }
}

template<template<typename> typename Container, typename T>
constexpr std::vector<Container<T>> GetDirectNeighbors(Container<T> pos) {
    std::vector<Container<T>> result;
    for (auto delta : _Impl::GetDeltas<Container, T>()) {
        result.push_back(pos + delta);
        result.push_back(pos - delta);
    }

    return result;
}

template<template<typename> typename Container, typename T>
constexpr std::vector<Container<T>> GetDirectNeighbors(Container<T> pos, Container<T> max, Container<T> min = {}) {
    std::vector<Container<T>> result;
    Container<T> temp;
    for (auto delta : _Impl::GetDeltas<Container, T>()) {
        temp = pos + delta;
        if (temp.Between(min, max)) result.push_back(temp);

        temp = pos - delta;
        if (temp.Between(min, max)) result.push_back(temp);
    }

    return result;
}

template<template<typename> typename Container, typename T>
constexpr std::vector<Container<T>> GetAllNeighbors(const Container<T>& pos) {
    std::vector<Container<T>> result{};
    for (auto d : _Impl::GetDeltas<Container, T>()) {
        auto size = result.size();
        for (auto i = 0; i < size; i++) {
            result.push_back(result[i] + d);
            result.push_back(result[i] - d);
        }
        result.push_back(pos + d);
        result.push_back(pos - d);
    }

    return result;
}

template<template<typename> typename Container, typename T>
constexpr std::vector<Container<T>> GetAllNeighbors(Container<T> pos, Container<T> max, Container<T> min = {}) {
    std::vector<Container<T>> result;
    Container<T> temp;

    auto AddValid = [&]() {
        if (temp.Between(min, max)) result.push_back(temp);
        };

    for (auto d : _Impl::GetDeltas<Container, T>()) {
        auto size = result.size();
        for (auto i = 0; i < size; i++) {
            auto n = result[i];
            temp = n + d;
            AddValid();

            temp = n - d;
            AddValid();
        }
        temp = pos + d;
        AddValid();

        temp = pos - d;
        AddValid();
    }

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


namespace _Impl {
    template<template<typename> typename Container, typename T>
    constexpr Container<T> InitialMin(Container<T>::FromValue(std::numeric_limits<T>::max()));

    template<template<typename> typename Container, typename T>
    constexpr Container<T> InitialMax(Container<T>::FromValue(std::numeric_limits<T>::min()));

    template<template<typename> typename Container, typename T>
    constexpr auto UpdateMax(Container<T>& max, Container<T> next) {
        max.X = std::max(max.X, next.X);
        max.Y = std::max(max.Y, next.Y);
        using Raw = std::remove_cvref_t<Container<T>>;

        if constexpr (std::is_same_v<Vec2<T>, Raw>) {
            return;
        }
        else {
            max.Z = std::max(max.Z, next.Z);
            if constexpr (std::is_same_v<Vec3<T>, Raw>) {
                return;
            }
            else {
                max.W = std::max(max.W, next.W);
            }
        }
    }

    template<template<typename> typename Container, typename T>
    constexpr auto UpdateMin(Container<T>& min, Container<T> next) {
        min.X = std::min(min.X, next.X);
        min.Y = std::min(min.Y, next.Y);
        using Raw = std::remove_cvref_t<Container<T>>;

        if constexpr (std::is_same_v<Vec2<T>, Raw>) {
            return;
        }
        else {
            min.Z = std::min(min.Z, next.Z);
            if constexpr (std::is_same_v<Vec3<T>, Raw>) {
                return;
            }
            else {
                min.W = std::min(min.W, next.W);
            }
        }
    }
}

template<template<typename> typename Container, typename T>
constexpr void GetLimits(auto start, auto end, Container<T>& outMin, Container<T>& outMax) {
    outMin = _Impl::InitialMin<Container, T>;
    outMax = _Impl::InitialMax<Container, T>;
    while (start != end) {
        _Impl::UpdateMin(outMin, start);
        _Impl::UpdateMax(outMax, start);
        start++;
    }
}

template<template<typename> typename Container, typename T>
constexpr void GetLimits(const auto& collection, Container<T>& outMin, Container<T>& outMax) {
    outMin = _Impl::InitialMin<Container, T>;
    outMax = _Impl::InitialMax<Container, T>;

    for (const auto& v : collection) {
        _Impl::UpdateMax(outMax, v);
        _Impl::UpdateMin(outMin, v);
    }
}

template<template<typename> typename Container, typename T>
constexpr void GetLimitsFromMap(const auto& map, Container<T>& outMin, Container<T>& outMax) {
    outMin = _Impl::InitialMin<Container, T>;
    outMax = _Impl::InitialMax<Container, T>;

    for (auto key : map.GetKeys()) {
        _Impl::UpdateMin(outMin, key);
        _Impl::UpdateMax(outMax, key);
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
constexpr Vec3<T> DotProduct(const Vec3<T>& pos, const std::array<std::array<s32, 3>, 3>& matrix) {
    Vec3<T> result;
    result.X = ((pos.X * matrix[0][0]) + (pos.Y * matrix[0][1]) + (pos.Z * matrix[0][2]));
    result.Y = ((pos.X * matrix[1][0]) + (pos.Y * matrix[1][1]) + (pos.Z * matrix[1][2]));
    result.Z = ((pos.X * matrix[2][0]) + (pos.Y * matrix[2][1]) + (pos.Z * matrix[2][2]));
    return result;
}

template<typename T>
constexpr T DotProduct(Vec3<T> lhs, Vec3<T> rhs) {
    return (lhs.X * rhs.X) + (lhs.Y * rhs.Y) + (lhs.Z * rhs.Z);
}

template<typename T>
constexpr Vec3<T> CrossProduct(Vec3<T> lhs, Vec3<T> rhs) {
    Vec3<T> result;
    result.X = lhs.Y * rhs.Z - lhs.Z * rhs.Y;
    result.Y = lhs.Z * rhs.X - lhs.X * rhs.Z;
    result.Z = lhs.X * rhs.Y - lhs.Y * rhs.X;

    return result;
}

template<typename T>
struct Rect {
    Vec2<T> TopLeft{};
    Vec2<T> BottomRight{};

    constexpr Rect(Vec2<T> topLeft, Vec2<T> bottomRight) : TopLeft(topLeft), BottomRight(bottomRight) {}
    constexpr Rect(Vec2<T> topLeft, T width, T height) : TopLeft(topLeft), BottomRight(topLeft + Vec2<T>{ width, height }) {}

    constexpr bool Contains(const Vec2<T>& p) {
        return TopLeft.X <= p.X && p.X <= BottomRight.X &&
            TopLeft.Y <= p.Y && p.Y <= BottomRight.Y;
    }
};

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

    template<typename T>
    constexpr bool TryFindIntersection(Vec2<T> lhsStart, Vec2<T> lhsEnd, Vec2<T> rhsStart, Vec2<T> rhsEnd, Vec2<T>& outResult) {
        auto Det = [](T a, T b, T c, T d) {
            return a * d - b * c;
            };

        auto leftDet = Det(lhsStart.X, lhsStart.Y, lhsEnd.X, lhsEnd.Y);
        auto rightDet = Det(rhsStart.X, rhsStart.Y, rhsEnd.X, rhsEnd.Y);
        auto dx1 = lhsStart.X - lhsEnd.X;
        auto dx2 = rhsStart.X - rhsEnd.X;
        auto dy1 = lhsStart.Y - lhsEnd.Y;
        auto dy2 = rhsStart.Y - rhsEnd.Y;

        auto xnom = Det(leftDet, dx1, rightDet, dx2);
        auto ynom = Det(leftDet, dy1, rightDet, dy2);
        auto denom = Det(dx1, dy1, dx2, dy2);
        if (Constexpr::Abs(denom) < 0.0000000001) {
            return false;
        }

        outResult = { xnom / denom, ynom / denom };
        return true;
    }

    template<typename T>
    constexpr bool TryFindIntersection(Vec2<T> aPos, Vec2<T> aV, Vec2<T> bPos, Vec2<T> bV, Vec2<T>& outIntersection, T& outATime, T& outBTime) {
        auto detA = (-aV.X * bV.Y) + (bV.X * aV.Y);
        if (Constexpr::Abs(detA) < 0.00000001) return false;
        auto detAt = (aPos.X * bV.Y) + (-bPos.X * bV.Y) + (-aPos.Y * bV.X) + (bPos.Y * bV.X);
        auto detAs = (-aPos.Y * aV.X) + (bPos.Y * aV.X) + (aPos.X * aV.Y) + (-bPos.X * aV.Y);
        outATime = detAt / detA;
        outBTime = detAs / detA;

        outIntersection.X = aPos.X + (aV.X * outATime);
        outIntersection.Y = aPos.Y + (aV.Y * outATime);

        return true;
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