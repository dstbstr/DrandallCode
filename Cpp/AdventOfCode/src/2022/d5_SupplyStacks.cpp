#include "2022/d5_SupplyStacks.h"

#include <stack>

SOLUTION(2022, 5) {
    using StackType = std::vector<std::stack<char>>;

    s32 FindBottomStack(const std::vector<std::string>&lines) {
        for (s32 i = 0; i < lines.size(); i++) {
            if (lines[i][0] == '1' || lines[i][1] == '1') {
                return i - 1;
            }
        }

        return 0;
    }

    void FillStacks(const std::string & line, StackType & stacks) {
        int stack = 0;
        for (auto i = 0; i < line.size(); i += 4) {
            if (stacks.size() < stack + 1) {
                stacks.push_back(std::stack<char>{});
            }
            if (line[i + 1] != ' ') {
                stacks[stack].push(line[i + 1]);
            }
            stack++;
        }
    }

    void ParseInstruction(const std::string & line, u32 & move, u32 & from, u32 & to) {
        static const auto re = std::regex(R"(\D+(\d+)\D+(\d+)\D+(\d+))");
        std::smatch match;
        if (std::regex_search(line, match, re)) {
            Constexpr::ParseNumber(match[1].str(), move);
            Constexpr::ParseNumber(match[2].str(), from);
            Constexpr::ParseNumber(match[3].str(), to);

            // fix 1 based indexing
            from--;
            to--;
        }
    }

    void ProcessInstruction(u32 move, u32 from, u32 to, StackType & stacks) {
        for (u32 i = 0; i < move; i++) {
            stacks[to].push(stacks[from].top());
            stacks[from].pop();
        }
    }

    void ProcessInstruction9001(u32 move, u32 from, u32 to, StackType & stacks) {
        std::stack<char> temp;
        for (u32 i = 0; i < move; i++) {
            temp.push(stacks[from].top());
            stacks[from].pop();
        }
        while (!temp.empty()) {
            stacks[to].push(temp.top());
            temp.pop();
        }
    }

    std::string RunImpl(const std::vector<std::string>&lines, std::function<void(u32, u32, u32, StackType&)> processFunc) {
        StackType stacks;
        auto bottomStack = FindBottomStack(lines);
        for (s32 i = bottomStack; i >= 0; i--) {
            FillStacks(lines[i], stacks);
        }

        for (s32 i = bottomStack + 3; i < lines.size(); i++) {
            u32 move;
            u32 from;
            u32 to;
            ParseInstruction(lines[i], move, from, to);
            processFunc(move, from, to, stacks);
        }

        std::string result = "";
        for (auto stack : stacks) {
            result += stack.top();
        }

        return result;
    }
    std::string RunFirst(const std::vector<std::string>&lines) {
        return RunImpl(lines, ProcessInstruction);
    }

    std::string Run(const std::vector<std::string>&lines) {
        return RunImpl(lines, ProcessInstruction9001);
    }

    bool RunTests() {
        std::vector<std::string> lines;
        lines.push_back("    [D]     ");
        lines.push_back("[N] [C]     ");
        lines.push_back("[Z] [M] [P] ");
        lines.push_back(" 1   2   3  ");
        lines.push_back("");
        lines.push_back("move 1 from 2 to 1");
        lines.push_back("move 3 from 1 to 3");
        lines.push_back("move 2 from 2 to 1");
        lines.push_back("move 1 from 1 to 2");

        StackType stacks;
        auto bottomStack = FindBottomStack(lines);
        for (s32 i = bottomStack; i >= 0; i--) {
            FillStacks(lines[i], stacks);
        }

        if (stacks.size() != 3) return false;
        if (stacks[0].top() != 'N') return false;
        if (stacks[1].top() != 'D') return false;
        if (stacks[2].top() != 'P') return false;
        if (stacks[0].size() != 2) return false;
        if (stacks[1].size() != 3) return false;
        if (stacks[2].size() != 1) return false;

        for (s32 i = bottomStack + 3; i < lines.size(); i++) {
            u32 move;
            u32 from;
            u32 to;
            ParseInstruction(lines[i], move, from, to);
            ProcessInstruction(move, from, to, stacks);
        }

        if (stacks[0].top() != 'C') return false;
        if (stacks[1].top() != 'M') return false;
        if (stacks[2].top() != 'Z') return false;
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