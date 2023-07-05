#include "2018/d2_BoxId.h"
SOLUTION(2018, 2) {

    constexpr void HasTwosAndThrees(const std::string & line, bool& outHasTwos, bool& outHasThrees) {
        std::array<u8, 26> counts{};

        outHasTwos = false;
        outHasThrees = false;
        for (const auto& c : line) {
            counts[c - 'a']++;
        }
        for (auto i = 0; i < counts.size(); i++) {
            if (counts[i] == 2) outHasTwos = true;
            if (counts[i] == 3) outHasThrees = true;
        }
    }

    constexpr bool TestHasTwosAndThrees(const std::string & line, bool expectedTwos, bool expectedThrees) {
        bool hasTwos = false;
        bool hasThrees = false;
        HasTwosAndThrees(line, hasTwos, hasThrees);

        return hasTwos == expectedTwos && hasThrees == expectedThrees;
    }

    static_assert(TestHasTwosAndThrees("abcdef", false, false));
    static_assert(TestHasTwosAndThrees("abcd", false, false));
    static_assert(TestHasTwosAndThrees("aacd", true, false));
    static_assert(TestHasTwosAndThrees("aaad", false, true));

    auto Part1(const std::vector<std::string>&lines) {
        u32 twos = 0;
        u32 threes = 0;
        for (const auto& line : lines) {
            bool hasTwos, hasThrees = false;
            HasTwosAndThrees(line, hasTwos, hasThrees);
            if (hasTwos) twos++;
            if (hasThrees) threes++;
        }

        return twos * threes;
    }

    constexpr bool OffByOne(const std::string & lhs, const std::string & rhs) {
        u32 deltas = 0;
        for (size_t i = 0; i < lhs.size(); i++) {
            deltas += lhs[i] != rhs[i];
        }
        return deltas == 1;
    }

    static_assert(OffByOne("abc", "adc"));
    static_assert(!OffByOne("abcd", "efgh"));

    auto Part2(const std::vector<std::string>&input) {
        std::vector<std::string> lines = input;
        std::sort(lines.begin(), lines.end());
        for (auto i = 0; i < lines.size() - 1; i++) {
            if (OffByOne(lines[i], lines[i + 1])) {
                return lines[i] + '\n' + lines[i + 1];
            }
        }

        return ""s;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Part2(lines);
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "abcdef",
            "bababc",
            "abbcde",
            "abcccd",
            "aabcdd",
            "abcdee",
            "ababab"
        };

        if (Part1(lines) != 12) return false;
        return true;
    }

    PART_ONE() {
        return lines[0];
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}