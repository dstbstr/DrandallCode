#pragma once
#include "pch.h"

using namespace std::string_view_literals;
using namespace std::string_literals;

static inline std::vector<std::string> _logs;
constexpr std::vector<std::string>& GET_LOGS() {
	return _logs;
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

constexpr size_t Count2D(const auto& collection, auto pred) {
	return std::accumulate(collection.begin(), collection.end(), 0ull, [pred](size_t previous, const auto& row) {
		return previous + std::count_if(row.begin(), row.end(), pred);
		});
}