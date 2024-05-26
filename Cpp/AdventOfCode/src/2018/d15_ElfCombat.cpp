#include "2018/d15_ElfCombat.h"
#include "Core/Algorithms/AStar.h"

SOLUTION(2018, 15) {

    using Grid = std::vector<std::string>;
    using Hp = Constexpr::SmallMap<RowCol, s32>;

    constexpr std::vector<RowCol> GetTargets(const Grid & grid, const RowCol unit, const std::vector<RowCol> units) {
        char opposing = grid[unit.Row][unit.Col] == 'E' ? 'G' : 'E';
        return Constexpr::Where(units, [&](const auto& u) { return grid[u.Row][u.Col] == opposing; });
    }

    constexpr bool TryFindMoveTarget(const Grid & grid, RowCol limits, const RowCol unit, const std::vector<RowCol>& targets, RowCol& outTarget, RowCol& outFirstStep) {
        for (const auto& rc : targets) {
            if (MDistance(unit, rc) == 1) {
                outTarget = rc;
                outFirstStep = unit;
                return true;
            }
        }

        auto openSpaces = [&]() {
            std::vector<RowCol> result;
            for (const auto& rc : targets) {
                auto neighbors = GetDirectNeighbors(rc, limits);
                auto toKeep = Constexpr::Where(neighbors, [&](const auto u) { return grid[u.Row][u.Col] == '.'; });
                std::copy(toKeep.begin(), toKeep.end(), std::back_inserter(result));
            }
            return result;
        }();

        std::sort(openSpaces.begin(), openSpaces.end(), [&](const RowCol& lhs, const RowCol& rhs) {
            return MDistance(unit, lhs) < MDistance(unit, rhs);
            });

        auto startingPoints = Constexpr::Where(GetDirectNeighbors(unit, limits), [&](const RowCol& rc) { return grid[rc.Row][rc.Col] == '.'; });
            
        size_t shortestPath = 999;
        std::vector<std::vector<RowCol>> paths;
        for (const auto& start : startingPoints) {
            for (const auto& space : openSpaces) {
                if (MDistance(start, space) > shortestPath) break;
                auto path = AStarMin<RowCol>(start, space, [&](const RowCol& pos) {
                    return Constexpr::Where(GetDirectNeighbors(pos, limits), [&](const RowCol& rc) { return grid[rc.Row][rc.Col] == '.'; });
                    });
                if (path.size() > 0) {
                    paths.push_back(path);
                    shortestPath = std::min(shortestPath, path.size());
                }
            }
        }

        if (paths.empty()) return false;

        std::sort(paths.begin(), paths.end(), [](const auto& lhs, const auto& rhs) { 
            return lhs.size() != rhs.size() ?
                lhs.size() < rhs.size() :
                lhs.back() != rhs.back() ?
                lhs.back() < rhs.back() :
                lhs[0] < rhs[0];
            });
        
        outTarget = paths[0].back();
        outFirstStep = paths[0][0];
        return true;
    }

    constexpr RowCol FindAttackTarget(const Grid& grid, const Hp& hp, const RowCol& unit, const RowCol& limits) {
        auto neighbors = GetDirectNeighbors(unit, limits);
        std::sort(neighbors.begin(), neighbors.end());
        size_t lowestLife = 999;
        RowCol result{ 0, 0 };
        for (const auto& n : neighbors) {
            //let's not kill our friends
            if (hp.contains(n) && grid[n.Row][n.Col] != grid[unit.Row][unit.Col] && hp.at(n) < lowestLife) {
                lowestLife = hp.at(n);
                result = n;
            }
        }
        return result;
    }

    constexpr bool TickRound(Grid& grid, Hp& hp, RowCol limits, s32 elfAttack) {
        auto units = hp.GetKeys();
        std::sort(units.begin(), units.end());
        RowCol moveTarget;
        RowCol firstStep;
        bool canAttack = false;
        char unitType = '.';

        for (auto& unit : units) {
            canAttack = false;
            unitType = grid[unit.Row][unit.Col];
            if (unitType == '.') continue;  //this unit has been killed
            auto targets = GetTargets(grid, unit, units);
            if (targets.empty()) return true; //battle over

            if (TryFindMoveTarget(grid, limits, unit, targets, moveTarget, firstStep)) {
                if (firstStep == unit) {
                    canAttack = true;
                }
                else {
                    std::swap(grid[unit.Row][unit.Col], grid[firstStep.Row][firstStep.Col]);
                    hp[firstStep] = hp[unit];
                    hp.erase(unit);
                    unit = firstStep;
                    canAttack = firstStep == moveTarget;
                }
            }
            if (canAttack) {
                auto attackTarget = FindAttackTarget(grid, hp, unit, limits);
                hp[attackTarget] -= unitType == 'E' ? elfAttack : 3;
                if (hp[attackTarget] <= 0) {
                    hp.erase(attackTarget);
                    grid[attackTarget.Row][attackTarget.Col] = '.';
                }
            }
        }
        return false;
    }

    constexpr std::pair<size_t, size_t> CountUnits(const Grid& grid) {
        size_t elfCount = 0;
        size_t goblinCount = 0;
        for (const auto& line : grid) {
            elfCount += std::count(line.begin(), line.end(), 'E');
            goblinCount += std::count(line.begin(), line.end(), 'G');
        }

        return { elfCount, goblinCount };
    }

    constexpr std::string PrintState(const Grid& grid, const Hp& hp) {
        std::string result;
        for (size_t row = 0; row < grid.size(); row++) {
            std::string rowMap;
            std::string rowHp;
            for (size_t col = 0; col < grid[row].size(); col++) {
                rowMap.push_back(grid[row][col]);
                RowCol rc = { row, col };
                if (hp.contains(rc)) {
                    rowHp += "(" + Constexpr::ToString(hp.at(rc)) + ") ";
                }
            }
            result += rowMap + " " + rowHp + "\n";
        }
        return result;
    }

    constexpr size_t Battle(Grid& grid, Hp& hp, s32 elfAttack = 3) {
        size_t rounds = 0;
        auto [initialElves, initialGoblins] = CountUnits(grid);
        RowCol limits = { grid.size() - 1, grid[0].size() - 1 };

        while (true) {
            auto battleCompleted = TickRound(grid, hp, limits, elfAttack);
            if (elfAttack > 3) {
                auto [elfCount, goblinCount] = CountUnits(grid);
                if (elfCount < initialElves) return 0;
            }
            if (battleCompleted) break;
            rounds++;
        }

        return rounds;
    }
    

    constexpr Hp GetHp(const Grid& grid) {
        Hp hp;
        Constexpr::ApplyToMatrixIndex(grid, [&](size_t row, size_t col) {
            if (grid[row][col] >= 'A') {
                hp[{row, col}] = 200;
            }
        });

        return hp;
    }

    PART_ONE() {
        Grid grid = CopyToVector(lines);
        auto hp = GetHp(grid);
        auto rounds = Battle(grid, hp);
        auto hpValues = hp.GetValues();
        auto remainingHp = std::accumulate(hpValues.begin(), hpValues.end(), 0);

        return Constexpr::ToString(rounds * remainingHp);
    }

    PART_TWO() {
        auto initialGrid = CopyToVector(lines);
        auto initialHp = GetHp(initialGrid);
        for (auto i = 15; i < 20; i++) {
            Grid grid = initialGrid;
            Hp hp = initialHp;
            auto rounds = Battle(grid, hp, i);
            if (rounds > 0) {
                auto hpValues = hp.GetValues();
                auto remainingHp = std::accumulate(hpValues.begin(), hpValues.end(), 0);
                return Constexpr::ToString(rounds * remainingHp);
            }
        }
        return "Not Found";
    }

    constexpr bool TestMove(const std::vector<std::string>& initial, const std::vector<std::string>& expected) {
        auto grid = initial;
        RowCol limits = { grid.size() - 1, grid[0].size() - 1 };
        auto GetUnits = [&]() {
            std::vector<RowCol> units;
            Constexpr::ApplyToMatrixIndex(grid, [&](size_t row, size_t col) {
                if (grid[row][col] >= 'A') {
                    units.push_back({ row, col });
                }
                });
            return units;
        };
        auto Move = [&](RowCol& from, RowCol to) {
            std::swap(grid[from.Row][from.Col], grid[to.Row][to.Col]);
            from = to;
        };

        auto units = GetUnits();
        std::sort(units.begin(), units.end());
        RowCol moveTarget, firstStep;
        for (auto& unit : units) {
            auto targets = GetTargets(grid, unit, units);
            if (TryFindMoveTarget(grid, limits, unit, targets, moveTarget, firstStep)) {
                Move(unit, firstStep);
            }
        }

        return grid == expected;
    }

    constexpr bool TestBattle(const std::vector<std::string>& initial, const std::vector<std::string>& expected, const Hp& expectedHp, const size_t expectedRounds) {
        Grid grid = initial;
        auto hp = GetHp(grid);
        auto rounds = Battle(grid, hp, 3);

        if (rounds != expectedRounds) return false;
        if (grid != expected) return false;
        if (hp.size() != expectedHp.size()) return false;

        for (const auto& [key, value] : expectedHp) {
            if (!hp.contains(key)) return false;
            if (hp.at(key) != value) return false;
        }

        return true;
    }

    TESTS() {
        if (!TestMove({ "###", "#G#", "#.#", "#E#", "###" }, { "###", "#.#", "#G#", "#E#", "###" })) return false;
        if (!TestMove({ "######", "#.E..#", "#.##.#", "#.##G#", "#G####", "######" }, { "######", "#..E.#", "#.##G#", "#G##.#", "#.####", "######" })) return false;

        if(!TestBattle(
            { "#######", "#.G...#", "#...EG#", "#.#.#G#", "#..G#E#", "#.....#","#######" },
            { "#######", "#G....#", "#.G...#", "#.#.#G#", "#...#.#", "#....G#", "#######" },
            { {{1,1}, 200}, {{2,2}, 131}, {{3,5}, 59}, {{5,5}, 200 }},
            47)) return false;

        if (!TestBattle(
            { "#######", "#G..#E#", "#E#E.E#", "#G.##.#", "#...#E#", "#...E.#", "#######" },
            { "#######", "#...#E#", "#E#...#", "#.E##.#", "#E..#E#", "#.....#", "#######" },
            { {{1,5}, 200}, {{2,1}, 197}, {{3,2}, 185}, {{4,1}, 200}, {{4,5}, 200 } },
            37)) return false;
        
        if (!TestBattle(
            { "#######", "#E..EG#", "#.#G.E#", "#E.##E#", "#G..#.#", "#..E#.#", "#######" },
            { "#######", "#.E.E.#", "#.#E..#", "#E.##.#", "#.E.#.#", "#...#.#", "#######" },
            { {{1,2}, 164}, {{1,4}, 197}, {{2,3}, 200}, {{3,1}, 98}, {{4,2}, 200} },
            46)) return false;

        if (!TestBattle(
            { "#######", "#E.G#.#", "#.#G..#", "#G.#.G#", "#G..#.#", "#...E.#", "#######" },
            { "#######", "#G.G#.#", "#.#G..#", "#..#..#", "#...#G#", "#...G.#", "#######" },
            { {{1,1}, 200}, {{1,3}, 98}, {{2,3}, 200}, {{4,5}, 95}, {{5,4}, 200} },
            35)) return false;


        if (!TestBattle(
            { "#######", "#.E...#", "#.#..G#", "#.###.#", "#E#G#G#", "#...#G#", "#######" },
            { "#######", "#.....#", "#.#G..#", "#.###.#", "#.#.#.#", "#G.G#G#", "#######" },
            { {{2,3}, 200}, {{5,1}, 98}, {{5,3}, 38}, {{5,5}, 200} },
            54)) return false;

        if (!TestBattle(
            { "#########", "#G......#", "#.E.#...#", "#..##..G#", "#...##..#", "#...#...#", "#.G...G.#", "#.....G.#", "#########" },
            { "#########", "#.G.....#", "#G.G#...#", "#.G##...#", "#...##..#", "#.G.#...#", "#.......#", "#.......#", "#########" },
            { {{1,2}, 137}, {{2,1}, 200}, {{2,3}, 200}, {{3,2}, 200}, {{5,2}, 200} },
            20)) return false;
        return true;
    }
}