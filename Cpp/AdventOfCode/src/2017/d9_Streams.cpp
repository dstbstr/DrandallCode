#include "2017/d9_Streams.h"

SOLUTION(2017, 9) {
    constexpr void RemoveExtraNots(std::string & str) {
        Constexpr::ReplaceAll(str, "!!", "");
    }

    constexpr u32 RemoveGarbage(std::string & str) {
        size_t offset = 0;
        size_t toSubtract = std::count(str.begin(), str.end(), '!') * 2;
        size_t initial = str.size();
        auto start = str.npos;
        auto end = str.npos;
        while ((start = str.find('<')) != str.npos) {
            end = str.find('>');
            while (str[end - 1] == '!') {
                offset = end + 1;
                end = str.find('>', offset);
            }
            str.replace(str.begin() + start, str.begin() + end + 1, "");
            toSubtract += 2;
        }

        return static_cast<u32>(initial - str.size() - toSubtract);
    }

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

    PART_ONE() {
        auto running = lines[0];
        RemoveExtraNots(running);
        RemoveGarbage(running);
        return Constexpr::ToString(CalculateScore(running));
    }

    PART_TWO() {
        auto running = lines[0];
        RemoveExtraNots(running);
        return Constexpr::ToString(RemoveGarbage(running));
    }

    constexpr bool TestRemoveNots(const std::string& input, const std::string& expected) {
        std::string actual = input;
        RemoveExtraNots(actual);
        return actual == expected;
    }

    constexpr bool TestRemoveGarbage(const std::string& input, const std::string& expected, u32 expectedCount) {
        std::string running = input;
        auto removed = RemoveGarbage(running);
        if (running != expected) return false;
        return removed == expectedCount;
    }

    TESTS() {
        static_assert(TestRemoveNots("!!", ""));
        static_assert(TestRemoveNots("!", "!"));
        static_assert(TestRemoveNots("!!!", "!"));
        static_assert(TestRemoveNots("!!!!", ""));
        
        static_assert(TestRemoveGarbage("<>", "", 0));
        static_assert(TestRemoveGarbage("<random characters>", "", 17));
        static_assert(TestRemoveGarbage("<<<<>", "", 3));
        static_assert(TestRemoveGarbage("<{!>}>", "", 2));
        static_assert(TestRemoveGarbage("{{<ab>},{<ab>},{<ab>},{<ab>}}", "{{},{},{},{}}", 8));
        static_assert(TestRemoveGarbage("{<{},{},{{}}>}", "{}", 10));
        static_assert(TestRemoveGarbage("{{<!>},{<!>},{<!>},{<a>}}", "{{}}", 13));
        static_assert(TestRemoveGarbage("{<>,<>,<>}", "{,,}", 0));
        
        static_assert(CalculateScore("{}") == 1);
        static_assert(CalculateScore("{{{}}}") == 6);
        static_assert(CalculateScore("{{},{}}") == 5);
        static_assert(CalculateScore("{{{},{},{{}}}}") == 16);
        
        static_assert(PartTwo({ "<>" }) == "0");
        static_assert(PartTwo({ "<random characters>" }) == "17");
        static_assert(PartTwo({ "<<<<>" }) == "3");
        static_assert(PartTwo({ "<{!>}>" }) == "2");
        static_assert(PartTwo({ "<!!>" }) == "0");
        static_assert(PartTwo({ "<!!!>>" }) == "0");
        static_assert(PartTwo({ "<{o\"i!a,<{i<a>" }) == "10");

        return true;
    }
}