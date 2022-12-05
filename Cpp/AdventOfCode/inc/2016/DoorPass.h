#pragma once

#include "Common.h"
#include "md5Constexpr.h"

#include <iostream>

std::string RunKey(const std::string& key) {
	u64 index = 0;
	std::string result = "";
	while (result.size() < 8) {
		std::string attempt = key + ToString(index);
		auto hex = ToHex(md5::compute(attempt.c_str()));
		if (StrUtil::StartsWith(hex, "00000")) {
			result += hex[5];
		}
		index++;
	}

	std::cout << "\n";
	return result;
}

std::string RunFirst() {
	return RunKey("abbhdwsy");
}

std::string RunScrambledKey(const std::string& key) {
	u64 index = 0;
	std::string result = "________";
	std::cout << "\n";
	int totalFound = 0;
	while (totalFound < 8) {
		std::string attempt = key + ToString(index);
		auto hex = ToHex(md5::compute(attempt.c_str()));
		if (StrUtil::StartsWith(hex, "00000") && hex[5] < '8') {
			auto pos = hex[5] - '0';
			if(result[pos] == '_') {
				result[pos] = hex[6];
				totalFound++;
			}
		}
		index++;
	}

	return result;
}
std::string Run() {
	return RunScrambledKey("abbhdwsy");
}

bool RunTests() {
	if (RunKey("abc") != "18F47A30") return false;
	return true;
}