#include "2020/d2_PassCheck.h"

SOLUTION(2020, 2) {
    struct Entry {
        char Letter;
        size_t MinCount;
        size_t MaxCount;
        std::string Word;
    };

    constexpr Entry ParseEntry(const std::string & line) {
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

    static_assert(ParseEntry("1-3 a: abcde").MinCount == 1);
    static_assert(ParseEntry("1-3 a: abcde").MaxCount == 3);
    static_assert(ParseEntry("1-3 a: abcde").Letter == 'a');
    static_assert(ParseEntry("1-3 a: abcde").Word == "abcde");

    constexpr std::vector<Entry> ParseEntries(const std::vector<std::string>&lines) {
        std::vector<Entry> result;
        for (const auto& line : lines) {
            result.push_back(ParseEntry(line));
        }

        return result;
    }

    constexpr bool IsEntryValid(const Entry & entry) {
        size_t letterCount = 0;
        for (auto c : entry.Word) {
            letterCount += c == entry.Letter;
        }
        return letterCount >= entry.MinCount && letterCount <= entry.MaxCount;
    }

    static_assert(IsEntryValid(ParseEntry("1-3 a: abcde")));
    static_assert(IsEntryValid(ParseEntry("2-9 c: ccccccccc")));
    static_assert(!IsEntryValid(ParseEntry("1-3 b: cdefg")));

    auto Part1(const std::vector<std::string>&lines) {
        auto entries = ParseEntries(lines);
        return std::count_if(entries.begin(), entries.end(), IsEntryValid);
    }

    constexpr bool IsEntryValid2(const Entry & entry) {
        return (entry.Word[entry.MinCount - 1] == entry.Letter) != (entry.Word[entry.MaxCount - 1] == entry.Letter);
    }

    static_assert(IsEntryValid2(ParseEntry("1-3 a: abcde")));
    static_assert(!IsEntryValid2(ParseEntry("2-9 c: ccccccccc")));
    static_assert(!IsEntryValid2(ParseEntry("1-3 b: cdefg")));

    auto Part2(const std::vector<std::string>&lines) {
        auto entries = ParseEntries(lines);
        return std::count_if(entries.begin(), entries.end(), IsEntryValid2);
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
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