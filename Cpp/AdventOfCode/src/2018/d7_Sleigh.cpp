#include "2018/d7_Sleigh.h"

SOLUTION(2018, 7) {
    constexpr std::pair<u8, u8> ParseLine(std::string_view line) {
        auto split = Constexpr::Split(line, " ");
        u8 lhs = static_cast<u8>(split[1][0] - 'A');
        u8 rhs = static_cast<u8>(split[7][0] - 'A');
        return { lhs, rhs };
    }

    //static_assert(ParseLine("Step C must be finished before step A can begin.") == std::pair<u8, u8>{2u, 0u});

    struct Instruction {
        char Name;
        std::vector<char> Requires;
    };

    constexpr void BuildMap(const auto& lines, std::array<u32, 26>& requirements, std::array<bool, 26>& existance) {
        auto instructions = ParseLines(lines, ParseLine);

        requirements.fill(0);
        existance.fill(false);

        for (auto [prereq, toBuild] : instructions) {
            existance[prereq] = true;
            existance[toBuild] = true;
            requirements[toBuild] |= 1 << prereq;
        }
    }

    constexpr char FindNext(std::array<bool, 26>&existance, const std::array<u32, 26>&requirements) {
        char next = -1;
        for (char i = 0; i < 26; i++) {
            if (existance[i] && requirements[i] == 0) {
                next = i;
                existance[i] = false;
                break;
            }
        }

        return next;
    }

    constexpr void RemoveRequirement(const std::array<bool, 26>&existance, std::array<u32, 26>&requirements, char done) {
        for (char i = 0; i < 26; i++) {
            if (existance[i]) {
                requirements[i] &= ~(1 << done);
            }
        }
    }

    constexpr bool TickWorkers(std::vector<u32>&workers) {
        bool hasWorker = false;
        for (auto& worker : workers) {
            worker = std::max(0u, worker - 1);
            if (worker == 0) hasWorker = true;
        }

        return hasWorker;
    }

    PART_ONE() {
        std::array<u32, 26> requirements;
        std::array<bool, 26> existance;
        BuildMap(Lines, requirements, existance);
        std::string result = "";

        while (true) {
            auto next = FindNext(existance, requirements);
            if (next != -1) {
                result.push_back('A' + next);
                RemoveRequirement(existance, requirements, next);
            }
            else {
                break;
            }
        }

        return result;
    }

    PART_TWO() {
        std::array<u32, 26> requirements;
        std::array<bool, 26> existance;
        BuildMap(Lines, requirements, existance);
        u32 totalSeconds = 0;
        u32 availableWorkers = 5;
        std::array<u32, 26> inFlightWork;
        inFlightWork.fill(0);

        while (true) {
            bool allDone = true;
            for (auto i = 0; i < existance.size(); i++) {
                if (existance[i]) {
                    allDone = false;
                    break;
                }
            }
            if (allDone) break;

            totalSeconds++;
            while (availableWorkers > 0) {
                auto next = FindNext(existance, requirements);
                if (next != -1) {
                    inFlightWork[next] = next + 60 + 1;
                    availableWorkers--;
                }
                else {
                    break;
                }
            }

            for (char i = 0; i < 26; i++) {
                if (inFlightWork[i] == 0) continue;
                else if (inFlightWork[i] == 1) {
                    RemoveRequirement(existance, requirements, i);
                    availableWorkers++;
                }
                inFlightWork[i]--;
            }
        }

        auto remaining = Constexpr::FindMax(inFlightWork);
        return Constexpr::ToString(totalSeconds + remaining);
    }

    TESTS() {
        //std::vector<std::string> lines = {
        //    "Step C must be finished before step A can begin.",
        //    "Step C must be finished before step F can begin.",
        //    "Step A must be finished before step B can begin.",
        //    "Step A must be finished before step D can begin.",
        //    "Step B must be finished before step E can begin.",
        //    "Step D must be finished before step E can begin.",
        //    "Step F must be finished before step E can begin."
        //};
        //
        //if (PartOne(lines) != "CABDFE") return false;

        return true;
    }
}