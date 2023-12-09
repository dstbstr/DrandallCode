#pragma once

#include "2015/d6_LightWar.h"

SOLUTION(2015, 6) {
    enum struct CommandType { On, Off, Toggle };

    struct Command {
        UCoord BottomLeft{};
        UCoord TopRight{};
        CommandType CommandType{};
    };

    constexpr Command ParseCommand(std::string_view line) {
        auto s1 = Constexpr::Split(line, " through ");
        auto s2 = Constexpr::Split(s1[0], " ");
        auto blStr = Constexpr::Split(s2.back(), ",");
        auto trStr = Constexpr::Split(s1[1], " ");

        Command result;
        result.CommandType = s2.size() == 2 ? CommandType::Toggle :
            s2[1] == "on" ? CommandType::On : CommandType::Off;

        result.BottomLeft = UCoord(s2.back());
        result.TopRight = UCoord(s1[1]);

        return result;
    }

    constexpr std::vector<size_t> GetLightIndexes(const Command& command, size_t gridSize) {
        std::vector<size_t> result;
        for (auto x = command.BottomLeft.X; x <= command.TopRight.X; x++) {
            for (auto y = command.BottomLeft.Y; y <= command.TopRight.Y; y++) {
                result.push_back(x * gridSize + y);
            }
        }
        return result;
    }

    constexpr size_t SolvePartOne(auto& lines) {
        constexpr size_t gridSize = 1000;
        auto gridPtr = new std::array<bool, gridSize* gridSize>();
        auto grid = *gridPtr;

        for (const auto& line : lines) {
            auto command = ParseCommand(line);
            auto indexes = GetLightIndexes(command, gridSize);
            switch (command.CommandType) {
            case CommandType::On: for (auto i : indexes) grid[i] = true; break;
            case CommandType::Off: for (auto i : indexes) grid[i] = false; break;
            case CommandType::Toggle: for (auto i : indexes) grid[i] = !grid[i]; break;
            }
        }

        auto count = std::count(grid.cbegin(), grid.cend(), true);
        delete gridPtr;

        return count;
    }
    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(lines));
    }

    constexpr size_t SolvePartTwo(auto& lines) {
        constexpr size_t gridSize = 1000;
        size_t* grid = new size_t[gridSize * gridSize]();

        for (const auto& line : lines) {
            auto command = ParseCommand(line);
            auto indexes = GetLightIndexes(command, gridSize);
            switch (command.CommandType) {
            case CommandType::On: for (auto i : indexes) grid[i]++; break;
            case CommandType::Off: for (auto i : indexes) grid[i] = grid[i] == 0 ? 0 : grid[i] - 1; break;
            case CommandType::Toggle: for (auto i : indexes) grid[i] += 2; break;
            }
        }

        size_t result = 0;
        for (size_t i = 0; i < gridSize * gridSize; i++) {
            result += grid[i];
        }

        delete[] grid;
        return result;
    }

    PART_TWO() {
        return Constexpr::ToString(SolvePartTwo(lines));
    }

    constexpr bool TestParseCommand() {
        auto c1 = ParseCommand("turn on 0,0 through 999,999");
        if (c1.CommandType != CommandType::On) return false;
        if (c1.BottomLeft != UCoord{ 0, 0 }) return false;
        if (c1.TopRight != UCoord{ 999, 999 }) return false;

        auto c2 = ParseCommand("toggle 0,0 through 999,0");
        if (c2.CommandType != CommandType::Toggle) return false;
        if (c2.BottomLeft != UCoord{ 0, 0 }) return false;
        if (c2.TopRight != UCoord{ 999,0 }) return false;

        auto c3 = ParseCommand("turn off 499,499 through 500,500");
        if (c3.CommandType != CommandType::Off) return false;
        if (c3.BottomLeft != UCoord{ 499, 499 }) return false;
        if (c3.TopRight != UCoord{ 500,500 }) return false;

        return true;
    }

    TESTS() {
        if (!TestParseCommand()) return false;

        std::vector<std::string> lines = {
            "turn on 0,0 through 1,1",
            "toggle 1,1 through 2,2"
        };
        if (SolvePartOne(lines) != 6) return false;

        return true;
    }
}