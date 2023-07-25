#include "2019/d4_VenusPass.h"

SOLUTION(2019, 4) {
    constexpr bool Matches(u32 val) {
        auto str = Constexpr::ToString(val);

        bool hasPair = false;
        for (auto i = 1; i < str.size(); i++) {
            if (str[i] < str[i - 1]) return false;
            if (str[i] == str[i - 1]) hasPair = true;
        }

        return hasPair;
    }

    constexpr bool ExtendedMatch(u32 val) {
        auto str = Constexpr::ToString(val);
        std::vector<std::string> pairs;

        for (auto i = 0; i < str.size() - 1; i++) {
            if (str[i + 1] < str[i]) return false;
            if (str[i] == str[i + 1]) {
                pairs.push_back(str.substr(i, 2));
            }
        }

        for (const auto& pair : pairs) {
            if (str.find(pair) + 1 == str.find_last_of(pair)) return true;
        }
        return false;
    }

    constexpr std::pair<u32, u32> ParseInput(const std::string& line) {
        auto s = Constexpr::Split(line, "-");
        std::pair<u32, u32> result;
        Constexpr::ParseNumber(s[0], result.first);
        Constexpr::ParseNumber(s[1], result.second);
        return result;
    }

    constexpr u32 Solve(u32 start, u32 end, auto Matcher) {
        u32 result = 0;
        while (start <= end) {
            result += Matcher(start++);
        }

        return result;
    }

    PART_ONE() {
        auto [start, end] = ParseInput(lines[0]);
        return Constexpr::ToString(Solve(start, end, Matches));
    }

    PART_TWO() {
        auto [start, end] = ParseInput(lines[0]);
        return Constexpr::ToString(Solve(start, end, ExtendedMatch));
    }

    TESTS() {
        static_assert(Matches(111111));
        static_assert(!Matches(223450));
        static_assert(!Matches(123456));
 
        static_assert(ExtendedMatch(112233));
        static_assert(!ExtendedMatch(123444));
        static_assert(ExtendedMatch(111122));

        return true;
    }
}