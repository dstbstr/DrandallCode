#include "2021/d10_Syntax.h"
#include <stack>

SOLUTION(2021, 10) {

    constexpr size_t GetCorruptScore(char c) {
        switch (c) {
        case ')': return 3;
        case ']': return 57;
        case '}': return 1197;
        case '>': return 25137;
        default: return 0;
        }
    }

    constexpr size_t GetAcScore(char c) {
        switch (c) {
        case ')': return 1;
        case ']': return 2;
        case '}': return 3;
        case '>': return 4;
        default: throw "Unrecognized";
        }
    }

    constexpr char GetCorresponding(char c) {
        switch (c) {
        case '(': return ')';
        case ')': return '(';
        case '{': return '}';
        case '}': return '{';
        case '[': return ']';
        case ']': return '[';
        case '<': return '>';
        case '>': return '<';
        default: throw "Unrecognized char";
        }
    }

    constexpr std::pair<size_t, size_t> GetScore(std::string_view line) {
        Constexpr::Stack<char> stack;
        std::pair<size_t, size_t> result;
        for (auto c : line) {
            switch (c) {
            case '(': case '[': case '{': case '<':
                stack.push(c);
                break;
            default:
                auto top = stack.pop();
                if (top != GetCorresponding(c)) {
                    result.first = GetCorruptScore(c);
                    return result;
                }
            }
        }
        
        while (!stack.is_empty()) {
            result.second *= 5;
            result.second += GetAcScore(GetCorresponding(stack.pop()));
        }

        return result;
    }

    PART_ONE() {
        auto scores = ParseLines(Lines, GetScore);
        return Constexpr::ToString(std::accumulate(scores.begin(), scores.end(), 0ull, [](size_t prev, const auto& pair) {
            return prev + pair.first;
            }));
    }

    constexpr size_t SolvePartTwo(const auto& lines) {
        auto scores = ParseLines(lines, GetScore);
        std::vector<size_t> toKeep;
        std::transform(scores.begin(), scores.end(), std::back_inserter(toKeep), [](const auto& p) {
            return p.second;
            });
        std::erase_if(toKeep, [](auto s) { return s == 0; });
        std::sort(toKeep.begin(), toKeep.end());
        return toKeep[toKeep.size() / 2];
    }
    PART_TWO() {
        return Constexpr::ToString(SolvePartTwo(Lines));
    }

    TESTS() {
        static_assert(GetScore("{([(<{}[<>[]}>{[]{[(<()>").first == 1197);
        static_assert(GetScore("[[<[([]))<([[{}[[()]]]").first == 3);
        static_assert(GetScore("(((({<>}<{<{<>}{[]{[]{}").second == 1480781);

        std::vector<std::string> lines = {
            "[({(<(())[]>[[{[]{<()<>>",
            "[(()[<>])]({[<{<<[]>>(",
            "(((({<>}<{<{<>}{[]{[]{}",
            "{<[[]]>}<{[{[{[]{()[[[]",
            "<{([{{}}[<[[[<>{}]]]>[]]"
        };

        if (SolvePartTwo(lines) != 288957) return false;
        return true;
    }
}