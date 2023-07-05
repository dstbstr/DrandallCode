#include "2021/d12_CavePath.h"

SOLUTION(2021, 12) {
    using Map = Constexpr::SmallMap<std::string, std::vector<std::string>>;

    constexpr Map ParseInput(const std::vector<std::string>&lines) {
        Map result;
        for (const auto& line : lines) {
            auto split = Constexpr::Split(line, "-");
            auto lhs = std::string(split[0]);
            auto rhs = std::string(split[1]);

            if (!result.contains(lhs)) {
                result[lhs] = std::vector<std::string>{};
            }
            if (!result.contains(rhs)) {
                result[rhs] = std::vector<std::string>{};
            }
            result[lhs].push_back(rhs);
            result[rhs].push_back(lhs);
        }

        return result;
    }

    constexpr bool TestParseInput() {
        std::vector<std::string> lines = {
            "start-A",
            "start-b",
            "A-c",
            "A-b",
            "b-d",
            "A-end",
            "b-end"
        };
        auto map = ParseInput(lines);
        if (map["start"].size() != 2) return false;
        if (map["A"].size() != 4) return false;
        if (map["b"].size() != 4) return false;
        if (map["c"].size() != 1) return false;
        if (map["d"].size() != 1) return false;
        if (map["end"].size() != 2) return false;
        return true;
    }
    static_assert(TestParseInput());

    constexpr bool HasDuplicate(const std::vector<std::string>&path) {
        for (const auto& step : path) {
            if (step[0] <= 'Z') continue;
            if (std::count(path.cbegin(), path.cend(), step) > 1) return true;
        }
        return false;
    }

    constexpr void RecursePaths(const Map & map, std::vector<std::string> currentPath, std::vector<std::vector<std::string>>&result, bool allowDuplicate) {
        auto currentPosition = currentPath.back();
        std::vector<std::string> nextSteps;

        std::copy_if(map.at(currentPosition).cbegin(), map.at(currentPosition).cend(), std::back_inserter(nextSteps), [&](std::string step) {
            if (step[0] <= 'Z') return true;
            if (step == "start") return false;

            if (std::find(currentPath.cbegin(), currentPath.cend(), step) == currentPath.cend()) {
                return true;
            }
            else {
                return allowDuplicate && !HasDuplicate(currentPath);
            }
            });

        for (const auto& step : nextSteps) {
            currentPath.push_back(step);
            if (step == "end") {
                if (std::find(result.begin(), result.end(), currentPath) == result.end()) {
                    result.push_back(currentPath);
                }
                currentPath.pop_back();
                continue;
            }
            RecursePaths(map, currentPath, result, allowDuplicate);
            currentPath.pop_back();
        }
    }

    constexpr bool TestRecursePaths() {
        std::vector<std::string> lines = {
        "start-A",
        "start-b",
        "A-c",
        "A-b",
        "b-d",
        "A-end",
        "b-end"
        };
        auto map = ParseInput(lines);

        std::vector<std::vector<std::string>> result;
        RecursePaths(map, { "start" }, result, false);
        if (result.size() != 10) return false;

        result.clear();
        RecursePaths(map, { "start" }, result, true);
        if (result.size() != 36) return false;
        return true;
    }
    static_assert(TestRecursePaths());

    constexpr auto Part1(const std::vector<std::string>&lines) {
        auto map = ParseInput(lines);
        std::vector<std::vector<std::string>> result;
        RecursePaths(map, { "start" }, result, false);
        return result.size();
    }

    constexpr auto Part2(const std::vector<std::string>&lines) {
        auto map = ParseInput(lines);
        std::vector<std::vector<std::string>> result;
        RecursePaths(map, { "start" }, result, true);
        return result.size();
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        if (!TestParseInput()) return false;
        if (!TestRecursePaths()) return false;

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