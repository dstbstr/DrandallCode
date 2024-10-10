#pragma once

#include <functional>
#include <unordered_map>
#include <string>

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