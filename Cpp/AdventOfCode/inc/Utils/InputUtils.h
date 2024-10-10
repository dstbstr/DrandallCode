#pragma once

#include "Core/Constexpr/ConstexprStrUtils.h"

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
