#include "2022/d4_ElfCleaning.h"

SOLUTION(2022, 4) {
    struct Section {
        u32 Start;
        u32 End;
    };

    constexpr bool FullyContains(Section lhs, Section rhs) {
        if (lhs.Start == rhs.Start) {
            return true;
        }
        if (lhs.Start < rhs.Start) {
            return rhs.Start > lhs.Start && rhs.End <= lhs.End;
        }
        else {
            return lhs.Start > rhs.Start && lhs.End <= rhs.End;
        }
    }

    static_assert(FullyContains({ 6, 6 }, { 4, 6 }));
    static_assert(FullyContains({ 4, 6 }, { 6, 6 }));
    static_assert(FullyContains({ 2, 8 }, { 3, 7 }));
    static_assert(FullyContains({ 3, 7 }, { 2, 8 }));
    static_assert(!FullyContains({ 2, 4 }, { 6, 8 }));
    static_assert(!FullyContains({ 2, 6 }, { 4, 8 }));
    static_assert(FullyContains({ 55, 55 }, { 55, 83 }));

    constexpr bool Overlap(Section lhs, Section rhs) {
        if (lhs.Start == rhs.Start || lhs.End == rhs.End) {
            return true;
        }
        if (lhs.Start < rhs.Start) {
            return lhs.End >= rhs.Start;
        }
        else {
            return rhs.End >= lhs.Start;
        }
    }

    static_assert(!Overlap({ 2, 4 }, { 6, 8 }));
    static_assert(!Overlap({ 2, 3 }, { 4, 5 }));
    static_assert(Overlap({ 5, 7 }, { 7, 9 }));
    static_assert(Overlap({ 2, 8 }, { 3, 7 }));
    static_assert(Overlap({ 6, 6 }, { 4, 6 }));
    static_assert(Overlap({ 2, 6 }, { 4, 8 }));
    static_assert(Overlap({ 55, 55 }, { 55, 83 }));


    u32 RunImpl(const std::vector<std::string>&lines, std::function<bool(Section, Section)> func) {
        u32 result = 0;
        for (const auto& line : lines) {
            auto firstSplit = StrUtil::Split(line, ",");

            auto lhsStr = StrUtil::Split(firstSplit[0], "-");
            Section lhs;
            Constexpr::ParseNumber(lhsStr[0], lhs.Start);
            Constexpr::ParseNumber(lhsStr[1], lhs.End);

            auto rhsStr = StrUtil::Split(firstSplit[1], "-");
            Section rhs;
            Constexpr::ParseNumber(rhsStr[0], rhs.Start);
            Constexpr::ParseNumber(rhsStr[1], rhs.End);

            if (func(lhs, rhs)) {
                result++;
            }
        }

        return result;

    }

    u32 RunFirst(const std::vector<std::string>&lines) {
        return RunImpl(lines, FullyContains);
    }

    u32 Run(const std::vector<std::string>&lines) {
        return RunImpl(lines, Overlap);
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