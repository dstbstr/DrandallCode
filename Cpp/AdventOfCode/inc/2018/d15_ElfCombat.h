#pragma once

#include "Common.h"
#include "AStar.h"

#include <iostream>

constexpr auto BookOrder = [](const RowCol& lhs, const RowCol& rhs) -> bool {
    if (lhs.Row != rhs.Row) return lhs.Row < rhs.Row;
    return lhs.Col < rhs.Col;
};

enum struct UnitType {Goblin, Elf};
struct Unit {
    RowCol Pos;
    s32 Hp = 200;
    UnitType Type;
    s32 Attack = 3;

    constexpr bool operator<(const Unit& other) const {
        if (Pos.Row != other.Pos.Row) return Pos.Row < other.Pos.Row;
        return Pos.Col < other.Pos.Col;
    }
};

class Grid {
public:
    Grid(const std::vector<std::string>& lines, u32 elfPower) {
        for (size_t row = 0; row < lines.size(); row++) {
            std::vector<bool> gridLine;
            for (size_t col = 0; col < lines[row].size(); col++) {
                auto c = lines[row][col];
                gridLine.push_back(c == '.');
                if (c == 'G' || c == 'E') {
                    Unit unit;
                    unit.Pos = { row, col };
                    unit.Type = (c == 'G' ? UnitType::Goblin : UnitType::Elf);
                    if (unit.Type == UnitType::Elf) {
                        unit.Attack = elfPower;
                    }
                    units.push_back(unit);
                }
            }
            grid.push_back(gridLine);
        }
    }

    void SortUnits() {
        std::sort(units.begin(), units.end());
    }

    size_t GetUnitCount() const {
        return units.size();
    }

    std::vector<RowCol> GetAvailableNeighbors(RowCol pos) const {
        auto neighbors = Get4Neighbors(pos, GetLimits());
        std::vector<RowCol> result;
        std::copy_if(neighbors.begin(), neighbors.end(), std::back_inserter(result), [&](RowCol potential) {
            return IsAvailable(potential);
            });

        return result;
    }

    bool IsAvailable(RowCol pos) const {
        return grid[pos.Row][pos.Col];
    }

    bool IsDead(size_t unit) const {
        return units[unit].Hp <= 0;
    }

    bool BattleOver() const {
        bool hasGoblin = false;
        bool hasElf = false;
        for (const auto& unit : units) {
            if (unit.Hp > 0) {
                if (unit.Type == UnitType::Elf) hasElf = true;
                else hasGoblin = true;
            }

            if (hasElf && hasGoblin) return false;
        }

        return !(hasElf && hasGoblin);
    }

    bool ElvesWin() const {
        for (const auto& unit : units) {
            if (unit.Type == UnitType::Elf && unit.Hp <= 0) return false;
        }
        return true;
    }

    u32 GetRemainingHp() const {
        u32 remainingHp = 0;
        for (const auto& unit : units) {
            remainingHp += std::max(0, unit.Hp);
        }

        return remainingHp;
    }

    RowCol GetLimits() const {
        static RowCol limits = { grid.size() - 1, grid[0].size() - 1};
        return limits;
    }

    std::vector<size_t> GetTargets(size_t unitIndex) const {
        const auto& unit = units[unitIndex];
        std::vector<size_t> result;
        for(size_t i = 0; i < units.size(); i++) {
            if (unit.Type != units[i].Type && units[i].Hp > 0) result.push_back(i);
        }
        return result;
    }

    std::vector<RowCol> GetPath(RowCol fromPos, RowCol target) const {
        auto isCompleteFunc = [&target](const RowCol& pos) {
            return pos == target;
        };

        auto costFunc = [](const RowCol&, const RowCol&) { return 1; };
        auto heuristic = [&](const RowCol& pos) {
            return static_cast<u32>(MDistance(pos, target));
        };

        auto neighborFunc = [&](const RowCol& pos) {
            return GetAvailableNeighbors(pos);
        };

        return AStar<RowCol, RowColHash>(fromPos, costFunc, AStarFuncs::TrySmallest, isCompleteFunc, heuristic, neighborFunc);
    }

