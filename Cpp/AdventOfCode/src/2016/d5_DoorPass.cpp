#include "2016/d5_DoorPass.h"
#include "Algorithms/md5Constexpr.h"

SOLUTION(2016, 5) {

	constexpr std::string PartOneImpl(std::string_view key, const std::vector<u64>&hints) {
		std::string prefix = std::string(key);
		if (hints.empty()) {
			u64 index = 0;
			std::string result = "";
			while (result.size() < 8) {
				std::string attempt = prefix + ToString(index);
				auto hex = ToHexLower(md5::compute(attempt.c_str()));
				if (hex.substr(0, 5) == "00000") {
					result += hex[5];
				}
				index++;
			}
			return result;
		}
		else {
			std::string result = "";
			for (auto hint : hints) {
				std::string attempt = prefix + ToString(hint);
				auto hex = ToHexLower(md5::compute(attempt.c_str()));
				if (hex.substr(0, 5) != "00000") {
					throw "Bad hint";
				}
				result += hex[5];
			}

			return result;
		}
	}

	PART_ONE() {
		std::vector<u64> hints = {
			1739529,
			1910966,
			1997199,
			2854555,
			2963716,
			3237361,
			4063427,
			7777889
		};
		return PartOneImpl(lines[0], hints);
	}

	constexpr std::string PartTwoImpl(std::string_view key, const std::vector<u64>& hints) {
		std::string result = "________";
		std::string prefix = std::string(key);

		if (hints.empty()) {
			u64 index = 0;

			int totalFound = 0;
			while (totalFound < 8) {
				std::string attempt = prefix + ToString(index);
				auto hex = ToHexLower(md5::compute(attempt.c_str()));
				if (hex.substr(0, 5) == "00000" && hex[5] < '8') {
					auto pos = hex[5] - '0';
					if (result[pos] == '_') {
						result[pos] = hex[6];
						totalFound++;
					}
				}
				index++;
			}
			
		}
		else {
			for (const auto hint : hints) {
				std::string attempt = prefix + ToString(hint);
				auto hex = ToHexLower(md5::compute(attempt.c_str()));
				if (hex.substr(0, 5) != "00000" || hex[5] >= '8') throw "Bad hint";

				auto pos = hex[5] - '0';
				if (result[pos] != '_') throw "Bad Hint";
				result[pos] = hex[6];
			}
		}

		return result;
	}

	PART_TWO() {
		std::vector<u64> hints = {
			1910966,
			1997199,
			2963716,
			3237361,
			7777889,
			12850790,
			12942170,
			25651067
		};
		return PartTwoImpl(lines[0], hints);
	}

	TESTS() {
		std::vector<u64> hints = {
			3231929,
			5017308,
			5278568,
			5357525,
			5708769,
			6082117,
			8036669,
			8605828
		};

		if (PartOneImpl("abc", hints) != "18f47a30") return false;
		return true;
	}
}