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
#include "Constexpr/ConstexprGeometry.h"
#include "Constexpr/ConstexprCollections.h"
#include "Constexpr/ConstexprMath.h"
#include "Constexpr/ConstexprStrUtils.h"

using namespace std::string_view_literals;
using namespace std::string_literals;

static inline std::vector<std::string> _logs;
constexpr std::vector<std::string>& GET_LOGS() {
    return _logs;
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
constexpr std::vector<T> ParseLinesAsNumbers(const std::vector<std::string>& lines) {
    std::vector<T> result;
    for (const auto& s : lines) {
        T num;
        Constexpr::ParseNumber(s, num);
        result.push_back(num);
    }
    return result;
}

//Returns a vector of whatever parseFunc returns, when given a string and any extra args
constexpr auto ParseLines(const std::vector<std::string>& lines, auto parseFunc, auto... args) {
    using T = decltype(parseFunc("", args...));
    std::vector<T> result;
    for (const auto& line : lines) {
        result.push_back(parseFunc(line, args...));
    }
    return result;
}

constexpr auto ParseLinesWithIndex(const std::vector<std::string>& lines, auto parseFunc) {
    using T = decltype(parseFunc("", 0));
    std::vector<T> result;
    for (size_t i = 0; i < lines.size(); i++) {
        result.push_back(parseFunc(lines[i], i));
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