    RowCol GetBestPos(const std::vector<RowCol>& neighbors, const std::vector<size_t>& distances) const {
        size_t minPositiveDistance = 9999;
        for (auto distance : distances) {
            if (distance > 0 && distance < minPositiveDistance) {
                minPositiveDistance = distance;
            }
        }

        std::vector<RowCol> closest;
        for (auto i = 0; i < distances.size(); i++) {
            if (distances[i] == minPositiveDistance) {
                closest.push_back(neighbors[i]);
            }
        }

        std::sort(closest.begin(), closest.end());
        return closest[0];
    }

    bool TryFindNearestTargetSquare(size_t unitIndex, RowCol& outPos) const {
        auto targets = GetTargets(unitIndex);
        if (targets.empty()) return false;

        std::vector<RowCol> neighbors;
        for (size_t targetIndex : targets) {
            auto n = GetAvailableNeighbors(units[targetIndex].Pos);
            std::copy(n.begin(), n.end(), std::back_inserter(neighbors));
        }
        if (neighbors.empty()) return false;

        std::vector<size_t> distances;
        for (auto pos : neighbors) {
            distances.push_back(GetPath(units[unitIndex].Pos, pos).size());
        }
        
        if (Constexpr::FindMax(distances) == 0) return false;
        outPos = GetBestPos(neighbors, distances);
        return true;
    }


    std::vector<RowCol> GetPath(size_t fromIndex, size_t toIndex) const {
        const auto& to = units[toIndex];
        const auto& from = units[fromIndex];

        auto isCompleteFunc = [&to](const RowCol& pos) {
            return MDistance(to.Pos, pos) == 1;
        };

        auto costFunc = [](const RowCol&, const RowCol&) { return 1; };
        auto heuristic = [&](const RowCol& pos) {
            auto mDist = static_cast<u32>(MDistance(pos, to.Pos));
            if (pos.Row > from.Pos.Row) mDist += 3;
            if (pos.Col > from.Pos.Col) mDist += 2;
            if (pos.Col < from.Pos.Col) mDist += 1;
            return mDist;
        };

        auto neighborFunc = [&](const RowCol& pos) {
            return GetAvailableNeighbors(pos);
        };

        return AStar<RowCol, RowColHash>(from.Pos, costFunc, AStarFuncs::TrySmallest, isCompleteFunc, heuristic, neighborFunc);
    }

    RowCol GetFirstStep(size_t unitIndex, RowCol targetPos) {
        auto& unit = units[unitIndex];
        auto neighbors = GetAvailableNeighbors(unit.Pos);
        if (neighbors.size() == 1) {
            return neighbors[0];
        }

        std::vector<size_t> distances;
        for (auto pos : neighbors) {
            distances.push_back(GetPath(pos, targetPos).size());
        }

        return GetBestPos(neighbors, distances);
    }

    void Move(size_t unitIndex, RowCol targetPos) {
        auto& unit = units[unitIndex];
        auto move = GetFirstStep(unitIndex, targetPos);
        grid[unit.Pos.Row][unit.Pos.Col] = true;
        grid[move.Row][move.Col] = false;
        unit.Pos = move;
    }

    void Attack(size_t attackerIndex, size_t targetIndex) {
        const auto& attacker = units[attackerIndex];
        auto& target = units[targetIndex];
        target.Hp -= attacker.Attack;
        if (target.Hp <= 0) {
            grid[target.Pos.Row][target.Pos.Col] = true;
        }
    }

    bool GetTarget(size_t unitIndex, size_t& targetIndex) {
        const auto& attackerPos = units[unitIndex].Pos;
        std::vector<size_t> realTargets;
        auto targets = GetTargets(unitIndex);
        std::copy_if(targets.begin(), targets.end(), std::back_inserter(realTargets), [&](const size_t& targetIndex) {
            return units[targetIndex].Hp > 0 && MDistance(attackerPos, units[targetIndex].Pos) == 1;
            });
        
        if (realTargets.empty()) return false;

        std::sort(realTargets.begin(), realTargets.end(), [&](size_t lhs, size_t rhs) {
            if (units[lhs].Hp != units[rhs].Hp) {
                return units[lhs].Hp < units[rhs].Hp;
            }
            return units[lhs] < units[rhs];
            });

        targetIndex = realTargets[0];
        return true;
    }

