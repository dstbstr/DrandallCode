#include "2017/d9_Streams.h"

SOLUTION(2017, 9) {
    constexpr void RemoveExtraNots(std::string & str) {
        while (str.find("!!") != str.npos) {
            auto where = str.find("!!");
            str.replace(str.begin() + where, str.begin() + where + 2, "");
        }
    }

    constexpr bool TestRemoveNots(const std::string & input, const std::string & expected) {
        std::string actual = input;
        RemoveExtraNots(actual);
        return actual == expected;
    }

    static_assert(TestRemoveNots("!!", ""));
    static_assert(TestRemoveNots("!", "!"));
    static_assert(TestRemoveNots("!!!", "!"));
    static_assert(TestRemoveNots("!!!!", ""));

    constexpr u32 RemoveGarbage(std::string & str) {
        size_t offset = 0;
        size_t toSubtract = std::count(str.begin(), str.end(), '!') * 2;
        size_t initial = str.size();
        while (str.find('<') != str.npos) {
            auto start = str.find('<');
            auto end = str.find('>');
            while (str[end - 1] == '!') {
                offset = end + 1;
                end = str.find('>', offset);
            }
            str.replace(str.begin() + start, str.begin() + end + 1, "");
            toSubtract += 2;
        }

        return static_cast<u32>(initial - str.size() - toSubtract);
    }

    constexpr bool TestRemoveGarbage(const std::string & input, const std::string & expected, u32 expectedCount) {
        std::string running = input;
        auto removed = RemoveGarbage(running);
        if (running != expected) return false;
        return removed == expectedCount;
    }

    static_assert(TestRemoveGarbage("<>", "", 0));
    static_assert(TestRemoveGarbage("<random characters>", "", 17));
    static_assert(TestRemoveGarbage("<<<<>", "", 3));
    static_assert(TestRemoveGarbage("<{!>}>", "", 2));
    static_assert(TestRemoveGarbage("{{<ab>},{<ab>},{<ab>},{<ab>}}", "{{},{},{},{}}", 8));
    static_assert(TestRemoveGarbage("{<{},{},{{}}>}", "{}", 10));
    static_assert(TestRemoveGarbage("{{<!>},{<!>},{<!>},{<a>}}", "{{}}", 13));
    static_assert(TestRemoveGarbage("{<>,<>,<>}", "{,,}", 0));

    constexpr u32 CalculateScore(const std::string & str) {
        u32 depth = 0;
        u32 result = 0;
        for (auto c : str) {
            if (c == '{') {
                depth++;
            }
            else if (c == '}') {
                result += depth;
                depth--;
            }
        }

        return result;
    }

    static_assert(CalculateScore("{}") == 1);
    static_assert(CalculateScore("{{{}}}") == 6);
    static_assert(CalculateScore("{{},{}}") == 5);
    static_assert(CalculateScore("{{{},{},{{}}}}") == 16);

    auto Part1(const std::string & line) {
        auto running = line;
        RemoveExtraNots(running);
        RemoveGarbage(running);
        return CalculateScore(running);
    }

    auto Part2(const std::string & line) {
        auto running = line;
        RemoveExtraNots(running);
        return RemoveGarbage(running);
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines[0]));
        return Constexpr::ToString(Part2(lines[0]));
    }

    bool RunTests() {
        if (Part2("<>") != 0) return false;
        if (Part2("<random characters>") != 17) return false;
        if (Part2("<<<<>") != 3) return false;
        if (Part2("<{!>}>") != 2) return false;
        if (Part2("<!!>") != 0) return false;
        if (Part2("<!!!>>") != 0) return false;
        if (Part2("<{o\"i!a,<{i<a>") != 10) return false;
        /*
        if(!TestRemoveGarbage("<>", "")) return false;
        if(!TestRemoveGarbage("<random characters>", "")) return false;
        if(!TestRemoveGarbage("<<<<>", "")) return false;
        if(!TestRemoveGarbage("<!>}>", "")) return false;
        if(!TestRemoveGarbage("<!!>", "")) return false;
        */
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