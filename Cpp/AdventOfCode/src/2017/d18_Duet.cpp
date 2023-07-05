#include "2017/d18_Duet.h"

#include <queue>

SOLUTION(2017, 18) {
    using Registers = std::array<s64, 26>;

    constexpr s64 ReadValue(std::string_view sv, const Registers & registers) {
        if (sv.size() == 1 && sv[0] >= 'a') {
            return registers[sv[0] - 'a'];
        }

        s64 result;
        Constexpr::ParseNumber(sv, result);
        return result;
    }

    constexpr s64 ApplyCommand(const std::string & command, s64 & instruction, Registers & registers, s64 & lastPlayed) {
        auto split = Constexpr::Split(command, " ");
        auto regIndex = split[1][0] - 'a';
        s64 value = 0;
        if (split.size() > 2) {
            value = ReadValue(split[2], registers);
        }

        if (split[0] == "snd") {
            lastPlayed = registers[regIndex];
        }
        else if (split[0] == "set") {
            registers[regIndex] = value;
        }
        else if (split[0] == "add") {
            registers[regIndex] += value;
        }
        else if (split[0] == "mul") {
            registers[regIndex] *= value;
        }
        else if (split[0] == "mod") {
            registers[regIndex] %= value;
        }
        else if (split[0] == "rcv") {
            if (registers[regIndex] != 0) {
                instruction++;
                return lastPlayed;
            }
        }
        else if (split[0] == "jgz") {
            if (registers[regIndex] > 0) {
                instruction += value - 1;
            }
        }

        instruction++;
        return 0;
    }

    constexpr auto Part1(const std::vector<std::string>&lines) {
        s64 instruction = 0;
        s64 lastPlayed = 0;
        Registers registers;
        registers.fill(0);

        while (instruction < static_cast<s64>(lines.size())) {
            auto recovered = ApplyCommand(lines[instruction], instruction, registers, lastPlayed);
            if (recovered > 0) {
                return recovered;
            }
        }

        return 0ll;
    }

    constexpr bool ApplyCommandV2(const std::string & command, s64 & instruction, Registers & registers, std::queue<s64>&inQueue, std::queue<s64>&outQueue) {
        auto split = Constexpr::Split(command, " ");
        size_t regIndex = 0;
        if (split[1][0] >= 'a') {
            regIndex = split[1][0] - 'a';
        }
        else {
            Constexpr::ParseNumber(split[1], regIndex);
        }
        s64 value = 0;
        if (split.size() > 2) {
            value = ReadValue(split[2], registers);
        }

        if (split[0] == "snd") {
            outQueue.push(ReadValue(split[1], registers));
        }
        else if (split[0] == "set") {
            registers[regIndex] = value;
        }
        else if (split[0] == "add") {
            registers[regIndex] += value;
        }
        else if (split[0] == "mul") {
            registers[regIndex] *= value;
        }
        else if (split[0] == "mod") {
            registers[regIndex] %= value;
        }
        else if (split[0] == "rcv") {
            if (inQueue.empty()) {
                return false;
            }
            registers[regIndex] = inQueue.front();
            inQueue.pop();
        }
        else if (split[0] == "jgz") {
            if (registers[regIndex] > 0) {
                instruction += value - 1;
            }
        }

        instruction++;
        return true;
    }

    auto Part2(const std::vector<std::string>&lines) {
        s64 p0Inst = 0;
        s64 p1Inst = 0;

        Registers p0Regs;
        Registers p1Regs;
        p0Regs.fill(0);
        p1Regs.fill(0);
        p1Regs['p' - 'a'] = 1;

        std::queue<s64> p0Queue;
        std::queue<s64> p1Queue;
        s64 maxInst = static_cast<s64>(lines.size());

        u32 p1SendCount = 0;
        while (true) {
            bool progress = false;

            if (p0Inst < maxInst) {
                if (ApplyCommandV2(lines[p0Inst], p0Inst, p0Regs, p0Queue, p1Queue)) {
                    progress = true;
                }
            }
            if (p1Inst < maxInst) {
                auto initialSize = p0Queue.size();
                if (ApplyCommandV2(lines[p1Inst], p1Inst, p1Regs, p1Queue, p0Queue)) {
                    progress = true;
                    if (p0Queue.size() != initialSize) {
                        p1SendCount++;
                    }
                }
            }

            if (!progress) {
                break;
            }
        }

        return p1SendCount;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "snd 1",
            "snd 2",
            "snd p",
            "rcv a",
            "rcv b",
            "rcv c",
            "rcv d"
        };

        if (Part2(lines) != 3) return false;
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