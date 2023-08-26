#include "2020/d2_PassCheck.h"

SOLUTION(2020, 2) {
    struct Entry {
        char Letter{ ' ' };
        size_t MinCount{ 0 };
        size_t MaxCount{ 0 };
        std::string Word {""};
    };

    constexpr Entry ParseEntry(std::string_view line) {
        Entry entry;
        auto split1 = Constexpr::Split(line, ": ");
        entry.Word = split1[1];

        auto split2 = Constexpr::Split(split1[0], " ");
        entry.Letter = split2[1][0];

        auto split3 = Constexpr::Split(split2[0], "-");
        size_t min, max;
        Constexpr::ParseNumber(split3[0], min);
        Constexpr::ParseNumber(split3[1], max);
        entry.MinCount = min;
        entry.MaxCount = max;

        return entry;
    }

    constexpr bool IsEntryValid(const Entry & entry) {
        size_t letterCount = std::count(entry.Word.begin(), entry.Word.end(), entry.Letter);
        return letterCount >= entry.MinCount && letterCount <= entry.MaxCount;
    }

    constexpr bool IsEntryValid2(const Entry & entry) {
        return (entry.Word[entry.MinCount - 1] == entry.Letter) != (entry.Word[entry.MaxCount - 1] == entry.Letter);
    }

    PART_ONE() {
        auto entries = ParseLines(Lines, ParseEntry);
        return Constexpr::ToString(std::count_if(entries.begin(), entries.end(), IsEntryValid));
    }

    PART_TWO() {
        auto entries = ParseLines(Lines, ParseEntry);
        return Constexpr::ToString(std::count_if(entries.begin(), entries.end(), IsEntryValid2));
    }

    TESTS() {
        static_assert(ParseEntry("1-3 a: abcde").MinCount == 1);
        static_assert(ParseEntry("1-3 a: abcde").MaxCount == 3);
        static_assert(ParseEntry("1-3 a: abcde").Letter == 'a');
        static_assert(ParseEntry("1-3 a: abcde").Word == "abcde");
        
        static_assert(IsEntryValid(ParseEntry("1-3 a: abcde")));
        static_assert(IsEntryValid(ParseEntry("2-9 c: ccccccccc")));
        static_assert(!IsEntryValid(ParseEntry("1-3 b: cdefg")));

        static_assert(IsEntryValid2(ParseEntry("1-3 a: abcde")));
        static_assert(!IsEntryValid2(ParseEntry("2-9 c: ccccccccc")));
        static_assert(!IsEntryValid2(ParseEntry("1-3 b: cdefg")));

        return true;
    }
}