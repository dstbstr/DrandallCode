#pragma once

#include <string>
#include <regex>
#include <utility>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <exception>
#include <functional>
#include <iostream>
#include <numeric>


#include "Platform/Types.h"
#include "Utilities/StringUtils.h"
#include "Utilities/Format.h"

#include "Constexpr/ConstexprAlgorithms.h"
#include "Constexpr/ConstexprBits.h"
#include "Constexpr/ConstexprCollections.h"
#include "Constexpr/ConstexprGeometry.h"
#include "Constexpr/ConstexprMath.h"
#include "Constexpr/ConstexprStrUtils.h"

using namespace std::string_view_literals;
using namespace std::string_literals;

enum Direction { Up, Right, Down, Left };

constexpr std::string ToString(Direction dir) {
    switch (dir) {
    case Up: return "Up"; break;
    case Right: return "Right"; break;
    case Down: return "Down"; break;
    case Left: return "Left"; break;
    default: return "Unknown"; break;
    }
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

constexpr void Move(Coord& pos, Direction facing) {
    switch (facing) {
    case Up: pos.Y--; return;
    case Down: pos.Y++; return;
    case Left: pos.X--; return;
    case Right: pos.X++; return;
    default:break;
    }
}

constexpr void Move(UCoord& pos, Direction facing) {
    switch (facing) {
    case Up: pos.Y--; return;
    case Down: pos.Y++; return;
    case Left: pos.X--; return;
    case Right: pos.X++; return;
    default:break;
    }
}

template<typename Key, typename Value, typename Hash = std::hash<Key>>
void PrintMap(const std::unordered_map<Key, Value, Hash>& map, std::function<char(const Key&, const Value&)> iconFn, char defaultIcon) {
    Key min, max;
    GetLimitsFromMap(map, min, max);
    for (auto row = min.Y; row <= max.Y; row++) {
        for (auto col = min.X; col <= max.X; col++) {
            Key key = { col, row };
            char icon = defaultIcon;
            if (map.find(key) != map.end()) {
                Value val = map.at(key);
                icon = iconFn(key, val);
            }
            std::cout << icon;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

constexpr std::vector<std::vector<std::string>> SplitInputIntoGroups(const std::vector<std::string>& lines) {
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> group;
    for (const auto& line : lines) {
        if (line.empty()) {
            result.push_back(group);
            group.clear();
        }
        else {
            group.push_back(line);
        }
    }

    result.push_back(group);
    return result;
}

template<typename T>
constexpr std::vector<T> ParseLineAsNumbers(const std::string& line, std::string delimiter = ",") {
    std::vector<T> result;
    auto split = Constexpr::Split(line, delimiter);
    for (const auto& s : split) {
        T num;
        Constexpr::ParseNumber(s, num);
        result.push_back(num);
    }
    return result;
}

template<typename T>
constexpr std::vector<T> ParseLines(const std::vector<std::string>& lines, auto parseFunc) {
    std::vector<T> result;
    for (const auto& line : lines) {
        result.push_back(parseFunc(line));
    }
    return result;
}

constexpr auto RunAllReturnMax(const auto& collection, auto func, auto... args) {
    //define best's type as whatever func returns when given an element of the collection, and the extra args
    decltype(func(std::declval<decltype(collection.back())>(), args...)) best = {};

    for (const auto& element : collection) {
        best = std::max(best, func(element, args...));
    }

    return best;
}

constexpr auto RunAllReturnMin(const auto& collection, auto func, auto... args) {
    decltype(func(std::declval<decltype(collection.back())>(), args...)) best = {};

    for (const auto& element : collection) {
        best = std::min(best, func(element, args...));
    }

    return best;
}

using SolutionFunc = std::function<std::string(const std::vector<std::string>&)>;
std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, SolutionFunc>>>& GetSolutions();
std::unordered_map<size_t, std::unordered_map<size_t, std::function<bool()>>>& GetTests();

struct SolutionRegistrar {
    SolutionRegistrar(size_t year, size_t day, size_t part, SolutionFunc solution) {
        GetSolutions()[year][day][part] = solution;
    }
};

struct TestRegistrar {
    TestRegistrar(size_t year, size_t day, std::function<bool()> testFunc) {
        GetTests()[year][day] = testFunc;
    }
};


#define SOLUTION(_year, _day) namespace Year##_year##Day##_day
#define PART_ONE() constexpr std::string PartOne(const std::vector<std::string>& lines)
#define PART_TWO() constexpr std::string PartTwo(const std::vector<std::string>& lines)
#define TESTS() constexpr bool Tests()

#define DECLARE_SOLUTION(_year, _day) \
    SOLUTION(_year, _day) { \
    PART_ONE(); \
    PART_TWO(); \
    TESTS(); \
    inline SolutionRegistrar reg_PartOne{_year, _day, 1, PartOne}; \
    inline SolutionRegistrar reg_PartTwo{_year, _day, 2, PartTwo}; \
    inline TestRegistrar reg_Tests{_year, _day, Tests}; \
    }

/*
#define PART_ONE(_year, _day) \
    constexpr std::string PartOne##_year##_##_day(const std::vector<std::string>&); \
    SolutionRegistrar reg##_year##_##_day##_1{_year, _day, 1, PartOne##_year##_##_day}; \
    constexpr std::string PartOne##_year##_##_day(const std::vector<std::string>& lines)

#define PART_TWO(_year, _day) \
    constexpr std::string PartTwo##_year##_##_day(const std::vector<std::string>&); \
    SolutionRegistrar reg##_year##_##_day##_2{ _year, _day, 2, PartTwo##_year##_##_day }; \
    constexpr std::string PartTwo##_year##_##_day(const std::vector<std::string>& lines)

#define TESTS(_year, _day) \
    constexpr bool Tests##_year##_##_day(); \
    TestRegistrar reg##_year##_##_day{ _year, _day, Tests##_year##_##_day }; \
    constexpr bool Tests##_year##_##_day()
*/