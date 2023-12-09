#include "2018/d2_BoxId.h"
SOLUTION(2018, 2) {
    constexpr void HasTwosAndThrees(std::string_view line, bool& outHasTwos, bool& outHasThrees) {
        std::array<u8, 26> counts{};

        outHasTwos = false;
        outHasThrees = false;
        for (const auto& c : line) {
            counts[c - 'a']++;
        }
        for (auto i = 0; i < counts.size(); i++) {
            outHasTwos |= counts[i] == 2;
            outHasThrees |= counts[i] == 3;
        }
    }

    constexpr bool OffByOne(std::string_view lhs, std::string_view rhs) {
        u32 deltas = 0;
        for (size_t i = 0; i < lhs.size(); i++) {
            deltas += lhs[i] != rhs[i];
        }
        return deltas == 1;
    }

    PART_ONE() {
        u32 twos = 0;
        u32 threes = 0;
        for (const auto& line : lines) {
            bool hasTwos, hasThrees = false;
            HasTwosAndThrees(line, hasTwos, hasThrees);
            twos += hasTwos;
            threes += hasThrees;
        }

        return Constexpr::ToString(twos * threes);
    }

    PART_TWO() {
        auto copy = lines;
        std::sort(copy.begin(), copy.end());
        for (auto i = 0; i < copy.size() - 1; i++) {
            if (OffByOne(copy[i], copy[i + 1])) {
                return "\n" + std::string(copy[i]) + '\n' + std::string(copy[i + 1]);
            }
        }

        return ""s;
    }

    constexpr bool TestHasTwosAndThrees(const std::string& line, bool expectedTwos, bool expectedThrees) {
        bool hasTwos = false;
        bool hasThrees = false;
        HasTwosAndThrees(line, hasTwos, hasThrees);

        return hasTwos == expectedTwos && hasThrees == expectedThrees;
    }

    TESTS() {
        static_assert(TestHasTwosAndThrees("abcdef", false, false));
        static_assert(TestHasTwosAndThrees("abcd", false, false));
        static_assert(TestHasTwosAndThrees("aacd", true, false));
        static_assert(TestHasTwosAndThrees("aaad", false, true));

        static_assert(OffByOne("abc", "adc"));
        static_assert(!OffByOne("abcd", "efgh"));
        
        //std::vector<std::string> lines = {
        //    "abcdef",
        //    "bababc",
        //    "abbcde",
        //    "abcccd",
        //    "aabcdd",
        //    "abcdee",
        //    "ababab"
        //};
        //
        //if (PartOne(lines) != "12") return false;

        return true;
    }
}