#include "2018/d16_Chronal.h"

SOLUTION(2018, 16) {
    using Registers = std::array<s32, 4>;
    using Ops = std::array<std::string_view, 16>;

    constexpr void Apply(const std::string_view & op, u8 a, u8 b, u8 c, Registers & regs) {
        if (op == "addr") regs[c] = regs[a] + regs[b];
        else if (op == "addi") regs[c] = regs[a] + b;
        else if (op == "mulr") regs[c] = regs[a] * regs[b];
        else if (op == "muli") regs[c] = regs[a] * b;
        else if (op == "banr") regs[c] = regs[a] & regs[b];
        else if (op == "bani") regs[c] = regs[a] & b;
        else if (op == "borr") regs[c] = regs[a] | regs[b];
        else if (op == "bori") regs[c] = regs[a] | b;
        else if (op == "setr") regs[c] = regs[a];
        else if (op == "seti") regs[c] = a;
        else if (op == "gtir") regs[c] = a > regs[b] ? 1 : 0;
        else if (op == "gtri") regs[c] = regs[a] > b ? 1 : 0;
        else if (op == "gtrr") regs[c] = regs[a] > regs[b] ? 1 : 0;
        else if (op == "eqir") regs[c] = a == regs[b] ? 1 : 0;
        else if (op == "eqri") regs[c] = regs[a] == b ? 1 : 0;
        else if (op == "eqrr") regs[c] = regs[a] == regs[b] ? 1 : 0;
    }

    constexpr Ops OpCodes = { "addr", "addi", "mulr", "muli", "banr", "bani", "borr", "bori", "setr", "seti", "gtir", "gtri", "gtrr", "eqir", "eqri", "eqrr" };

    constexpr Registers ParseRegisters(const std::string & str) {
        auto running = str.substr(str.find('[') + 1);
        running = running.substr(0, running.size() - 1);
        auto split = Constexpr::Split(running, ", ");

        Registers result;
        for (auto i = 0; i < 4; i++) {
            Constexpr::ParseNumber(split[i], result[i]);
        }

        return result;
    }

    static_assert(ParseRegisters("Before: [3, 2, 1, 1]") == Registers{ 3, 2, 1, 1 });
    static_assert(ParseRegisters("After:  [3, 2, 2, 1]") == Registers{ 3, 2, 2, 1 });

    struct Triplet {
        Registers Before;
        Registers After;
        std::array<u8, 4> Instructions;
    };

    constexpr std::array<u8, 4> ParseInstruction(const std::string & line) {
        std::array<u8, 4> result;
        auto split = Constexpr::Split(line, " ");
        for (auto i = 0; i < 4; i++) {
            Constexpr::ParseNumber(split[i], result[i]);
        }
        return result;
    }

    constexpr Triplet ParseTriplet(const std::vector<std::string>& lines) {
        Triplet result;
        result.Before = ParseRegisters(lines[0]);
        result.Instructions = ParseInstruction(lines[1]);
        result.After = ParseRegisters(lines[2]);
        return result;
    }

    constexpr std::vector<Triplet> GetAllTriplets(const std::vector<std::vector<std::string>>& groups) {
        std::vector<Triplet> result;
        for (const auto& group : groups) {
            if (group.size() == 0) break;
            result.push_back(ParseTriplet(group));
        }

        return result;
    }

    constexpr std::vector<size_t> GetMatchingCodes(const Triplet & triplet) {
        std::vector<size_t> result;
        auto [i, a, b, c] = triplet.Instructions;
        for (size_t index = 0; index < OpCodes.size(); index++) {
            Registers regs = triplet.Before;
            Apply(OpCodes[index], a, b, c, regs);
            if (regs == triplet.After) {
                result.push_back(index);
            }
        }

        return result;
    }

    constexpr void OnSingle(std::array<std::array<bool, 16>, 16>&remaining, std::array<size_t, 16>& remainingCounts, size_t opId) {
        size_t opCode = 0;
        for (auto i = 0; i < 16; i++) {
            if (remaining[opId][i]) {
                opCode = i;
                break;
            }
        }

        for (auto i = 0; i < 16; i++) {
            if (i == opId) continue;
            if (remaining[i][opCode]) {
                remaining[i][opCode] = false;
                remainingCounts[i]--;

                if (remainingCounts[i] == 1) {
                    OnSingle(remaining, remainingCounts, i);
                }
            }
        }
    }

    constexpr Ops FindOps(const std::vector<Triplet>& triplets) {
        std::array<std::array<bool, 16>, 16> remaining;
        std::array<bool, 16> initial;
        initial.fill(true);
        remaining.fill(initial);
        std::array<size_t, 16> remainingCounts;
        remainingCounts.fill(16);

        for (const auto& triplet : triplets) {
            auto opId = triplet.Instructions[0];
            if (remainingCounts[opId] == 1) continue;
            auto codes = GetMatchingCodes(triplet);
            for (auto i = 0; i < 16; i++) {
                if (remaining[opId][i] && std::find(codes.begin(), codes.end(), i) == codes.end()) {
                    remaining[opId][i] = false;
                    remainingCounts[opId]--;
                }
            }
            if (remainingCounts[opId] == 1) {
                OnSingle(remaining, remainingCounts, opId);
            }
        }

        Ops result;

        for (auto i = 0; i < 16; i++) {
            for (auto codeIndex = 0; codeIndex < 16; codeIndex++) {
                if (remaining[i][codeIndex]) {
                    result[i] = OpCodes[codeIndex];
                    break;
                }
            }
        }

        return result;
    }

    constexpr s32 RunProgram(const std::vector<std::string>& lines, const Ops & ops) {
        Registers regs = { 0, 0, 0, 0 };
        for (size_t currentLine = 0; currentLine < lines.size(); currentLine++) {
            auto [i, a, b, c] = ParseInstruction(lines[currentLine]);
            Apply(ops[i], a, b, c, regs);
        }

        return regs[0];
    }

    PART_ONE() {
        auto groups = SplitInputIntoGroups(Lines);
        auto triplets = GetAllTriplets(groups);

        auto res = std::count_if(triplets.begin(), triplets.end(), [](const Triplet& t) { return GetMatchingCodes(t).size() > 2; });
        return Constexpr::ToString(res);
    }

    PART_TWO() {
        auto groups = SplitInputIntoGroups(Lines);
        auto triplets = GetAllTriplets(groups);
        auto ops = FindOps(triplets);

        return Constexpr::ToString(RunProgram(groups.back(), ops));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "Before: [3, 2, 1, 1]",
            "9 2 1 2",
            "After:  [3, 2, 2, 1]"
        };
        Triplet triplet = ParseTriplet(lines);
        
        if (triplet.Before != Registers{ 3, 2, 1, 1 }) return false;
        if (triplet.After != Registers{ 3, 2, 2, 1 }) return false;
        if (triplet.Instructions != std::array<u8, 4>{9, 2, 1, 2}) return false;

        return true;
    }
}