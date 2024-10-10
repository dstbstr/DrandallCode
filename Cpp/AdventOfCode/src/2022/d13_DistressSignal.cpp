#include "2022/d13_DistressSignal.h"

SOLUTION(2022, 13) {
    constexpr bool Compare(const std::string& first, const std::string& second) {
        //Need to be able to add square brackets, but not change the original
        std::string lhs = first;
        std::string rhs = second;
        size_t i = 0;
        char a, b;
        while (i < lhs.size() && i < rhs.size()) {
            a = lhs[i];
            b = rhs[i];
            if (a == b);
            else if (b == ']') return false;
            else if (a == ']') return true;
            else if (a == '[') { //[ vs d
                rhs.insert(rhs.begin() + i + 1, ']');
                rhs.insert(rhs.begin() + i, '[');
            }
            else if (b == '[') {//d vs [
                lhs.insert(lhs.begin() + i + 1, ']');
                lhs.insert(lhs.begin() + i, '[');
            }
            else { //d vs d
                return a < b;
            }

            i++;
        }
        return lhs.size() < rhs.size();
    }

    constexpr size_t SolvePartOne(const std::vector<std::string>& lines) {
        auto copy = lines;
        for (auto& line : copy) {
            Constexpr::ReplaceAll(line, "10", "A");
        }
        auto groups = SplitInputIntoGroups(copy);
        size_t result = 0;
        for (size_t i = 0; i < groups.size(); i++) {
            result += (1 + i) * Compare(groups[i][0], groups[i][1]);
        }
        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(lines));
    }

    constexpr size_t SolvePartTwo(const auto& lines) {
        std::vector<std::string> all;
        for (auto i = 0; i < lines.size(); i += 3) {
            all.push_back(lines[i]);
            all.push_back(lines[i + 1]);
        }
        for (auto& line : all) {
            Constexpr::ReplaceAll(line, "10", "A");
        }
        std::string a = "[[2]]";
        std::string b = "[[6]]";
        all.push_back(a);
        all.push_back(b);

        std::sort(all.begin(), all.end(), Compare);

        size_t result = 1;
        for (auto i = 0; i < all.size(); i++) {
            if (all[i] == a || all[i] == b) {
                result *= (i + 1);
            }
        }
        return result;
    }
    PART_TWO() {
        return Constexpr::ToString(SolvePartTwo(lines));
    }

    TESTS() {
        static_assert(Compare("[1,1,3,1,1]", "[1,1,5,1,1]"));
        static_assert(Compare("[[1],[2,3,4]]", "[[1],4]"));
        static_assert(!Compare("[9]", "[[8,7,6]]"));
        static_assert(Compare("[[4,4],4,4]", "[[4,4],4,4,4]"));
        static_assert(!Compare("[7,7,7,7]", "[7,7,7]"));
        static_assert(Compare("[]", "[3]"));
        static_assert(!Compare("[[[]]]", "[[]]"));
        static_assert(!Compare("[1,[2,[3,[4,[5,6,7]]]],8,9]", "[1,[2,[3,[4,[5,6,0]]]],8,9]"));

        static_assert(Compare("[2]", "[A]"));
        static_assert(Compare("[A]", "[B]"));
        static_assert(!Compare("[A]", "[2]"));
        static_assert(!Compare("[[1],[4,3,2]", "[[1],2]"));
        static_assert(!Compare("[1,[2,[1,2]],3]", "[1,[2,[1,2]],3]"));
        static_assert(Compare("[1,[2,[1,2]],1]", "[1,[2,[1,2]],3]"));
        static_assert(!Compare("[]", "[]"));
        static_assert(!Compare("[[8,[[7]]]]", "[[[[8]]]]"));
        static_assert(Compare("[8,[[7]]]]", "[[[[8],2]]]"));
        static_assert(!Compare("[[1,2],4]", "[[1],5,5]"));
        static_assert(Compare("[[1,2],4]", "[[[3]],5,5]"));
        static_assert(!Compare("[1,2,3,[1,2,3],4,1]", "[1,2,3,[1,2,3],4,0]"));
        static_assert(Compare("[[8,[[7,A,A,5],[8,4,9]],3,5],[[[3,9,4],5,[7,5,5]],[[3,2,5],[A],[5,5],0,[8]]],[4,2,[],[[7,5,6,3,0],[4,4,A,7],6,[8,A,9]]],[[4,[],4],A,1]]", "[[[[8], [3, A],[7,6,3,7,4],1,8]]]"));
        std::vector<std::string> lines = {
            "[1,1,3,1,1]",
            "[1,1,5,1,1]",
            "",
            "[[1],[2,3,4]]",
            "[[1],4]",
            "",
            "[9]",
            "[[8,7,6]",
            "",
            "[[4,4],4,4]",
            "[[4,4],4,4,4]",
            "",
            "[7,7,7,7]",
            "[7,7,7]",
            "",
            "[]",
            "[3]",
            "",
            "[[[]]]",
            "[[]]",
            "",
            "[1,[2,[3,[4,[5,6,7]]]],8,9]",
            "[1,[2,[3,[4,[5,6,0]]]],8,9]",
        };

        if (SolvePartOne(lines) != 13) return false;
        if (SolvePartTwo(lines) != 140) return false;

        return true;
    }
}