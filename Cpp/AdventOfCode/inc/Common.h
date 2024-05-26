#pragma once

#include "Core/Platform/Types.h"
#include "Core/Utilities/StringUtils.h"
#include "Core/Utilities/Format.h"

#include "Core/Constexpr/ConstexprAlgorithms.h"
#include "Core/Constexpr/ConstexprBits.h"
#include "Core/Constexpr/ConstexprGeometry.h"
#include "Core/Constexpr/ConstexprCollections.h"
#include "Core/Constexpr/ConstexprMath.h"
#include "Core/Constexpr/ConstexprMatrix.h"
#include "Core/Constexpr/ConstexprStrUtils.h"

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

template<typename Key, typename Value, size_t Capacity, typename Hash = Constexpr::Hasher<Key>>
constexpr void PrintMap(const Constexpr::BigMap<Key, Value, Capacity, Hash>& map, char exists, char missing) {
	Key min, max;
	GetLimitsFromMap(map, min, max);
	for (auto row = min.Y; row <= max.Y; row++) {
		std::string currentLine;
		for (auto col = min.X; col <= max.X; col++) {
			Key key = { col, row };
			if (row != 0 || col != 0) {
				currentLine.push_back(map.contains(key) ? exists : missing);
			}
			else {
				currentLine.push_back('O');
			}
		}
		GET_LOGS().push_back(currentLine);
	}
}

constexpr std::vector<std::vector<std::string>> SplitInputIntoGroups(const auto& lines) {
	std::vector<std::vector<std::string>> result;
	std::vector<std::string> group;
	for (const auto& line : lines) {
		if (line.empty()) {
			result.push_back(group);
			group.clear();
		}
		else {
			if constexpr (std::is_same_v<decltype(line), const std::string&>) {
				group.push_back(line);
			}
			else {
				group.push_back(Constexpr::ToString(line));
			}
		}
	}

	result.push_back(group);
	return result;
}

template<size_t LineCount>
constexpr std::array<std::string_view, LineCount> SplitInputIntoLines(const std::string_view& input) {
	std::array<std::string_view, LineCount> result{};
	auto s = Constexpr::Split(input, "\n", true);
	for (size_t i = 0; i < LineCount && i < s.size(); i++) {
		result[i] = s[i];
	}
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
constexpr std::vector<T> ParseLineAsNumbers(std::string_view line, std::string_view delimiter = ",") {
	std::vector<T> result;
	auto split = Constexpr::Split(line, delimiter);
	for (auto s : split) {
		T num;
		Constexpr::ParseNumber(s, num);
		result.push_back(num);
	}
	return result;
}

template<typename T>
constexpr std::vector<T> ParseLinesAsNumbers(const auto& lines) {
	std::vector<T> result;
	for (const auto& s : lines) {
		T num;
		Constexpr::ParseNumber(s, num);
		result.push_back(num);
	}
	return result;
}

//Returns a vector of whatever parseFunc returns, when given a string and any extra args
constexpr auto ParseLines(const auto& lines, auto parseFunc, auto&... args) {
	using T = decltype(parseFunc("", args...));
	std::vector<T> result;
	for (const auto& line : lines) {
		result.push_back(parseFunc(line, args...));
	}
	return result;
}

constexpr auto ParseLinesWithIndex(const auto& lines, auto parseFunc) {
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
	using Ret = decltype(func(std::declval<decltype(collection.back())>(), args...));

	auto best = std::numeric_limits<Ret>::max();
	for (const auto& element : collection) {
		best = std::min(best, func(element, args...));
	}

	return best;
}

constexpr std::vector<std::string> CopyToVector(const auto& lines) {
	std::vector<std::string> result;
	std::transform(lines.begin(), lines.end(), std::back_inserter(result), [](std::string_view sv) { return std::string(sv); });
	return result;
}

constexpr size_t Count2D(const auto& collection, auto pred) {
	return std::accumulate(collection.begin(), collection.end(), 0ull, [pred](size_t previous, const auto& row) {
		return previous + std::count_if(row.begin(), row.end(), pred);
		});
}

using SolutionFunc = std::function<std::string(const std::vector<std::string>&)>;
//using SolutionFunc = std::function<std::string()>;
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
//#define PART_ONE() constexpr std::string PartOne()
//#define PART_TWO() constexpr std::string PartTwo()
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
#define DECLARE_SOLUTION(_year, _day, _input) \
	SOLUTION(_year, _day) { \
	constexpr std::string_view Line = _input; \
	constexpr auto lines = SplitInputIntoLines<std::count(Line.begin(), Line.end(), '\n') + 1>(Line); \
	PART_ONE(); \
	PART_TWO(); \
	TESTS(); \
	inline SolutionRegistrar reg_PartOne{_year, _day, 1, PartOne}; \
	inline SolutionRegistrar reg_PartTwo{_year, _day, 2, PartTwo}; \
	inline TestRegistrar reg_Tests{_year, _day, Tests}; \
	}
*/