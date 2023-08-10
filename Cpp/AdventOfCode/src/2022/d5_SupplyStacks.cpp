#include "2022/d5_SupplyStacks.h"

SOLUTION(2022, 5) {
    using Stacks = std::vector<Constexpr::Stack<char>>;

    constexpr Stacks BuildStacks(const std::vector<std::string>& lines) {
        Stacks result;
        result.resize((lines[0].size() / 4) + 1);

        for (s32 line = static_cast<s32>(lines.size()) - 2; line >= 0; line--) {
            for (auto c = 1, i = 0; c < lines[0].size(); c += 4, i++) {
                auto box = lines[line][c];
                if (box == ' ') continue;
                result[i].push(box);
            }
        }
        return result;
    }

    constexpr Vec3<size_t> ParseMove(const std::string& line) {
        Vec3<size_t> result;
        //move 10 from 21 to 15
        auto s = Constexpr::Split(line, " ");
        Constexpr::ParseNumber(s[1], result.X);
        Constexpr::ParseNumber(s[3], result.Y);
        Constexpr::ParseNumber(s.back(), result.Z);
        result -= {0, 1, 1}; //Make these 0 based
        return result;
    }

    constexpr auto Solve(const std::vector<std::string>& lines, auto MoveFunc) {
        auto groups = SplitInputIntoGroups(lines);
        auto stacks = BuildStacks(groups[0]);
        auto moves = ParseLines(groups[1], ParseMove);

        for (auto move : moves) {
            MoveFunc(move, stacks);
        }

        std::string result;
        for (auto s : stacks) {
            if (s.is_empty()) continue;
            result.push_back(s.top());
        }
        return result;
    }

    PART_ONE() {
        return Solve(lines, [](Vec3<size_t> move, Stacks& stacks) {
            for (auto i = 0; i < move.X; i++) {
                stacks[move.Z].push(stacks[move.Y].pop());
            }
            });
    }

    PART_TWO() {
        return Solve(lines, [](Vec3<size_t> move, Stacks& stacks) {
            Constexpr::Stack<char> temp;
            for (auto i = 0; i < move.X; i++) {
                temp.push(stacks[move.Y].pop());
            }
            for (auto i = 0; i < move.X; i++) {
                stacks[move.Z].push(temp.pop());
            }
        });
    }

    TESTS() {
        std::vector<std::string> lines{
            "    [D]     ",
            "[N] [C]     ",
            "[Z] [M] [P] ",
            " 1   2   3  ",
            "",
            "move 1 from 2 to 1",
            "move 3 from 1 to 3",
            "move 2 from 2 to 1",
            "move 1 from 1 to 2"
        };
        
        if (PartOne(lines) != "CMZ") return false;
        if (PartTwo(lines) != "MCD") return false;

        return true;
    }
}