    void PrintState() {
        for (size_t row = 0; row < grid.size(); row++) {
            for (size_t col = 0; col < grid[row].size(); col++) {
                RowCol pos = { row, col };
                char icon = (grid[row][col] ? '.' : '#');
                for (const auto& unit : units) {
                    if (unit.Pos == pos && unit.Hp > 0) {
                        icon = unit.Type == UnitType::Elf ? 'E' : 'G';
                        break;
                    }
                }

                std::cout << icon;
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

private:
    std::vector<std::vector<bool>> grid;
    std::vector<Unit> units;
};

bool TryMove(size_t unit, Grid& grid) {
    RowCol targetPos;
    if (!grid.TryFindNearestTargetSquare(unit, targetPos)) {
        return false;
    }
    grid.Move(unit, targetPos);
    return true;
}

auto Battle(Grid& grid) {
    const size_t unitCount = grid.GetUnitCount();
    u32 rounds = 0;
    bool battleOver = false;

    while (true) {
        grid.SortUnits();
        for (size_t unit = 0; unit < unitCount; unit++) {
            if (grid.IsDead(unit)) continue;
            size_t targetIndex = 0;
            if (grid.GetTarget(unit, targetIndex)) {
                grid.Attack(unit, targetIndex);
            }
            else {
                if (TryMove(unit, grid)) {
                    if (grid.GetTarget(unit, targetIndex)) {
                        grid.Attack(unit, targetIndex);
                    }
                }
                else if (grid.BattleOver()) {
                    battleOver = true;
                    break;
                }
            }
        }
        if (battleOver) break;
        rounds++;
    }

    return rounds * grid.GetRemainingHp();
}

auto PartOne(const std::vector<std::string>& lines) {
    Grid grid(lines, 3);

    std::cout << "Initial\n";
    grid.PrintState();

    auto result = Battle(grid);

    std::cout << "Final\n";
    grid.PrintState();
    std::cout << '\n';

    return result;
}

auto PartTwo(const std::vector<std::string>& lines) {
    u32 result = 0;

    for (auto i = 4; i < 20; i++) {
        Grid grid(lines, i);
        result = Battle(grid);

        if (grid.ElvesWin()) {
            break;
        }
    }

    return result;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "#######",
        "#.G...#",
        "#...EG#",
        "#.#.#G#",
        "#..G#E#",
        "#.....#",
        "#######"
    };
    if (PartOne(lines) != 27730) return false;

    lines = {
        "#######",
        "#G..#E#",
        "#E#E.E#",
        "#G.##.#",
        "#...#E#",
        "#...E.#",
        "#######"
    };

    if (PartOne(lines) != 36334) return false;

    lines = {
        "#######",
        "#E..EG#",
        "#.#G.E#",
        "#E.##E#",
        "#G..#.#",
        "#..E#.#",
        "#######"
    };
    if (PartOne(lines) != 39514) return false;

    lines = {
        "#######",
        "#E.G#.#",
        "#.#G..#",
        "#G.#.G#",
        "#G..#.#",
        "#...E.#",
        "#######"
    };
    if (PartOne(lines) != 27755) return false;

    lines = {
        "#######",
        "#.E...#",
        "#.#..G#",
        "#.###.#",
        "#E#G#G#",
        "#...#G#",
        "#######"
    };
    if (PartOne(lines) != 28944) return false;

    lines = {
        "#########",
        "#G......#",
        "#.E.#...#",
        "#..##..G#",
        "#...##..#",
        "#...#...#",
        "#.G...G.#",
        "#.....G.#",
        "#########"
    };
    if (PartOne(lines) != 18740) return false;
    return true;
}